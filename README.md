STM32 VGA Snake Game
===
## Video
[Bilibili](https://www.bilibili.com/video/av16265480/)  
[Youtube](https://www.youtube.com/watch?v=F2g_jxPhLt0)

## Descriptions
Creating a snake game with STM32, and showing it on the screen through VGA. It's quite a simple project that we can finish it in no time. It's also easy to test. All we need are just a screen, some DuPont lines and focus.  

VGA settins: 800 x 600 @ 56Hz  
\(Real pixel value: 400 x 200\)

## Material
STM32F103C8  
VGA line  
Potentiometer  
PS2 controller

## Pin Connections
|||
| :---: | :---: |
| PA1   | Vsync |
| PA7   | Green |
| PA8   | Hsync |
| PB4   | DATA  | 
| PB5   | CMD   |
| PB6   | CS    | 
| PB7   | CLK   |

## Part of The Code Explaintion
The part of VGA is copied from [here](http://www.artekit.eu/vga-output-using-a-36-pin-stm32/). You can click the link and see more details.  

snake.c is the main part of the project. It contains the game's core code. Since it is easy to understand and write, I did not add to much notations in it. The body of the snake is stored by a list. By the way, the snake's head and tail are opposite to the head and tail of the list. Try not to be confused.  

adcrand.c uses adc to generate random number. It's more stochastic than using timer.
