#include "appController.h"



// --------------------------------------------------------------------------------------
void main(void)
{

	//	Application starting point --> initializes the app controller
	appController_initialize();

	// Let the operating system take over :)
	vTaskStartScheduler();
}
