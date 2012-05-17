#include "TileEngine.h"

ALLEGRO_DISPLAY *display = NULL;
ALLEGRO_EVENT_QUEUE *event_queue = NULL;
ALLEGRO_EVENT evnt;

bool done = false;

Engine::Engine()
{

}

Engine::~Engine()
{

}

bool Engine::init()
{
	display = al_create_display(WIDTH, HEIGHT);

	if (!display)
	{
		return false; 
	}

	return true;
}

void Engine::renderFrame()
{

}

void Engine::processInput()
{
	event_queue = al_create_event_queue();

	al_register_event_source(event_queue, al_get_display_event_source(display));

	al_wait_for_event(event_queue, &evnt);

	if (evnt.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
	{
		done = true;
	}
}

void Engine::update()
{

}

void Engine::loop()
{
	while (!done)
	{
		processInput();
		update();
		renderFrame();
	}
}

void Engine::start()
{
	loop();
}

void Engine::isFinished()
{
	if (done == true)
	{
		al_destroy_display(display); 
		al_destroy_event_queue(event_queue);
	}
}


