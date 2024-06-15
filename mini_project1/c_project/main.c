#include <stdio.h>
#define Eng_temp_controller 1


enum status{
	OFF , ON
};

enum status AC =OFF;
enum status Engine = OFF;
#if Eng_temp_controller ==1
enum status engine_temp_controller  = OFF;
#endif
char *states[] = {"OFF","ON"};


void display_start_menu();
void display_sensor_menu();
void set_vehicle_speed(int *speed , char z);
void control_AC(float *room_temeprature );
void control_engine_temperature(float *engine_temperature);
void check_speed(int speed , float *room_temperature , float *engine_temperature);
void display_states(int speed ,float room_temperature,float engine_temperature);

int main(void){
	/* gives inital_values for the vehicle */
	int vehicle_speed =50;
	float room_temperature =30;
	float engine_temperature =90;
	char traffic_light ;



	char input ;
	while(1){
		display_start_menu();
		fflush(stdout);
		scanf(" %c",&input);
		if (input=='a'){
			while (1){
				Engine=ON;
				display_sensor_menu();
				fflush(stdout);
				scanf(" %c",&input);
				if (input=='a'){
					Engine = OFF;
					break;

				}
				else if (input=='b'){
					printf("please enter the traffic light color{R,G,O} \n ");
					fflush(stdout);
					scanf(" %c",&traffic_light);
					set_vehicle_speed(&vehicle_speed ,traffic_light);

				}
				else if (input =='c'){
					printf("please enter the room temperature \n ");
					fflush(stdout);
					scanf(" %f",&room_temperature);
					control_AC(&room_temperature);

				}
				else if (input=='d'){
					printf("please enter the engine temperature \n ");
					fflush(stdout);
					scanf(" %f",&engine_temperature);
#if Eng_temp_controller==1
					control_engine_temperature(&engine_temperature);
#endif

				}
				check_speed(vehicle_speed,&room_temperature,&engine_temperature);
				display_states(vehicle_speed,room_temperature,engine_temperature);
				printf("\n");




			}

		}
		else if (input=='b'){
			printf("Turn OFF vehicle engine \n");
			Engine = OFF;




		}
		else if(input=='c'){
			printf("Quit the system ");
			break;


		}
	}


}

void display_start_menu(){
	/*diplays the start menu to the user */
	printf( "a. Turn on the vehicle engine \n");
	printf(	"b. Turn off the vehicle engine \n");
	printf(	"c. quit the program \n");
}
void display_sensor_menu(){
	/*displays the menu of all the sensors9 */
	printf( "a. Turn off engine \n");
	printf(	"b. Set the traffic light color \n");
	printf(	"c. Set the room temperature \n");
	printf(	"d. Set the engine temperature \n");


}
void set_vehicle_speed(int *speed,char z){

	switch(z){
	case ('G'):
								*speed = 100;
	break;
	case ('R'):
								*speed = 0;
	break;
	case ('O'):
								*speed =30;
	break;
	}



}
void control_AC(float *room_temperature){

	if (*room_temperature <10 || *room_temperature >30){
		AC =ON;
		*room_temperature = 20;
	}
	else  {
		AC= OFF;

	}



}
void control_engine_temperature(float *engine_temperature){
#if Eng_temp_controller ==1
	if(*engine_temperature <100 || *engine_temperature >150){
		engine_temp_controller = ON;
		*engine_temperature =125;
	}
	else{
		engine_temp_controller =OFF;
	}
#endif

}
void check_speed(int speed , float *room_temperature,float *engine_temperature){
	if (speed==30){
		AC =ON;
		*room_temperature =5.0/4.0 * (*room_temperature) +1;

#if Eng_temp_controller==1
		engine_temp_controller =ON;
		*engine_temperature = (5.0/4.0) * (*engine_temperature) +1;
#endif





	}
}
void display_states(int speed ,float room_temperature,float engine_temperature){
	/*function that displays the details about the vehicle */
	printf(". the engine is %s \n",states[Engine]);
	printf(". the AC is %s \n",states[AC]);
#if Eng_temp_controller ==1
	printf(". the engine temperature controller is %s \n",states[engine_temp_controller]);
#endif
	printf(". the vehicle speed is %d  \n",speed);
	printf(". the room temperature is %f \n" ,room_temperature);
	printf(". the engine temperature is %f \n",engine_temperature);


}





