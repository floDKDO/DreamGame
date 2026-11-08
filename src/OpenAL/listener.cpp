#include "listener.h"

#include <AL/al.h>

namespace al
{

void set_listener_position(glm::vec3 position)
{
	alListener3f(AL_POSITION, position.x, position.y, position.z);
}

void set_listener_velocity(glm::vec3 velocity)
{
	alListener3f(AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

void set_listener_orientation(glm::vec3 forward, glm::vec3 up)
{
	float listener_orientation[] = {forward.x, forward.y, forward.z, up.x, up.y, up.z};
	alListenerfv(AL_ORIENTATION, listener_orientation);
}

}