/*
  UART Tester
  Using a STM32 WROOM board this script is used to determine the baud rate of a target device connected to GPIO 10 - Rx and GPIO 11 - Tx.
  Please ensure the ground of the STM32 is also connected to your target device.
*/

enum states {
  WAITING, PROBE, PASS_THRU
};

int baud_rates[] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

int state = PROBE;

void setup()
{
  Serial.begin(115200);

}



void loop() {


  switch (state)
  {
    case WAITING:
      Serial.println("Please select a new State:");
      Serial.println("1: Probe for baud rate - A list of popular baud rates will be used to test the target for the valid baud rate. \nYou may also specify a specific baud rate to test for.");
      Serial.println("2: UART Pass Though. Please specify a baud rate you would like to test and connect the device to pins");
      break;

    case PASS_THRU:
      Serial.println("PASS THRU");
      break;

    case PROBE:
      Serial.println("Would you like to test a custom baud rate? (Y/N): ");
      
      while (!Serial.available());
      
      char userInput = Serial.read();
      while (Serial.available() > 0 && Serial.read() != '\n');
      
      while (userInput != 'Y' && userInput != 'N' && userInput != 'y' && userInput != 'n') {
        Serial.println("Incorrect Option. Would you like to test a custom baud rate? (Y/N): ");
        while (!Serial.available());
        userInput = Serial.read();
        while (Serial.available() > 0 && Serial.read() != '\n');

      }
      int customBaud = 0;
      if( userInput == 'Y' || userInput == 'y'){
            Serial.println("Please enter the Baud Rate: ");
          
          while (!Serial.available());
          
          int userInput = Serial.parseInt();
          while (Serial.available() > 0 && Serial.read() != '\n');
          

          customBaud = userInput;

//We were able to get the custom baudrate so far. Need to work on the below code to actually get the 2nd serial connection working.
          Serial2.begin(customBaud, SERIAL_8N1, 10,11);
          Serial2.setTimeout(300);
          char targetOutput[101];
          
          int bytesRead = 0;

          while (bytesRead < 100){
            if(Serial2.available() > 0) {
              char c = Serial2.read();
              targetOutput[bytesRead++] = c;
              
            }
          }

          Serial2.print("Output: ");
          Serial.println(targetOutput);
        
      }
      else {
        
      }

      break;



  }

}
