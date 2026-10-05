

struct Encoder
{
  Parameters& p;

  
  Encoder(Parameters& p_)
    :p(p_)
  {

  }

  float readEncoderAngle() {
    Wire.beginTransmission(0x36);
    Wire.write(0x0C);
    Wire.endTransmission(false);
    Wire.requestFrom(0x36, 2);

    int raw = (Wire.read() << 8) | Wire.read();
    raw &= 0x0FFF;

    return raw * 360.0f / 4096.0f;
  }

  void update()
  {
    Wire.beginTransmission(0x36);
    Wire.write(0x0B);
    Wire.endTransmission(false);
    Wire.requestFrom(0x36, 1);

    uint8_t status = Wire.read();

    bool detected = status & 0x20;
    bool tooWeak = status & 0x10;
    bool tooStrong = status & 0x08;

    if (detected && !tooWeak && !tooStrong)
      Serial.println("Magnet OK");
    else if (tooWeak)
      Serial.println("Magnet too weak");
    else if (tooStrong)
      Serial.println("Magnet too strong");
    else
      Serial.println("No magnet detected");

    float encoderAngle = readEncoderAngle();

    if (detected && !tooWeak && !tooStrong) {
      Serial.print("Wind angle: ");
      Serial.println(encoderAngle);
    }
    Serial.print("---\n");
  }

  void print()
  {
    
  }
};