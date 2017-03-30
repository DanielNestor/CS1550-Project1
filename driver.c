//This is the driver program for
//project 1, running this program 
//it will draw an image on the screen
#include "library.c"
//create your own color
#include <stdlib.h>
#include <stdio.h>

int main(){

	//initialize the graphics
	init_graphics();
//	printf("Graphics Really Initialized\n");

	//now test to see if the sleep ms works
	//	sleep_ms(1000000);
	
	//set blue color
	color_t purple_color = 0XF81F;	

	//clear out the screen
	clear_screen();


	//testing the draw line
//	draw_line(130, 200, 200, 350, blue_color);


	

	//draw a triangle on the screen
	draw_line(150, 300, 500, 100,purple_color);
	draw_line(150, 300, 30, 130,purple_color);
	draw_line(30, 130, 500, 100, purple_color);
	draw_text(150,400,"Press Any Key For More Shapes",purple_color);
	draw_text(150,100,"Triangle",purple_color);
	
	char key;	
	int exit = 0;
	scanf("%d", exit);
	//close out of the graphics
	//now create the needed delay for the program
	sleep_ms(10000);

	//clear the screen again to draw another polygon
	clear_screen();
	//use a different color for this one 
	color_t new_color = 0x7556;
	
	//now draw a square on the screen
	//top
	draw_line(200, 400, 500, 400,new_color);
	//bottom
	draw_line(200, 100, 500, 100,new_color);
	//left
	draw_line(200, 400, 200, 100,new_color);
	//right
	draw_line(500, 100, 500, 400,new_color);

	draw_text(150,200,"SQUARE",purple_color);
	
	draw_text(150,400,"Press 'q' to proceed",new_color);
//	scanf("%d", exit);
	do{
		key = getkey();
	}while(key != 'q');
	
	//clear off the screen for part 3
	clear_screen();


	//now it's time for the character typing test
	color_t last_test_color = 0x3255;
	color_t last_test_color_2 = 0x3435;
	draw_text(150,150,"Type A Character to put onscreen \';\' to exit",last_test_color);
//	exit = NULL;
	//starting point
	int new_x = 200;
	int new_y = 200;
	char string_1[2];
	const char *pointer_to_string;
	pointer_to_string = string_1;
	do{
		key = getkey();
		string_1[0] = key;
		string_1[1] = '\0';
		draw_text(new_x,new_y,pointer_to_string, last_test_color_2);
		new_x = new_x + 10;
	}while(key != ';');
	clear_screen();

	color_t square_color = 0x0453;


	
	int corner1_x = 100, corner2_x = 200, corner3_x = 100 , corner4_x = 200;
	int corner1_y = 100, corner2_y = 100, corner3_y = 200 , corner4_y = 200;
	//finally for the last loop
	do{
		//draw the lines needed
		//top
		draw_line(corner1_x,corner1_y,corner2_x,corner2_y,square_color);
		//left
		draw_line(corner1_x,corner1_y,corner3_x,corner3_y,square_color);
		//right
		draw_line(corner2_x,corner2_y,corner4_x,corner4_y,square_color);
		//bottom
		draw_line(corner3_x,corner3_y,corner4_x,corner4_y,square_color);
		draw_text(150,150,"Move the square with WASD controlls \'q\' to exit",last_test_color);
		


		key = getkey();	
		clear_screen();
		if(key == 'd'){
			corner1_x = corner1_x + 10;
			corner2_x = corner2_x + 10;
			corner3_x = corner3_x + 10;
			corner4_x = corner4_x + 10;
		}
		if(key == 'a'){
			corner1_x = corner1_x - 10;
			corner2_x = corner2_x - 10;
			corner3_x = corner3_x - 10;
			corner4_x = corner4_x - 10;

		
		}
		if(key == 's'){
			corner1_y = corner1_y + 10;
			corner2_y = corner2_y + 10;
			corner3_y = corner3_y + 10;
			corner4_y = corner4_y + 10;
		
		}
		if(key == 'w'){
			corner1_y = corner1_y - 10;
			corner2_y = corner2_y - 10;
			corner3_y = corner3_y - 10;
			corner4_y = corner4_y - 10;

		
		
		}
	
	}while(key != 'q');












	exit_graphics();
	return 0;
}
