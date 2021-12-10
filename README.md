# Engine-Monitoring-Project

This is for the Engine Monitoring Project

Allowed Authors:

Tho Trinh
Nhi Nguyen
Robert Tebeau
Pete Pike
Triston Warren

First, let’s get started with downloading the Arduino IDE. Click this link https://www.arduino.cc/en/software . Then, install the IDE with the onsite instructions. After the IDE is installed, we can move on to the next step!

Secondly, let’s copy the current code into an Arduino “sketch” (another name for a file). Click this link to see the github repository where we keep the current code. Next, copy the entire file and paste it into the currently opened Arduino sketch. Make sure to save this file, and feel free to name it what you want! Now, you will need to connect the board to your computer. Connect the microUSB end into the Arduino and the USB A end into your computer.

Thirdly, let’s get the setup to work with the board. So, we are using the Arduino Due. So, open the “Tools” tab, then hover your mouse over “Boards”. Click on “Boards Manager.” In the search bar of the window that just opened up, type in “Due.” Then, click install for the tab that says “Arduino SAM Boards.” After installation (this may take awhile), go back to Board Manager and select the “Arduino SAM Boards” option. This step is assuming you are using the Due Board. If using a different board, instruction will differ.

Fourthly, you will want to select the correct COM port. We will use the Native Port. You might want to cycle through the available COM ports if the default COM port doesn’t correctly upload to the board.

If there is any trouble with set-up so far, consult this webpage: https://www.arduino.cc/en/Guide/ArduinoDue .

Fifthly, you will need to install the Adafruit Library. Go to Sketch-> Include Library -> Manage Libraries. Search in the search bar “Adafruit RGB LCD Shield Library.” Install that selected library. Then, search in the search bar “HX711_ADC”. Install the library by Olab Kallhovd.

Now, you will want to click the checkmark button to see if everything loads correctly. Clicking the “right arrow” button will upload the program to the board. The board should automatically load the program now when you switch the power on. This means, unless you want to change the code again, that the board should run the program automatically without needing to be plugged in to a laptop.

If you are using the serial monitor on the Arduino Application, make sure to set the baud rate to 9600. 

