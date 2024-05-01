# February 2nd, 2019
This is the beginning of entries for Luke Hartmann in the development of JargonJolt.
### Objectives
1. Research basics of the ESP32-23-WROOM and it's peripherals
2. Research the basics of using Arduino IDE for development
### Journal
Worked on figuring out pin assignments for the ESP 32 in KiCad, didn't see a lot of progress getting made. Seems like there are two SPI channels that are going to be usable through Arduino IDE, those being SPIs 2 and 3.
These are otherwise known as VSPI and HSPI, but I don't think the V and the H stand for anything in particular.
Anyway, things have been pretty slow so far, but here are my main concerns.

1. Keeping SRAM always powered. We're considering switching over to using an SD card since its stable and you can  also buy off the shelf parts that can read/write SD cards through SPI. This however might require killing the internet capabilities.

2. Internet. I don't know a lot about internet protocol. I'm not sure how we're going to tell the device which sets to download or where they even are. Using an SD card to just load files would reduce convenience somewhat, but it makes is much easier on our end. This product is somewhat proof of concept, so the way we get flashcard data isn't that important right now in my opinion.

3. Storing video memory. Audio memory is already going to be a headache with the amount of space it takes up if you just store it as a bunch of 16 bit samples. At 44.1KHz, which is standard, this comes out to $16\times\frac{44100}{8}=88.2$ kilobytes
per second. That just isnt something we're going to be able to afford, which means we'll have do some on board compression or down sampling or something. The video memory I can only imagine is only going to be a bigger issue. Also don't know how we're going to do the pixel art. I'm hoping however we do it we can just hard code it into the flash memory where it'll be nice and stable.

### Used resources
- https://www.espressif.com/sites/default/files/documentation/esp32-s3-wroom-1_wroom-1u_datasheet_en.pdf

# February 20, 2024
### Objectives
1. Figure out goals for the coming week
### Journal
Daniel said he's going to be working on figuring out the pinouts for the ESP32 which means I should be able to get started working on the schematic proper tomorrow or the day after that. I don't anticipate that it'll be too bad, since the majority of the complexity in this project comes from the microcontroller's job, which I hope the CE majors will be able to help me through. I did put some thought into the algorithm though, and I'll place what I
have so far here. The main thing I am not sure about in that algorithm is how we're going to go about 
the SPI routine with memory, which will be present even if we end up swapping our design's current external SRAM with an SD card. The more I think about the SD card though the better of an idea it seems like.

Goals for the next week:

1. Get the other members thinking about the MCU algorithm
2. Make a decision about memory
3. Get a rough draft of the schematic done so that we have something to show for our professor design review

