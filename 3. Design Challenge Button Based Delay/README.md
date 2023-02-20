
This code is used to flash a red led at a variable rate. It goes about this by counting the amount of cycles that have gone by while the button is pressed. 
This code goes about this by utilizing interrupts and timer b.
First the code runs the initializaton routines that set the inital values of the system. these include seting P4.1 and P2.3 to both be considered pull up inputs that detect when there is a swich from low to high. It also sets p1.0 and p6.6 to be outputs. The program also creates 3 variables each an unsigned 16 bit int type, these 3 are speed, timepress, and count 

The next thing the code does is initializes the timer. It sets it so that the interrupt is enabled, the inital value of tb1ccr0 to be the value held in speed. The clock is then set to use ACLK in continous mode with a devider value of 4.

Once the initializaton is complete the program begins to run first the program checks to see if timepress is equal to 0. if this is the case the function sets a timpress to be a value of 1000.
The next thing the program does is start a case statement. in state 0 nothing happens. it will stay like this until a button is pressed. If P4.1 is pressed than timepress will be set to 1000 and it will return to the loop. If 2.3 is pressed a timer will start, counting how long the button is held down for. It also switched the interupt to now check for the button to be unpressed. Once this is detected the program replaces the value held in timepress with this value. The program then resets the count and watis for a button to be pressed again. 