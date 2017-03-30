
//the includes for the various drivers that must
//be imported for  use in this program
#include <sys/select.h>
//#include </dev/fb0>
#include "iso_font.h"
#include <linux/fb.h>
#include <sys/time.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <termios.h>
#include <sys/stat.h>
#include <sys/types.h>



//struct instances for use at later times
struct fb_fix_screeninfo finfo;
struct fb_var_screeninfo vinfo;
//struct termios termios_public;


//typedefs for datatypes
typedef unsigned short color_t;


//declaring other variables that will be needed in various functions
int fileLocationDescription;
int fb_fd;
unsigned short * screen_pointer;
unsigned long public_x_size;
unsigned long public_y_size;
unsigned long screen_size;
//prototypes for data here
int convert_color(int r, int g, int b);
void init_graphics();
void exit_graphics();
void clear_screen();
char getkey();
void sleep_ms(long ms);
void draw_pixel(int x, int y, color_t color);
void draw_line(int x1, int y1, int x2, int y2, color_t c);
void draw_text(int x, int y, const char *text, color_t c);
void draw_character(int x, int y, color_t c, int ascii);
int absolute_value(int x);

//this function initializes the graphics
//it utilizes ioctl which is short for 
//input/output control syscall
void init_graphics(){
	//open up the device file for use later on
	fb_fd = open("/dev/fb0",O_RDWR);

	//store the information about the screen in the struct
	//instances declared above by using the ioctl it gets
	//values for both variable and fixed screen info
	ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);
	ioctl(fb_fd, FBIOGET_FSCREENINFO, &finfo);

	//calculate the size of the screen in bites
	long y_virt_length = vinfo.yres_virtual;
	long x_virt_length = vinfo.xres_virtual; 
	//make the virtual lengths public
	public_x_size = x_virt_length;
	public_y_size = y_virt_length;

	//set the screen size variable to a public value
	//long temp_fixed_size = finfo.line_length;
	screen_size = finfo.line_length;
	//now map the buffer to memory
	screen_pointer = (unsigned short *) mmap(NULL, x_virt_length 
	* screen_size, PROT_READ | PROT_WRITE, MAP_SHARED,fb_fd,(off_t)0);

	//getting the program to render in color
	//vinfo.grayscale = 0;
	//vinfo.bits_per_pixel = 16;
	//ioctl(fb_fd, FBIOPUT_VSCREENINFO, &vinfo);
	//ioctl(fb_fd, FBIOGET_VSCREENINFO, &vinfo);


	//disable keypresses and echo the information to the screen

	//set the new struct termios to the public one
	struct termios new_termios;
	//new_termios = termios_public;
	ioctl(STDIN_FILENO,TCGETS, &new_termios);
	//deal with what needs to be done to the terminal
	new_termios.c_lflag &= ~ECHO;
	new_termios.c_lflag &= ~ICANON;
	ioctl(STDIN_FILENO,TCSETS,&new_termios);

//	printf("Initialized Graphics\n");

}


//the exit graphics function only utilizes the
// ioctl syscall in it's implementation
//as a result this is much simpler than initializing
void exit_graphics(){

//	printf("Exit Graphics Flag 1\n");

	//declare the struct to recieve the values
	munmap(screen_pointer, screen_size * public_y_size);
//	printf("Exit Graphics Flag 2\n");
	struct termios termios_variable;

	//use I/O control to deal with the stdinput
	ioctl(STDIN_FILENO, TCGETS, &termios_variable);
//	printf("Exit Graphics Flag 3\n");

	//by using the bitwise or we can set the flags
	//to exit the graphics
	termios_variable.c_lflag |= ICANON;
	termios_variable.c_lflag |= ECHO;
//	printf("Exit Graphics Flag 4\n");

	//now use the I/O control to set things back
	ioctl(STDIN_FILENO, TCSETS, &termios_variable);
//	printf("Exit Graphics Flag 5\n");

	//manage the memory by closing the file
	close(fb_fd);
//	printf("Exit Graphics Flag 6\n");


}


//for this I only need to write the thing to clear the 
//screen to do so what I must do is write the string "\033{2J"
//which lets the system know that  it is clearing itself
void clear_screen(){
	write(1, "\033[2J",8);

}

