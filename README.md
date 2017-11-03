# Feature based image morphing tool

## Introduction

This application implement the Feature based image morphing tool which apply [Beier–Neely morphing algorithm](https://en.wikipedia.org/wiki/Beier%E2%80%93Neely_morphing_algorithm).

![Core formula](images/phase0.png)

User may interactively place feature lines on source and destination images, deform input image based on feature lines to repaint the destination image.

![Morphing Example 1](images/phase0.png)

![Terminator Morphing](images/phase0.png)

![Hulk Morphing](images/phase0.png)

![Beckham Leonardo Morphing](images/phase0.png)

## Environments and language requirements

* Envionments: Unix-like environments which may support [Makefile](https://en.wikipedia.org/wiki/Makefile)

* Language: C++

* Prerequisite Packages: ImageMagick, OpenGL

## Run the application


1. Enter the 'code' folder and type 'make' to compile and link. Then choose images, type in the command line like blow:

` ./morph images/3.jpg images/4.jpg 0 1 2`

  the p, a, b arguments is optional, if you do not input, the default value would also be 0, 1, 2

2. Then you may see two windows which display the source image and destination image. 

  Now, you may interactively place feature lines on the source image.

  ![](images/phase1.png)

3. After placing all the feature lines, click **middle button of your mouse** on the **source image**, 
  then click the **destination image** by **left button of your mouse**. 
  Then all the corresponding feature lines would show up in the destination image.

  ![](images/phase2.png)

  [4] Then you may adjust the feature lines in both the source image and destination image to map to the corresponding positions.

  ![](images/phase3.png)

  [5] When you have done adjusting the feature lines, you may click **middle button** of your mouse on the **destination image**. 

  Then the command line would require you to input time **'t'** to do the morph, which is a float number in range of 0 to 1.
  For example, 0.5

  [6] Then the program would begin to morph. 
  When the morph is done, the command line will require you to input the output file name of the morphed image. 
  After that, you may see the morphed image in the local folder.

  ![](images/h6.jpg)

  [7] if you still want to get morphed result images with the same feature lines but different time 't', 
  you can click **middle button** of your mouse on the **destination image** to repeat the process.

## At the end

If you like this project, please star it and let me know if you want to help or contribute. Thank you.



