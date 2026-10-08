
#include <Adafruit_GPS.h>

//Currently it doesn't distinguises between GGA (position, satellites, hdop) and RMC (speed and angle) from the nmea string
//The criteria for deciding if  
struct GPS
{
  Parameters& p;

  HardwareSerial serialPort; //assigned uart
  Adafruit_GPS handle; 


  //Variables are only stored as member variables when they score above the previous saved score*timeMultiplier
  float score = 0; //from 0 to 1, ranks how good a saved set of data is

  unsigned long lastMeasureTime = 0;
  unsigned long elapsedTime = 0;
  float timeHalfLife = 15000; //ms //for the timeMultiplier, which will be 1 if elapsed time is 0; 0.5 if it is timeHalfLife

  //These 4 variables are the ones we are interested on processing
  float longitude = 0; //degrees
  float latitude = 0;
  float speed = 0; //GPS O DOPPLER?
  float angle = 0; //RUTA O BRUJULA?

  //DEBUG VARIABLES TO IMPROVE THE ALGORITHM. TO COMMENT IN FINAL VERSION
  uint8_t satellites = 0;
  float hdop = 0;

  //Variables to determine score:
  //bool handle.fix: is the measurement valid
  //uint8_t handle.satellites: available satellites
  //uint8_t fixQuality: 0 no fix, 1 fix, 2 fix improved with external reference systems
  //float hdop: how reliable the hororizontal pos is based on how well the satellites are spread across the sky
  // ~1 very good ~3 mediocre >5 very bad
  float calculateNewScore()
  {
    if(!handle.fix) return 0;

    float newScore = 1; //starting value

    if (handle.fixquality!=2)
      newScore *= 0.9;
    
    //To incentive measures over a threshold, although it's already accounted in hdop
    if (handle.satellites<=5)
      newScore *= 0.5;
    
    //the closer HDOP is to 0 the better (10 satellites might give ~0.8, 7 sat ~1.4)
    //if hdop of 0.5 will be 1, 3 will be 0, 1 will be 0.64 
    float hdopWeight = (3.0f- handle.HDOP)/2.5f;
    hdopWeight = constrain(hdopWeight, 0.0f, 1.0f); //clipping it between 0 and 1

    newScore *= (hdopWeight * hdopWeight); //a more pronounced curve
    return newScore;
  }

  

  
  GPS(Parameters& p_)
    :p(p_), serialPort(2), handle(&serialPort)
  {
    serialPort.begin(9600, SERIAL_8N1, p.pinGPSRX, p.pinGPSTX);
    handle.begin(9600);
  }

  void update()
  {
    elapsedTime = millis() - lastMeasureTime;


    //Trying to constantly read the serial NMEA buffer is the intended implementation
    while(serialPort.available()) 
    {
      if(handle.newNMEAreceived())
        {
          if(handle.parse(handle.lastNMEA()))
          {
            float newScore =calculateNewScore();
            float timeMultiplier = pow(0.5f, elapsedTime / timeHalfLife);

            if (newScore > score*timeMultiplier)
              {
                score = newScore;
                lastMeasureTime = millis();
                elapsedTime = 0;

                longitude = handle.longitudeDegrees;
                latitude = handle.latitudeDegrees;
                speed = handle.speed * 0.514444f;
                angle = handle.angle;

                satellites = handle.satellites;
                hdop = handle.HDOP;
            }
          }
        }
    }
  }

  void print()
  {

      Serial.print("GPS data from ");
      Serial.print(lastMeasureTime/1000);
      Serial.println("s");
      

      Serial.print("Lat: ");
      Serial.println(latitude, 6);

      Serial.print("Lon: ");
      Serial.println(longitude, 6);

      Serial.print("Angle: ");
      Serial.print(angle);
      Serial.println(" deg");

      Serial.print("Speed: ");
      Serial.print(speed); //m/s
      Serial.println(" m/s");

      Serial.print("Score: ");
      Serial.println(score);

      Serial.print("Satellites: ");
      Serial.println(satellites);
      
      Serial.print("HDOP: ");
      Serial.println(hdop);

      // Serial.print("Altitude: ");
      // Serial.print(handle.altitude);
      // Serial.println(" m");

    
  }


};