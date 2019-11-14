#include <cubeos.h>
#include <sys_var.h>
#include <ptimer.h>

int sleep (unsigned int howlong)
{
	unsigned int seconds;
	unsigned int ticks;

	disable ();
	seconds = _time_seconds;
	ticks = _time_ticks;
	enable ();

	seconds += howlong;

	while (seconds > _time_seconds);
	while ((seconds == _time_seconds) && (ticks > _time_ticks));
	return (0);
}

int usleep (unsigned int howlong)
{
	unsigned int seconds;
	unsigned int ticks;


	disable ();
	seconds = _time_seconds;
	ticks = _time_ticks;
	enable ();
	ticks += howlong * TICKS_PER_SECOND / 1000000;
	seconds += ticks / TICKS_PER_SECOND;
	ticks = ticks % TICKS_PER_SECOND;
	while (seconds > _time_seconds);
	while ((seconds == _time_seconds) && (ticks > _time_ticks));
	return (0);

}
