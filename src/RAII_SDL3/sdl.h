#pragma once

namespace sdl
{

class SDL
{
	public:
		SDL();
		SDL(const SDL&) = delete;
		SDL(SDL&&) = delete;
		SDL& operator=(const SDL&) = delete;
		SDL& operator=(SDL&&) = delete;
		~SDL();
};

}
