#include <cubeos.h>
#include <global_def.h>
#include <sai.h>
#include <sai_cube.h>
#include <sai_i2c.h>
#include <sai_tpu.h>

struct sa_cube RoboCube;



int init_sai_cube ()
{
/* init functions go here */


	init_sai_tpu ();

	init_sai_i2c ();

	return 0;

}
