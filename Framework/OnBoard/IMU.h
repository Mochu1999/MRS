#include <MPU9250.h> //Motion Processing Unit 9250/6500

struct IMU
{
  Parameters& p;

  MPU9250 mpu;


  //Calculated variables
  float heading = 0; //º with north, CCW
  float rollAngle = 0; //º
  float pitchAngle = 0;
  float yawAngle = 0;

  IMU(Parameters& p_)
    :p(p_)
  {
    if (!mpu.setup(0x68)) 
    {
      Serial.println("MPU not found");
      while (1);
    }
  }

  void update()
  {
    mpu.update();
    
  }

  void calculateAngles()
  {

    //variables you directly get from the sensor
    //linear accelerations
    float aX = 0;  //Surge //Given in gs
    float aY = 0;  //Sway
    float aZ = 0;  //Heave

    //angular velocities
    float wXX = 0;  //Roll //º/s?
    float wYY = 0;  //Pitch
    float wZZ = 0;  //Yaw

  }

  void print()
  {
    if (mpu.update()) 
    {
      Serial.print("Accel: ");
      Serial.print(mpu.getAccX());
      Serial.print(" ");
      Serial.print(mpu.getAccY());
      Serial.print(" ");
      Serial.println(mpu.getAccZ());

      Serial.print("Gyro: ");
      Serial.print(mpu.getGyroX());
      Serial.print(" ");
      Serial.print(mpu.getGyroY());
      Serial.print(" ");
      Serial.println(mpu.getGyroZ());
    }
    Serial.print("Mag: ");
    Serial.print(mpu.getMagX());
    Serial.print(" ");
    Serial.print(mpu.getMagY());
    Serial.print(" ");
    Serial.println(mpu.getMagZ());
  }
};