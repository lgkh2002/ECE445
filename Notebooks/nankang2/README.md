### Nan Kang Lab Notebook

## 2/20/2024
Goal:
Understand basic AutoCAD operations for 3D modeling and make a concept model of our device.

Completed:
- Watched some tutorials about making model in AutoCAD.
- Created a rough 3D outshell in AutoCAD.

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/02202024_autocad.png)

## 3/2/2024
Goal:
Understand 3.52inch_e-Paper operations.

Completed:
- Find 3.52inch_e-Paper wiki, and read SPI communication and code to print image on the screen.
- Research image format conversion for screen.

## 3/3/2024
Goal:
Understand SD Card operations.

Completed:
- Watch some tutorial video and research SD Card SPI.
- Add SD Card test code to the git.

## 3/24/2024
Goal:
Create an outline of the enclosure based on the screen dimension.

Completed:
- Realize AutoCAD is not easy to use, then start working on enclosure 3D modeling in Fusion.
- Watched tutorial video and get familiar with Fusion.
- Measure the display region dimension of the screens and make a basic case shape according to major parts' sizes.

## 3/25/2024
Goal:
Add some screw holes to the model to hold the enclosure part together.

Completed:
- I excluded a face at the connection of the enclosure bottom case to prevent sliding when closing the enclosure and added a offset on it to make sure it has extra space to close the enclosure.

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/03252024_connection.png)

- I excluded some screw holes inside the bottom part so screws will hide in the holes without hurting fingers while useing.

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/03252024_screw.png)

## 3/26/2024
Goal:
Add screen holders on the enclosure.

Completed:
- I spent a lot of time on designing the way to hold the screen. Since these two screens are placed together very closely, it is impossible to use 4 holders on each one. And I also need to consider if these holders will prevert me to insert both screens into the slot.
- Another thing to consider about is the back side of the screen. Since the screen has a thick port on the bottom side, to save more space for our PCB board, I decided to place one of them up-side-down.

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/03262024_screen_holder.png)

## 3/29/2024
Goal:
Find way to fix the closed box.

Completed:
- I reasearched on screw size and drillling and decided to use M3 screws for the enclosure and M2.5 screws for the PCB board.
- I realized two screens will be very close and make it difficult to insert screen so I resizeed the upper screen position and left more space around.
- Later, I measured the part dimension and added button holes, speaker hole, pcb holder.

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/03292024_enclosure.png)

## 4/2/2024
Goal:
Be able to print text and image on the screen.

Completed:
- Used an arduino to test for the screen code.

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/04022024_screen_img.png)

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/04022024_screen_text.png)

## 4/3/2024
Goal:
Finalize some details on the enclosure.

Completed:
- Measure parts dimension and adjust the inner height and holder position of the enclosure.
- Submitted a consultation form to idea lab asking some confusion about 3D printing.

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/04032024_adjust_height.png)

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/04032024_adjusted_enclosure.png)

## 4/6/2024
Goal:
Submit 3D printing request.

Completed
- Still didn't get a response from idea lab after I submitted the consultation form, and I submitted the enclosure 3D model printing request directly.
- CompleteD screen word wrap function by an arduino, so that words will wrap at the last space on each line.

## 4/8/2024
Goal:
Make sure the screen code work on our PCB.

Completed:
- Tried to modify the arduino screen code on our PCB but failed, then switch to esp32 code.

## 4/9/2024
Goal:
Make sure the screen code work on our PCB.

Completed:
- Found that there are some pin assignment in esp32 code and tried to ressigned the pin in the esp32, still doesn't work.

## 4/13/2024
Goal:
Implement text printing on our PCB.

Completed:
- After several hours debug by my partner, we found that DC pin of the screen wasn' connected, then after soldering a wire on it, the screen code works.
- Modify the esp32 screen code, now it is working with a word wrap.

![Image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/04132024_smart_word_wrap.jpg)

## 4/17/2024
Goal:
Be able to display pet image on the screen.

Completed:
- Transform the pet image to binary format and write the pet display function according to the status.

![video](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/04172024_pet_emo.mov)

## 4/22/2024
Goal:
Assemby the enclosure and make sure every thing fit.

Completed:
- Since the 3D printed enclosure wasn't very smooth, and the holes for part holders were terriblae, I polished the printed enclosure by sandpaper and drilled some holes using a grinder.

![image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/04222024_before_polished.jpg)

- After a long time of polishing, we are able to assemble everything tightly inside the enclosure.

![image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/04222024_assemble.jpg)

![image](https://github.com/lgkh2002/ECE445/blob/main/Notebooks/nankang2/04222024_assemble2.jpg)

- We decided to show an overall health status on the device after daily practice, so I added more pet images for health status.