#include <cubeos.h>
#include <sai.h>
#include <sai_cube.h>


#define LEFTMOTOR RoboCube.tpu[6]
#define LEFTPERIOD RoboCube.tpu[6],CTL_TPU_SETPWMPERIOD
#define RIGHTMOTOR RoboCube.tpu[7]
#define RIGHTPERIOD RoboCube.tpu[7],CTL_TPU_SETPWMPERIOD


init_icontrol ()
{

	printf (" This is init_icontrol ");

	init_sai_cube ();

	sa_control (LEFTMOTOR, CTL_TPU_SETFUNCTION, TPU_FKT_PWM);
	sa_control (RIGHTMOTOR, CTL_TPU_SETFUNCTION, TPU_FKT_PWM);

	sa_control (LEFTPERIOD, 32000);
	sa_control (RIGHTPERIOD, 32000);
