## Daniel Chamoun Worklog


### 01/24/2024 10:16 PM
Name of Project has been decided: JargonJolt!

### 01/29/2024
Started RFA document 

### 01/30/2024
Project Proposal approved

### 2/09/2024
Team Contract submitted 

### 2/13/2024
First TA meeting
- We talked about storage concerns when using SDRAM
- We finalized that instead of SDRAM we would just use an SD card instead

### 2/16/2024
Started Arduino IDE project
- Researched ESP32S3 Internet module
- Continued work on the KiCad schematic, most notably the pinout for the ESP32S3WROOM
  
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/1.png)


### 2/16/2024
- Researched how SPI conections work with the ESP32 using Arduino IDE (All SPI pins are reassignable in Arudino IDE, so no need to worry about using specific ESP32 pins)
- Found and imported footprint for ESP32 in KICad
### 2/21/2024
Finished up high level requirements and subsystem goals

### 2/22/2024
- Switched buttons to be used with a Schmidtt trigger to prevent doubling user inputs
- Ordered parts
  
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/2.webp)

  
### 2/29/2024
-   Finished Memory Subsystem of PCB in KiCad
-   Ordered MicroSD Card Reader
-   Researched E-Paper Driver HAT by Waveshare
-   Ordered more parts

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/3.png)


### 3/1/2024
- Added I2S schematic/subsystem into KiCad
- Updated PCB design with I2S traces as well as audio connectors


### 3/2/2024
Assigned GPIO pins within Arduino IDE to correspond to the ESP32 pins

### 3/4/2024
Sent Gerbers to PCBWay for approval to TA (approved)

### 3/4/2024
- Sent second order of parts to TA
- Decided on good push buttons to use for our User Interface Subsystem

### 3/21/2024
Ordered more parts from the e-shop

### 3/23/2024
Fixed polarity of barrel jack connector pins (before we had to swap these manually which caused a lot of additional wires)

Old:

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/4.webp)

New: 

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/5.PNG)


### 3/25/2024
- Added test LED's to new PCB design for assisting when debugging
  
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/6.png)

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/7.webp)


- Added stencil request
  
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/8.webp)

  

### 3/26/2024
- Passed second order of PCBWay Gerber Audit
- Waiting on team to get back to me on the updated schematics and if they approve of it as well, I will submit the already approved gerbers to our TA.

### 3/31/2024
Implementing GPIO pins, initial ESP32 programming tests, initial debugging with first order PCB

### 4/2/2024
First successful program flased to the ESP32
- used serial monitor to confirm this
- Here you can see the orange and green wires which temporarily fixed the barrel jack connector before our second order arrived
  
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/9.webp)



### 4/4/2024
Started work on the SD Card module
- Attempted to use a 64GB SD card which would always provide us with error message "Card mount failed"
 
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/10.png)

- Used a default ESP32S3 example within Arduino


### 4/5/2024
Continuing debugging SD Card issues

### 4/6/2024
Next day of SD Card tests began
- Read some threads online regarding faulty SD cards not being configured with the ESP32 library, so we bought a new SD card from a store
- Issue was our 64GB SD card, swapped to a 32GB size
- In addition, changing default HSPI and VSPI values fixed the issue
- SD Card module fully functional with text data

### 4/8/2024
- Started configuring Waveshare library via their example code

### 4/10/2024
- Picked up second PCB order, more experienced teammate taught me how to populate a board using a reflow oven and stencil for smaller solder joints

### 4/12/2024
- Had to directly solder a wire to the ESP32 in order to add DATA/COMMAND pins needed for the Waveshare library as I did not include the data and command pins when planning out the PCB. We used the middle of these X X X pins to ensure there were no issues with any solder bridges/shorts between the other pins
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/13.webp)

-  Waveshare code finally working with default settings, next step is to parse data from a flashcard text file
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/11.PNG)

### 4/16/2024
- Confirmed that audio could work with our SD card using the ESP32-audioI2s module, however does not work when implemented with our code
- Library used: https://github.com/schreibfaul1/ESP32-audioI2S/wiki


### 4/17/2024-4/21/2024
Continue implementation of audio files, reading multiple threads online regarding why audioI2S files would are not playing. Still having issues

### 4/22/2024
- Added buttons to connectors so we can begin finishing touches.
- Finally fixed audio. Error was only reading one sample of the bitstream. The example code worked because the loop function itself was placed inside of a loop which automatically incremented the counter to the next sample. Putting the .loop into a loop in our code fixed this issue and now reads the entire bitstream.
- Helped finalize PCB by adding a switch and putting it into 3d printed encasement

### 4/23/2024
Demo Day :)
