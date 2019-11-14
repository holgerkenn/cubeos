/* Sensor Actuator Interface */
#include <sai.h>

int sa_write (sa_p device, int value)
{
	if (device->writehandler)
		return (*device->writehandler) (&device->iop, value);

	return SA_FAIL;
}

int sa_read (sa_p device)
{

	if (device->readhandler)
		return (*device->readhandler) (&device->iop);
	return SA_FAIL;
}

int sa_control (sa_p device, int control, int value)
{
	if (device->controlhandler)
		return (*device->controlhandler) (&device->iop, control, value);
	return SA_FAIL;
}
