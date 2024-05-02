## Daniel Chamoun Worklog


### 01/24/2024 10:16 PM
Goals: Finalize final project idea and come up with a name

Completed: Name of Project has been decided: JargonJolt!

### 01/29/2024
Goals: Complete RFA document

Completed: Started RFA document 

### 01/30/2024
Goals: Double check on the ECE445 board that our project is approved 

Completed: Project Proposal approved

Our solution is the JargonJolt, a digital pet and portable flashcard device that makes consistently practicing your language skills convenient and fun! The JargonJolt will take advantage of the “tamagotchi effect”. Named after the popular toy by Bandai, the tamagotchi effect is the phenomenon of humans becoming emotionally attached to machines, robots, or otherwise inanimate entities. We plan to harness this aspect of human psychology to encourage people to keep up with their daily language review and practice. Nurturing/playing with a digital pet who gets happier as you do better in your flashcard reviews will keep flashcard users more engaged during their reviews as well as more consistent. 


### 2/09/2024
Goal: Create Team Contract with team which includes a schedule for dates everyone is available to meet in person and online

Completed: Team Contract submitted 

### 2/13/2024
Goal: Clarify difficulties I think that we will experience. Regarding memory spaces, I believe there to be some necessity to using an SD card if we plan to store a large number of audio files

Completed: 
First TA meeting
- We talked about storage concerns when using SDRAM
- We finalized that instead of SDRAM we would just use an SD card instead

### 2/16/2024
Goal: Get started on our Internet submodule

Completed:
Started Arduino IDE project
- Researched ESP32S3 Internet module
- Continued work on the KiCad schematic, most notably the pinout for the ESP32S3WROOM
  
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/1.png)

After looking more into it, Internet module should be the last thing that we implement, the control should be the first.
Verifying that we can program to the ESP32 is the most important, so we looked at diagrams on the ECE 445 wiki regarding setting up strapping pins on the ESP32S3WROOM.

### 2/16/2024
Goal: Finish the control subsystem in KiCad, Setup SPI in KiCad

Completed:
- Researched how SPI connections work with the ESP32 using Arduino IDE (All SPI pins are reassignable in Arudino IDE, so no need to worry about using specific ESP32 pins)
- Found and imported footprint for ESP32 in KICad, making it accessible to everyone else in the git repo as it is a relatively specific version of the ESP32
  
### 2/21/2024
Goal: Finish up high level requirements and subsystem goals

Completed:
High Level Requirements:
- The device enables users to view flashcards, see answers, select their results, and monitor the status of a digital pet. Flipping and switching between flashcards must be completed within 1 second, and the digital pet should respond to any state changes within 1 second.
- The device must have the capacity to store and recall ‘question and answer’ data for up to 500 flashcards, in addition to retaining user interaction history with the flashcard set. Furthermore, it should be capable of downloading flashcard sets from the internet in under 5 minutes.
- The device should be portable, with dimensions not exceeding 160mm x 120mm, and designed for long-term use. It must feature a rechargeable battery with a lifespan of at least 2 hours on a single charge.

Subsystem Goals:
- Completed with detailed descriptions for verification methods once the PCB is populated and testing has begun.

### 2/22/2024
Goal: Update buttons from simple capacitor debounced buttons to Schmidtt trigger debouncing

Completed:
- Switched buttons to be used with a Schmidtt trigger to prevent doubling user inputs as opposed to just a simple capacitor
- Ordered parts
  
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/2.webp)

  
### 2/29/2024
Goal: Finish Memory Subsystem in the KiCad schematic, research more on the memory subsystem / microSD card module

Completed:
-   Finished Memory Subsystem in both the schematic and the PCB within KiCad
-   Ordered MicroSD Card Reader
-   Researched E-Paper Driver HAT by Waveshare
-   Ordered more parts

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/3.png)


### 3/1/2024
Goal: Add audio in the UI subsystem to the KiCad schematic and PCB

Completed:
- Added I2S schematic/subsystem into KiCad
- Updated PCB design with I2S traces as well as audio connectors


### 3/2/2024
Goal: Assigned/Define GPIO pins within Arduino IDE to correspond to the ESP32 pins

Completed:
- Researched how GPIO pins are defined
- GPIO pins are completely defineable for the ESP32S3 within Arduino IDE
Current pin setup where the number is the corresponding GPIO number:
#define vsck 40
#define vmiso 41
#define vmosi 42
#define vcs 1

#define hsck 10
#define hmiso 11
#define hmosi 12
#define hcs 14
#define hcs2 47

