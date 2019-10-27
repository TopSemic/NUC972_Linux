#include <stdio.h>
#include <stdlib.h>

// Operation of LED D2
#define EXPORT_GPIO  "echo  37 > /sys/class/gpio/export"
#define GPIO_OUTPUT  "echo  out > /sys/class/gpio/gpio37/direction"
#define TURNON_LED  "echo  0 > /sys/class/gpio/gpio37/value"
#define TURNOFF_LED "echo  1 > /sys/class/gpio/gpio37/value"

void InitLED()
{
	system(EXPORT_GPIO);
	system(GPIO_OUTPUT);
}

void TurnOnLED()
{
	system(TURNON_LED);
}

void TurnOffLED()
{
	system(TURNOFF_LED);
}


int main(void)
{
    printf("Content-type:text/html\n\n");
    printf("<html>\n");
	
    InitLED();
	TurnOnLED();
	
	printf("</body>\n");
	printf("</html>\n");
	return 0;
}