//the get key function is used to get input from the user
//in doing so the system needs to utilise the non blocking
//system call select()

//this fuction used information from 
//www.tutorialspoint.com/unix_system_calls/_newselect.htm
char getkey(){
//	printf("Inside Get Key\n");
	//character that the user pressed
	char key;
	//retrieval value
	fd_set rdfs;
	
	//get the values from the standard input
	FD_ZERO(&rdfs);
	FD_SET(0, &rdfs);

	//ready the struct for delay usage
	struct timeval delay;

	//delay for a little bit
	delay.tv_sec = 6;
	delay.tv_usec = 0;

	//now perform the selection
	int selector = select(STDIN_FILENO+1, &rdfs, NULL, NULL, 
	&delay);

	//verify that we got the key
	if(selector > 0){
		//read the key in
		read(0,&key,sizeof(key));
	}


	return key;
}

//sleep timer function here
//this  function will have it's purpose soon
void sleep_ms(long ms){

	struct timespec time_val;
	time_val.tv_sec = 0;
	time_val.tv_nsec = ms * 1000000;
	nanosleep(&time_val,NULL);

}

//this function draws a pixel based on what 
//parameters are recieved from the programmer
//numerically thus it takes the numbers and 
//converts them to colors
void draw_pixel(int x, int y, color_t color){
	
//	printf("Drawing a pixel\n");
	//drawing a pixel on the screen
	//declare variables that may be used
	unsigned long vertical_screen_size;
	unsigned long horizontal_screen_size;


	//check the boundries of the pixel drawer
	if(x < 0 || x >= public_x_size || y < 0 || y >= public_y_size){
		return;

	}


	vertical_screen_size = (screen_size/2) * y;
	horizontal_screen_size = x;
	unsigned short *pointer_to_screen = (screen_pointer + 
	vertical_screen_size + 
	horizontal_screen_size);


	//now set the pointer to the passed in color
	*pointer_to_screen = color;

}
//drawing a line with the bresenhams algorithm
//implemented in C based off of pseudo code from
//wikipedia
void draw_line(int x1, int y1, int x2, int y2, color_t c){
//reassigning the values
int x0 = x1;
int y0 = y1;
y1 = y2;
x1 = x2;


//this code is from rosetta code	
	int dx = absolute_value(x1-x0), sx = x0<x1 ? 1 : -1;
	int dy = absolute_value(y1-y0), sy = y0<y1 ? 1 : -1;
	int err = (dx>dy ? dx : -dy)/2, e2;
	
	for(;;){
		draw_pixel(x0,y0,c);
		if(x0 == x1 && y0 == y1) break;
		e2 = err;
		if(e2 > -dx) {err -= dy; x0 += sx;}
		if(e2 < dy)  {err += dx; y0 += sy;}
	}

}

//a quick absolute value function to  be used for the function above
int absolute_value(int x){
	if(x > 0){
		return x;
	}
	if(x < 0){
		return x * -1;

	}

return 0;

}




//draw text
//I believe this function will utilize some form of
//bitmap however I am quite uncertain as of now
//this will likely be the last function that I implement
void draw_text(int x, int y, const char *text, color_t c){
	const char *pointer;
	int offset = 0;
	for (pointer = text; *pointer != '\0'; pointer++){
		draw_character(x,y+offset,c,*pointer);
		offset = offset + 8;
	}
}

//draw the individual characters of the text string
void draw_character(int x, int y, color_t c, int ascii){
	int i, j, k;
	//itterate through the array
	for(i = 0; i < 16; i++){
		for(j = 0; j < 16; j++){
			//check the value against the ISO_FONT array
			k = ((iso_font[ascii*16+i] & 1 << j) >> j);
			if(k == 1){
				//draw the pixels at the valid locations
				draw_pixel(y+j, x+i, c);

			}
		}
	}
}

//additional shapes that need to be drawn are done here






//this function is to encode the 3 rgb values given into
//the function and produces a 16 bit number from it
//in doing so we have the required rgb value
int convert_color(int r, int g, int b){



}