#define button1 8
#define button2 9
#define button3 20


### 3/4/2024
Goal: Get audit approval from PCBWay in order to submit our first PCB order

Completed:
- Sent Gerbers to PCBWay for approval to TA (approved)
- Sent second order of parts to TA
- Decided on good push buttons to use for our User Interface Subsystem

### 3/21/2024
Goal: Ensure that we have all of the parts we need to begin populating our first PCB design

Completed: Ordered more parts from the e-shop to obtain a couple of parts that we forgot to order

### 3/23/2024
Goal: Work on a second PCB design in time for the next PCB order

Completed: 
Fixed polarity of barrel jack connector pins (before we had to swap these manually which caused a lot of additional wires)

Old:

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/4.webp)

New: 

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/5.PNG)


### 3/25/2024
Goal: Continue working on new PCB design with more debug/testing features

Completed:
- Added test LED's to new PCB design for assisting when debugging
  
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/6.png)

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/7.webp)


- Added stencil request
  
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/8.webp)

  

### 3/26/2024
Goal: Submit second PCB order to TA after approving the audit with PCBWay

Completed:
- Passed second order of PCBWay Gerber Audit
- Waiting on team to get back to me on the updated schematics and if they approve of it as well, I will submit the already approved gerbers to our TA.

### 3/31/2024
Goal: Get a program flashed onto the ESP32

Completed:
Implementing GPIO pins, initial ESP32 programming tests, initial debugging with first order PCB, still have yet to directly program the ESP32 as we need a USB/UART bridge programmer

### 4/2/2024
Goal: Program to ESP32

Completed:
First successful program flased to the ESP32
- used serial monitor to confirm this
- Ordered UART programmer so we do not have to use ones in the lab
- Here you can see the orange and green wires which temporarily fixed the barrel jack connector before our second order arrived
  
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/9.webp)



### 4/4/2024
Goal: Flash program that sends data from the SD card reader to the ESP32 and back, day 1

Completed:
Started work on the SD Card module
- Attempted to use a 64GB SD card which would always provide us with error message "Card mount failed"
 
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/10.png)

- Used a default ESP32S3 example within Arduino


### 4/5/2024
Goal: Flash program that sends data from the SD card reader to the ESP32 and back, day 2

Completed: Continuing debugging SD Card issues

### 4/6/2024
Goal: Flash program that sends data from the SD card reader to the ESP32 and back, day 3

Completed:
Next day of SD Card tests began
- Read some threads online regarding faulty SD cards not being configured with the ESP32 library, so we bought a new SD card from a store
- Issue was our 64GB SD card, swapped to a 32GB size
- In addition, changing default HSPI and VSPI values fixed the issue
- SD Card module fully functional with text data

### 4/8/2024
Goal: Get Waveshare library working on our PCB

Completed:
- Started configuring Waveshare library via their example code

### 4/10/2024
Goal: Populate second PCB design with Luke

Completed:
- Picked up second PCB order, more experienced teammate taught me how to populate a board using a reflow oven and stencil for smaller solder joints

### 4/12/2024
Goal: Figure out why waveshare library was not working with our PCB

Completed:
- Had to directly solder a wire to the ESP32 in order to add DATA/COMMAND pins needed for the Waveshare library as I did not include the data and command pins when planning out the PCB. We used the middle of these X X X pins to ensure there were no issues with any solder bridges/shorts between the other pins
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/13.webp)

-  Waveshare code finally working with default settings, next step is to parse data from a flashcard text file
![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/chamoun2/images/11.PNG)

### 4/16/2024
Goal: Get audio functional with our PCB design

Completed:
- Confirmed that audio could work with our SD card using the ESP32-audioI2s module, however does not work when implemented with our code
- Library used: https://github.com/schreibfaul1/ESP32-audioI2S/wiki


### 4/17/2024-4/21/2024
Goal: Need audio to work with our PCB design and program/pin definitions, days 1-4

Completed:
Continue implementation of audio files, reading multiple threads online regarding why audioI2S files would are not playing. Still having issues

### 4/22/2024
Goal: Need audio to work with our PCB design and program/pin definitions, day 5

Completed:
- Added buttons to connectors so we can begin finishing touches.
- Finally fixed audio. Error was only reading one sample of the bitstream. The example code worked because the loop function itself was placed inside of a loop which automatically incremented the counter to the next sample. Putting the .loop into a loop in our code fixed this issue and now reads the entire bitstream.
- Helped finalize PCB by adding a switch and putting it into 3d printed encasement

### 4/23/2024
Demo Day :)
