

struct LoRa 
{
  Parameters& p;

  //ESP32's hardware UART interface
  HardwareSerial serialPort; //will be set in the constructor as UART2

  String message = "";
  bool isMessageValid = false; 

  LoRa(Parameters& p_)
    : p(p_),serialPort(2) 
  {
    int comSpeed = 9600; //bauds or bits per second
    //UART configuration
    //speed //8 data bits, no parity (error detectin), 1 stop bit (boundary between ) //Transmitter pin //Receiver pin
    serialPort.begin(comSpeed, SERIAL_8N1, p.pinLoRaRX, p.pinLoRaTX);
  }

  void update() 
  {
    getMessage();
    
    if (isMessageValid)
      processMessage();
  }

  //a message doesn't get processed until a ";" is found, at that point it's rendered valid
  // if messages are concatenated in the serial, they are processed one at a time 
  void getMessage()
  {
    while (serialPort.available()) 
    {
        char c = serialPort.read();
        message += c;

        //if the end of the received message is reached it will exit, but message won't be valid
        if (c == ';')
        {
          isMessageValid = true;
          break;
        }
    }
  }

  //Checks if the message has the number of variables expected, assigns values if so, otherwise it deletes the message
  void processMessage()
  {
    float receivedRudderAngle;
    float receivedSailAngle;
    char endCharacter;

    //Expects the format "rudderAngle sailAngle;", number, number, character
    if (sscanf(message.c_str(), "%f %f %c", &receivedRudderAngle, &receivedSailAngle, &endCharacter) == 3)
    {
      p.rudderAngle = receivedRudderAngle;
      p.sailAngle = receivedSailAngle;

      //sending the message back so the pc can verify the message has been received
      //if this step fails, the pc will keep sending the message until receiving confirmation for the order
      serialPort.print('c'); //sending a c before message to for confirmation that it comes from the esp
      serialPort.print(message);
      Serial.print(message); //debug
    }

    //doesn't matter if the values where assigned or not, the message gets reset
    isMessageValid = false;
    message = "";
  }

};