### Figures
Algorithm flowchart
![Algorithm Flowchart](https://lh3.googleusercontent.com/pw/AP1GczOozcOp7-_9iRumxclQH0Iuul6fSsLIolQiMFycnHSQGe5dWqvcqObkFw3ZRaCy2xavsqxxEebhqQ2JSQeKe8P-xhdhTj9_81CNe7CBm-o_NNL2AKnd8cyn3RrYutu1-kKQy4InOsBIg-NO9L--J9KE=w551-h893-s-no?authuser=0)

# February 25th, 2024
## Journal
The design doc is finished up, and we have our design review tomorrow morning. The only piece of the design that I think still isn't really concretely decided upon is the implementation of the memory module, that being whether or not we should use the SRAM chip we found a few weeks ago or if we should instead use an SD card. I'm personally leaning towards an SD card, especially because it will allow us to test things without having to get internet working first, and it allows internet to be cut from the design completely if we can't get it to work. Furthermore, the SD card is stable memory, meaning when we want to power this thing off it an get powered off completely, saving valuable battery life. An SD card may take a little bit more power to read from and write to though, but the 3DS did it and it just had a battery so we should be fine. Honestly the more I think about it the more impressive it is that they got 3D graphics
to run so well on that little thing with 2 backlit screens. Anyway the schematic is being worked on, and is progressing well. Thankfully nothing on this board is that crazy, and most things can be plotted out at least without having to decide on the memory thing. Hopefully we can ask the professor tomorrow on his opinion and go from there.

No design real design progress made today, nothing that we didn't already have ideas about.

# February 26, 2024
### Objectives
1. Do PCB design
2. Complete design review.
### Journal
I think the design review went okay. We decided to use an SD card after all. I think we were always going to have to make this design change because without stable memory a lot of what this project should be capable of kind of goes down the toilet. The PCB design is progressing well. I think we'll be able to finish up layout after spring break.

#### Completed Tasks:
- Finished a lot of the PCB schematic
	- MCU pinout
	- debounced buttons
	- other signal routing
### Figures

Debounced Button Schematic

![debounced buttons](https://lh3.googleusercontent.com/pw/AP1GczORVfOSazwfGy5VMs0CEdBV8iaBcLNk-Id4oPbpLYV5m_UXov7supuUGDXTpWRlAhXj7oO6rmMgRcbXQB3ZdUI_WMMbLvoSHh4CqAeMMswhEBBEtbMOXBiJxTu_OVV643MhbDB_5gvP-XKmjAQFrips=w1356-h490-s-no?authuser=0)
MCU pinout Schematic
![MCU diagram](https://lh3.googleusercontent.com/pw/AP1GczM51Xq7zhXgY2kAFFWWV8M6HA38_U6wakS0Nat9J6PbV_h7JnexkIwUyvyBcwteXGviBNgjmilRF6mKCdQ_GLUJQ_-Ry5V7rtoKLq71QdD_bG6BZeFT_OOwh5U1dJYKpnfIujUmjzUnrgwhSbh46_gj=w1246-h787-s-no?authuser=0)

 # March 21 2024
 ### Objectives
 1. Assemble PCB
 2. Test PCB basic functionality

### Journal
Parts and the PCB came in today. I assembled it, mostly via hand soldering because we forgot to order a stencil. There are already some improvements that can be made to the next version of the board

- Make the board a little bigger maybe, we aren't hurting for space so might as well extend the board and space things out. It'll make things easier to test
- Add LEDs that indicate power is coming out of the linear regulator and LEDs that indicate when we are transmitting on the SPI lines. These are mostly debugging tools and can be desoldered to save power when its time to put everything together in the box
- Switch the terminals for the barrel jack connector. The pinout wasn't clear on the data sheet and i seem to have the pad assignments switched.

Also missing just a couple parts for fully completing the PCB. We should have them next week and I'm planning to do preliminary testing on it as soon as I can next week. I think Nancy has been dabbling in the programming side,
so I'll talk to her to see if we can also test the basic capabilities of the ESP 32. This does mean I was unable to do any real testing on the PCB.

### Figures

Mostly populated PCB v1.0.0
![enter image description here](https://lh3.googleusercontent.com/pw/AP1GczNBmuy5LHk_iiOufloTgItXHcxg8ZRe0S0qU6Ov-I7-QXmrD0hIm7yH0ID7cFx9-Z3M4z6pLqXQ0tv2ne92vMKz9rBuAqfQKpiRg6Amt6zOQbGLwx_qfxXc8unx6q7oVnfqrJg_zB6v0xvk1fQpLyMh=w435-h580-s-no?authuser=0)

# March 28, 2024
### Objectives
1. Work on individual progress report
2. Think about memory speed and size
### Journal
Worked on the individual progress report. In the process did some calculations involving the memory timing and power usage.

Assuming a 100MHz serial clock and a flashcard size allocation of 100kb, power of flashcard read/write

$100000 \text{ bytes} \times 8\frac{\text{bits}}{\text{byte}} \times \frac{1\text{s}}{1000000\text{ bits}} \times 0.0825 W = 0.066 \text{ joules}$

to read/write one flashcard from/to memory. Found the current cost of SPI writing to 
SD card to be 25mA at 3.3v, multipled to get power.

Timing of getting new/seen status from all cards.
requires 152 bits sent, includes two 6 byte control sequences from MCU, data, and some response/status bits from SD card

$500 \times 152 \text{ bits} \times \frac{1\text{ s}}{1000000\text{ bits}} = 0.079 s$

a perfectly acceptable amount of time, non even really noticeable by a human.

However, doing the individual progress report has been a wake up call, and I hope it serves the same for the rest of the team as well. We simply have not made enough progress, the PCB is not even tested yet. If we want to finish this project, we need to start moving a lot faster. 

# March 29, 2024

### Objectives
1. Finished soldering PCB with remaining parts
2. Test basic hardware functionality
### Journal

Finished soldering board. Should be ready for ESP32 programming test next week. All I had to add was Q1 and Q2 for programming. Also soldered pins to the I2S amplifier

Tested 3.3v power rail regulated down from 5v through barrel jack port. Because the pins were flipped I couldn't actually use the barrel jack, I needed to just solder wires to the correct pins and power them with a bench top DC source. Was able to see 3.295v measured at output terminal of linear regulator passed, within spec of 0.1 volts of 3.3v target. Note about the figure for this entry, these are the correct pad placements, but we should not be applying 12v to the pin. Turns out the linear regulator can only be supplied by a maximum of 6v. Barrel jack bricks that supply less than that are common, so using it should still not be an issue.

### Figures

Correct pad assignments for barrel jack
![enter image description here](https://lh3.googleusercontent.com/pw/AP1GczPZdu9um2gBWtTrY05FRBWtFV1gG04i4DIz-wifsffiU8-y1PScuYVJUGNl54cNlKYkJOphRUEsKAb0-cSccLIfiAe5djScKtoDn1NKeMQu2Lpo5iQOwWHQbmnj3TVnZU4uog3lqmZZJ_C3a6xLO3gC=w222-h301-s-no?authuser=0)


### Used Resources

- https://www.digikey.com/htmldatasheets/production/53270/0/0/1/adp3339.html

# April 1st, 2024

### Objectives

1. Program ESP32 with simple code to test it can be programmed
2. Fix power issue Daniel encountered

### Journal 

Continued testing with ESP32 and PCB

Daniel encountered some issue with powering the board the other day. Turns out it was just an issue with the 'on' switch. It's just a 2 pin header that if shorted, connects the output of the linear regulator to the rest of the 3.3v rail for the whole circuit. Those two pins need to be connected with a wire or something to deliver power to everything else

#### Completed Tasks:
- Successfully flashed circuit board with code that causes all of the SPI chip select lines
to blink low and high. Measured the output of these chips with an oscilloscope and found that it did work

>Some notes about programming the ESP32 with USB to UART bridge
>- Make sure not to mix up RX and TX pins. If something doesn't work, they're probably flipped
> - Seems like the program wont begin until the USB to UART bridge is unplugged. This might be a glitch,
	 but just unplugging it makes everything run fine so not a big deal

- successfully flashed ESP32 with a program that reads the input from button 1 and drives CS2 (pad 24 in KiCad) high when the button is pressed. Output pin measured again with an oscilloscope

With these tests, the circuit board is basically verified to be working properly. It can be programmed, is capable
of driving pins high, and is capable of reading input pins and responding accordingly. The 2nd version of the PCB
that is coming in next week will include some nice quality of life changes, but the project could theoretically
work just fine with the v1.0.0 board that we're using now. Now that the circuitry works as far as we know, its time
to jump into the actual coding more seriously.

We also need to buy our own USB to UART bridge, we just borrowed some guy in the lab's 

# April 6, 2024

### Objectives
1. Get SPI working 
2. Talk to the SD card

### Journal

Tried to get the SD card things running. Daniel did a lot of work on it earlier today and was unsuccessful so now
trying it out

- MOSI is not outputting
- SCLK is not outputting

both go high briefly when the SD card begin process is run, but the overall process still fails of course

Probable causes:
- SD card not formatted correctly, can't format our 64 GB micro SD card in FAT32. Every tutorial I've found says to use this format
- Pull up resistors on the SD card module are causing problems, pull all floating signals high. I suspect this because I found an article about it on Espressif's website

Tried all output configurations for the SD card SPI pins, didn't do anything helpful. Chip select seems to be doing something, but it isn't discernible what. Will buy a 32GB SD card and try again tomorrow

### Used Resources
 - https://docs.espressif.com/projects/esp-idf/en/v4.3.4/esp32/api-reference/peripherals/sd_pullup_requirements.html
 - https://randomnerdtutorials.com/esp32-microsd-card-arduino/

# April 7th 2024
The SD card finally works

### Objectives
1. Get SD card working

### Journal
Changed declaration of SPIClass handler to doing it with a pointer

Old:

SPIClass vspi = SPIClass(VSPI);

where VSPI is defined as 3 as per the datasheet

new:

SPIClass * vspi = NULL;

vspi = new SPIClass(VSPI);

Where VSPI defined as 0.

also changed the definition of VSPI to 0. 1 also works, but 2 and 3 don't, which is odd because most sources say
2 and 3 correspond to HSPI and VSPI respectively, which are the two SPI channels which are typically available
to users. However one of the SPI related includes seems to imply that isn't the case for the ESP32S3 and a few other ESP32 chips, so it makes sense that these are the numbers that work instead. As long as we have 2 SPI channels working it doesn't really matter which ones.

Included is an oscilloscope trace of a working serial clock signal for a short arbitrary bit write to test SPI. This is the first time we've seen an actually interesting trace being output from the ESP32

### Figures

SCK trace for arbitrary data transfer through SPI
![enter image description here](https://lh3.googleusercontent.com/pw/AP1GczOGl8iYVHETiFRTnMfs7gE67W2a65RhyKiE3l2zg2IrQUX2RnYHRevAGRlveTdKe8o79MQ_Fjw5S95Vm3Ipct3cSBIUUL5Y3-2qt7DgHpGvnG1JjywLjod74ALi_Pj-Ucbc_wWTAyDWpqReZdX-zLcr=w1190-h893-s-no?authuser=0)
# April 8th, 2024
### Objectives
1. Test SD card IO more extensively
2. Work on SD card reading and writing for main code
### Journal
Worked a lot on the file format of the SD card.
There will be 3 metadata text files for the flashcard set

newstatus.txt:

stores whether or not a flashcard has been seen by the user yet, or is 'new'. Important because a limited number of new cards may be seen per day and new cards are prioritized in the total daily card limit.

is just a string of ones and zeros. Example contents:

11111111111111100000000000000000000

the xth digit is 1 if the xth card in the set has been seen, 0 if it is new. The number of digits is equal to the number of cards in the set

learnstatus.txt:

stores how well the user has learned each card. Important because it affects the number of days until a card should
be reviewed.

Is a string of digits from 0 to 9. Example contents:

77786787676565564356433443332211100

the xth digit tells how well the user knows the xth card in the set. 0 for not at all, 9 for mastery.

revstatus.txt:
tells how many days/practice sessions remain until a card will be reviewed. 

Is a string of 2 digit numbers. Numbers less than 10 will have a leading 0. Example contents:

232018171819180917061314161118091607200110170605070303010000

the 2*xth and 2*x+1th digit tell the days until review for the xth card. 

Psudocode for SD card initialization:

>if newstatus.txt exists:
	>>read newstatus.txt
	>
>else:
	>>create newstatus.txt initialized to zeros
	read newstatus.txt
>
>if revstatus.txt exists:
	>>read revstatus.txt
>	
>else:
	>>create revstatus.txt initialized to 00s		//revstatus is only considered for cards that are NOT new
	>>read revstatus.txt		
>
>if learnstatus.txt exists:
	>>read learnstatus.txt
	>
>else:
	>>create learnstatus.txt initialized to zeros
	>>read learnstatus.txt

Will continue with runtime code soon.

# April 10th, 2024
### Objectives
1. Work on main runtime code
2. Edit algorithm flowchart to better match the design that actually gets implemented

### Journal
Began working on main runtime code. Making some small changes to the flowchart from the design document.

1. All references to SRAM of course replaced with SD card. Very happy we made this change.
2. Setting the days until review not as a card is finished for the day, but all at once once all cards are finished
3. Update the learn status of a card after every answer. Decrement if user is not confident, increment if they are.

It's going very smoothly. Will add the updated flowchart into the notebook.

Right now, the code is capable of the following:

1. Check for existing flashcard metadata, create it if not present
2. Decide of which flashcards to be 'active cards' for the day. Proritize new cards to a certain limit, then
   fill in up to another limit with seen cards up for review
3. Choose cards from the active list randomly
4. Respond to user button press to both flip cards and give feedback
5. update learn status based on the user responses
6. Recognize when all cards have been answered correctly and wrap up
7. update new status for newly seen cards, set the correct days until review for cards looked at today
8. decrement days until review for all other cards in the set.

### Figures
![enter image description here](https://lh3.googleusercontent.com/pw/AP1GczMiMup-R4Ck3dn1vQ1BCJGonyltYxrw6rXCI2GAj0uwD_YAuqUkjaCZGNE_LLji3nx1aHPN0TP6S1cFDg-8mmx1xZo3JiTaWz9jd5qb2Y7JjpFoIDlWu7C6OA40PDwvCyXcInaIUjYskCdhrN5pxv4j=w681-h893-s-no?authuser=0)
# April 13th, 2024
### Objectives
1. Figure out how to draw stuff on the screens
2. Get dual screens working

### Journal

Taking a bit of a detour to work on the screen displays. Daniel figured out earlier we needed to add a wire for the DC line on the screens, which controls the operation mode of the screen.

We found a good library to use the Waveshare digital ink screens that works with the ESP32.

Even so, there's a bit of a learning curve. Here are the steps used to write an image to the screen.

	DEV_Module_Init();  //initialize bitmaps, in seperate file called dev_config.cpp

	EPD_3IN52_Init();	//initalize waveshare

	EPD_3IN52_display_NUM(EPD_3IN52_WHITE);    //ID of device
	EPD_3IN52_lut_GC();						   //refresh and clear
	EPD_3IN52_refresh();

	EPD_3IN52_SendCommand(0x50);	//not sure what these do exactly, but they need to be send at initialization
    EPD_3IN52_SendData(0x17);

    DEV_Delay_ms(500);

    //Create a new image cache, allocate memory for the image to send to the waveshare
    UBYTE *Image;
    /* you have to edit the startup_stm32fxxx.s file and set a big enough heap size */
    UWORD Imagesize = ((EPD_3IN52_WIDTH % 8 == 0)? (EPD_3IN52_WIDTH / 8 ): (EPD_3IN52_WIDTH / 8 + 1)) * EPD_3IN52_HEIGHT;
    if((Image = (UBYTE *)malloc(Imagesize)) == NULL) {
        printf("Failed to apply for black memory...\r\n");
    }

    printf("Paint_NewImage\r\n");
    Paint_NewImage(Image, EPD_3IN52_WIDTH, EPD_3IN52_HEIGHT, 270, WHITE);
    Paint_Clear(WHITE);
	
    //At this point, the initialization ends, and these next lines can be repeated any time to draw someting new
	
    Paint_SelectImage(Image);
    Paint_Clear(WHITE);
	Paint_DrawBitMap(gImage_3in52);	   	//this is drawing a predetermined image, but this line can be replaced by many other 'Paint' functions to draw whatever
    EPD_3IN52_display(Image);
    EPD_3IN52_lut_GC();
    EPD_3IN52_refresh();
	
	//Once you're done using the screen, put it to sleep and free the image cache.

	EPD_3IN52_sleep();

	free(Image);
	Image = NULL;
	DEV_Delay_ms(2000);//important, at least 2s
	
	//NOTE: we did NOT write most of this code, a lot of it was from Waveshare. We just put it together in a convenient way to suit our needs.
	
	
	
Essentially the steps are:

1. Initialize screen
2. allocate memory
3. create new image stored in allocated memory
4. select image 
5. draw on image
6. display image
7. refresh cycle
8. sleep screen
9. free allocated memory


Now that the screens work, added some basic things to demonstrate we can draw text whenever we want.
Now when a flashcard is shown, screen indicates a question/answer should be on screen. Also indicates what button was pressed to progress
Signals when all flashcards for the day have been answered with confidence and the program ends.

Still need to do work on getting both screens to work at the same time. 

### Figures
Some text drawn with a modified version of the text drawing code I wrote that wraps text, written by Nan.
![enter image description here](https://lh3.googleusercontent.com/pw/AP1GczMR63mRur2qqeUPDMqT-HWRccgaoLxEiGwUDu2xWsOixl11cxxmaXuqET2M0ukMmpIJiBb8yeQYyuM3bL2p4aWZJxhQVahsxZRdYjPuwq2bzyStxH9FDEAyDu_ekwMsKrsoecHyzVYWh60ZGyqTIsXC=w774-h580-s-no?authuser=0)

# April 16th 2024
### Objectives
1. Get dual screens working to display the pet
2. Get ready for mock demo

### Journal
Nancy managed to rip some images to use for the digital pet and stored them in a .c file that's included in the project. She got them to display on a screen, but just one.

Both screens now work. I don't fully understand why, but screen 1 is favored. Screen 1 can be set independently, but screen 2 must be set simultaneously with screen 1 or bugs occur. This should be fine as if screen 2 is the pet, it will only update with an update on the flashcard screen anyway.

To get it to work I had to edit Waveshare's library a bit. Despite being SPI devices with CS lines, there's no support for two screens on the same SPI channel. When you initialize a screen with Waveshare's code, it uses whatever chip select pin you put into dev_config.h. There's no way to initialize separate screens with different CS pins, or to send a command while specifying a certain CS. 

To solve this I had to go into the library and remove every instance of the code setting CS to do an operation. We'll just set CS ourselves for the screen we want before calling the library function. For the initialization stuff we'll set both CS pins low. It does work, but as described above the behavior is a little weird. I think it just means there are some parts of the library code we don't want CS to be low, but we don't really have another option and it works well enough. Worked on getting the code in a state we can demo it for the mock demo. Now when you respond to a flashcard, the pet reacts to it with a couple of different reaction images. Nan created a function called drawpet() which I'm using a lot.


### Figures

A screen showing one of the states of the digital pet. I used it for a positive question response.
![enter image description here](https://lh3.googleusercontent.com/pw/AP1GczOYIsmt7-NOV0-fNgsUFb8c9EZ4FYzQcB7FFmDwd72lU-7UdpzlHoGy8ppE8-npExuKUoS2q8lsDLsOdU1hVAD_rWJeTY79ny_TuvvOSIdfihggQJhSMe2C7t0oYyqf_Pbt4vtKojWrNsbF1l8xU8eW=w774-h580-s-no?authuser=0)
### Used Resources:
- https://www.waveshare.com/wiki/E-Paper_ESP32_Driver_Board
- https://files.waveshare.com/upload/4/4a/E-Paper_ESP32_Driver_Board_user_manual_en.pdf

# April 17th, 2024
### Objectives
1. Work on runtime code 
2. Get audio output to work
### Journal
The audio does not work. It works fine in some example code we found, but it seems to be completely broken if we try to get it to work in the main code we already have. Seems like something isn't running, because certain commands print things to the serial monitor when they run, but this isn't happening for the same lines in our code. The code is really only a few lines, so its really confusing why this doesn't work. 

	audio.setPinout(I2S_BCLK, I2S_LRC, I2S_DOUT);
	audio.setVolume(21); // default 0...21
	audio.connecttoFS(SD, "/sample-15s.mp3"); // SD
	
	void  loop()
	{
		audio.loop();
	}

Other than code for setting up the SD card, which we already have and know works, this is it. It works in isolation but not in our main code.

In other news, the runtime code is progressing. We can use the two screens to show flashcard questions and answers, and it responds to button presses, which at the moment is just touching the two pins the buttons will be soldered to with something metal to short them.

### Used Resources
- https://github.com/schreibfaul1/ESP32-audioI2S

# April 19th, 2024

### Objectives
1. Work on runtime code

### Journal

Audio is getting benched because it isn't working. Nancy got more pet images, so today I worked on getting the pet to react to flashcard answers and such. Also added a new metadata text file to the SD card called pethealth.txt, which tracks how well the pet is doing overall.  Pretty low key day today.

# April 22nd, 2024 (Part 1)

### Objectives
1. Get the battery to work
2. Prepare for final demo
### Journal
Working in the lab today, getting the battery to work. We had to do some wack soldering to get the battery to be rechargeable and to power the device without having to plug/unplug it. We did some interesting soldering to get it done, shown in the figures for this section. Essentially we cut the two wires that connect the battery terminals to the pin header that plugs into the battery charger breakout board and connected them back by soldering in wires that can connect to the PCB. With this configuration we can charge the device while the device is powered no problem, and unplugging the micro USB from the charger instantly starts using the battery. Preliminary testing suggests that battery life is not going to be a problem at all.

 Nan also got the box 3D printed, and everything is getting fit into the box. The screw holes for securing the PCB didn't really print very well, so we might either have to glue it in or just be okay with it kind of sitting in there. There are so many wires crammed into the box that it probably will sit pretty snug anyway.
### Figures
Battery charging configuration
![enter image description here](https://lh3.googleusercontent.com/pw/AP1GczNc0FPHG7qzLZmEzlGZGHFiLH32sRZ7vep7SZjCgtOiaag1POkCelE9K7FpTw1QSnfsaBL-mRhF8k3ls7NjUee3pv154c9C9sD9nRvByaP-3V7cNLyVVjCjI67H1RFOCAXyULwV6pNBXEjDCo2_LLW4=w526-h431-s-no?authuser=0)
# April 22nd, 2024 (Part 2)

### Objectives
1. Get everything in the box
2. Get audio to work if possible
3. Get some internet connectivity to work if possible
4. Get everything in a presentable state for tomorrow's final demo

### Journal
The audio finally works. I was thinking about it the other day and had the idea that audio.loop() only plays a single sample of audio, which means it needs to be in some sort of loop by itself. This suspicion turns out to be right. It didn't work before because we put audio.loop() in the main loop with a bunch of other code, so of course it didn't play anything discernible. Now every time we want to play audio we put it in a while loop for a large number of loops so we know it will finish the audio clip. 

	int looper = 0;
	while(looper<100000){
		audio.loop();
		looper+=1;
	}
	
We're going to put mp3 files on the SD card, with the file names in the flashcard text file so the code knows where to find the audio for each card. 

Also got some internet stuff to work, as there's not nearly enough time now to get a web server for downloading flashcard text files to work. We can now get the date and time from the internet, so we can track how often the user has practiced. I'll have this number be used to calculate how the data in pethealth.txt changes. Also adding another text file to the SD card that saves the date of last practice session. It'll store the number of days since January 1st 1900, as that's an easy number to get from functions of the time.h include. Getting a string from the text file such as "48779" into an integer has been a consistent source of debugging, despite having to do so for all of the instances of reading numbers as data from text files, which we do a lot. atoi() doesn't work properly, so this is how I've been doing it.

	char* s;
	int dayspast=0;
	String readstring;
	readstring=readFile(SD,"/dateinfo.txt");
	s=&readstring[0];
	dayspast+=10000* (((int) *s)-48);
	s+=1;
	dayspast+=1000* (((int) *s)-48);
	s+=1;
	dayspast+=100* (((int) *s)-48);
	s+=1;
	dayspast+=10* (((int) *s)-48);
	s+=1;
	dayspast+=(((int) *s)-48);

This method works by just translating the ascii for each digit into the number itself. Then multiply by the digit's place (ones, tens, hundreds) and adding. This method requires knowing how many digits the integer we want to read is, so technically this method won't work anymore for this application once the number of days since 1900 gets larger than 100000. That's not for a pretty long time though. 

We also put everything into the 3D printed box. We did just kind of end up shoving everything in. There weren't enough holes in the box so we just had the speaker sit inside and used the speaker hole to have the charging port and the on/off switch poke out. The hole wasn't built for them, so they're just held in place by duct tape. I guess we're as ready for the final demo as we can be.

### Figures
Empty 3D printed enclosure
![enter image description here](https://lh3.googleusercontent.com/pw/AP1GczPg7VBfeIVVMPaa5AZyVJHyAxCk3X0_Jut9rjMKI2Gede0n4ySQabxRSDaNxbZcJIOQU9VFPvQ74CyKGqeIZWGkCwh4qSWJC_f22CrPh4ea5HKGF3sq64sdivMabPsS6xWqDv1Ln_czQvTSeD91asC9=w435-h580-s-no?authuser=0)
Completed Project
![enter image description here](https://lh3.googleusercontent.com/pw/AP1GczNVGDxoRoESy_B9o2d2zCNmEkSk2qkk4F0IZiTt2tZyYNJK1R_TBYjf2dtz_1tn_VgiSG08FdH5OdyL62FG8-OGWQEq_Ntl5FznklDEZf0SZihOERSvRHO3sDilHJ6fElYo07AG4YPe5PB_UiX6UB1t=w669-h893-s-no?authuser=0)
### Used Resources
- https://lastminuteengineers.com/esp32-ntp-server-date-time-tutorial/
	