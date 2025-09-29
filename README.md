Creating an STM32 Project with PlatformIO

1. Select the "PIO Home" tab
2. Click on "New Project"
3. Insert project name
4. As "Board" select "ST NUCLEO F401RE"
5. As "Framework" select CMSIS
6. After project creation, open the "platform.ini" file and add the following line:

	lib_deps = https://github.com/UniCT-ARSLab/stm32_unict_lib.git
7. Create, in the "src" dir, a main file and add "ClockConfig()" as the first procedure call in the main() function
   (this sets the CPU clock at 84 MHz)
8. Have fun...


