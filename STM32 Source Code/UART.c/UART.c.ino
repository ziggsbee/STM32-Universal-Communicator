/*
  UART Tester
  Using a STM32 WROOM board this script is used to determine the baud rate of a target device connected to GPIO 22 - Rx and GPIO 23 - Tx.
  Please ensure the ground of the STM32 is also connected to your target device.
*/

enum states {
  WAITING, PROBE, PASS_THRU
};

int baud_rates[] = {4800, 9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600};

int state = WAITING;

void setup()
{
  Serial.begin(115200);

}


void getState(){
        Serial.println("Please select a new State:");
        Serial.println("1: Probe for baud rate - A list of popular baud rates will be used to test the target for the valid baud rate. \nYou may also specify a specific baud rate to test for.");
        Serial.println("2: UART Pass Though. Please specify a baud rate you would like to test and connect the device to pins");
  
        while (!Serial.available());
        int userInput = Serial.read();
        while (Serial.available() > 0 && Serial.read() != '\n');
  
        if (userInput == '1'){
          state = PROBE;
        }
        else if (userInput == '2'){
          state = PASS_THRU;
        }
        else{
          Serial.println("Invalid Option.");
        }
  
}

void statePassThru(){
      Serial.println("Please enter the Baud Rate: ");
          
      while (!Serial.available());
          
      int userInput = Serial.parseInt();
      while (Serial.available() > 0 && Serial.read() != '\n');
      int baudRate = userInput;
      Serial2.begin(baudRate, SERIAL_8N1, 22,23);
      Serial2.setTimeout(300);
      while(true){
        int bytesRead = 0;
  
        while (bytesRead < 1){
          if(Serial2.available() > 0) {
            char c = Serial2.read();
            Serial.print(c);
          }
        }
      }
}

void testSerial(int baudRate){
  Serial2.begin(baudRate, SERIAL_8N1, 22,23);
  Serial2.setTimeout(300);
  char targetOutput[101];
  int bytesRead = 0;
  while (bytesRead < 100){
      if(Serial2.available() > 0) {
        char c = Serial2.read();
        targetOutput[bytesRead++] = c;
        }
  }
  Serial.print("Output: ");
  Serial.print(baudRate);
  Serial.print(": ");
  Serial.println(targetOutput);
  
  Serial2.end();
  while (Serial2.available() > 0)
  {
    Serial2.read();
  }
}

void stateProbe(){
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

        
      }
      else {
        Serial.println("Testing standard baud rates");
        int baudIndex = 0;
        

        while (baudIndex < (sizeof(baud_rates)/ sizeof(int))) {
          customBaud = baud_rates[baudIndex];
          testSerial(customBaud);
          baudIndex++;
        }
      }
      state = WAITING;
}


void loop() {

char userInput = ' ';

  switch (state)
  {
    case WAITING:
      getState();
    break;

    case PASS_THRU:
      statePassThru();
      break;

    case PROBE:
      stateProbe();
      
      break;
  }

  

}