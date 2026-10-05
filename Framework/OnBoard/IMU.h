#include <MPU9250.h>
MPU9250 mpu;

struct IMU
{
  Parameters& p;

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