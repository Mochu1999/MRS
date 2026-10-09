#include <MPU9250.h> //Motion Processing Unit 9250/6500

struct IMU
{
  Parameters& p;

  MPU9250 mpu;

  
  float magneticDeclination = 1.56; //º east of true north at Valencia //Correction from true and magnetic north in º

  bool ready = 0;
  
  //Calculated variables
  float heading = 0; //º
  float rollAngle = 0;
  float pitchAngle = 0;


  IMU(Parameters& p_)
    :p(p_)
  {
    ready = begin();
  }

  bool begin()
  {
    ready = mpu.setup(p.addressIMU);
    if (!ready)
    {
      return false;
    }
    mpu.setMagneticDeclination(magneticDeclination);
    return true;
  }

  void update()
  {
    if (!ready) return;


    // true only when the sensor has a new sample (200 Hz by default)
    if (mpu.update())
    {
      rollAngle = mpu.getRoll(); // wrapped to {-180º, 180º}
      pitchAngle = mpu.getPitch();
      heading = mpu.getYaw();
    }
  }


  void print()
  {
       
    if (!ready)
    {
      Serial.println("IMU not ready");
      return;
    }

    Serial.print("Heading: ");
    Serial.print(heading);
    Serial.println(" deg");

    Serial.print("Roll: ");
    Serial.print(rollAngle);
    Serial.println(" deg");

    Serial.print("Pitch: ");
    Serial.print(pitchAngle);
    Serial.println(" deg");

  
    // Serial.println("Raw data: ");

    // Serial.print("Accel: ");
    // Serial.print(mpu.getAccX());
    // Serial.print(" ");
    // Serial.print(mpu.getAccY());
    // Serial.print(" ");
    // Serial.println(mpu.getAccZ());

    // Serial.print("Gyro: ");
    // Serial.print(mpu.getGyroX());
    // Serial.print(" ");
    // Serial.print(mpu.getGyroY());
    // Serial.print(" ");
    // Serial.println(mpu.getGyroZ());

    // Serial.print("Mag: ");
    // Serial.print(mpu.getMagX());
    // Serial.print(" ");
    // Serial.print(mpu.getMagY());
    // Serial.print(" ");
    // Serial.println(mpu.getMagZ());
  }
};