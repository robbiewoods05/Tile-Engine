#include <allegro5.h>
#include <fstream> 
#include <iostream>
#include <allegro_primitives.h>


#define SCREEN_WIDTH	640
#define SCREEN_HEIGHT	480
#define FPS				60
#define MAP_HEIGHT		15
#define MAP_WIDTH		20

	
ALLEGRO_DISPLAY *display = NULL; 
ALLEGRO_EVENT_QUEUE *evq = NULL; 
ALLEGRO_TIMER *timer = NULL;
ALLEGRO_BITMAP *buffer = al_lock_bitmap(); 
ALLEGRO_BITMAP *map = NULL;
ALLEGRO_COLOR black = al_map_rgb(0, 0, 0), purple = al_map_rgb(255, 0, 255), blue = al_map_rgb(0, 0, 255), red = al_map_rgb(255, 0, 0), white = al_map_rgb(255, 255, 255), green = al_map_rgb(0, 255, 0);
ALLEGRO_EVENT ev;

bool mapRedraw = false, wallMode = false, floorMode = false, help = false, done = false; 

int otherCount = 0;
int mapArray[MAP_HEIGHT][MAP_WIDTH];



int destroy(ALLEGRO_DISPLAY *display, ALLEGRO_EVENT_QUEUE *evqueue, ALLEGRO_TIMER *timer);  

void draw(); 
void drawmap(); 
void edit(); 
void load(); 
void save();

int main(void)
{
	if (!al_init())
		return -1;

	display = al_create_display(SCREEN_WIDTH, SCREEN_HEIGHT); 

	evq = al_create_event_queue(); 
	timer = al_create_timer(1.0 / FPS); 
	al_install_keyboard();

	al_install_mouse();
	al_init_primitives_addon();

	al_register_event_source(evq, al_get_mouse_event_source());
	al_register_event_source(evq, al_get_keyboard_event_source());
	al_register_event_source(evq, al_get_timer_event_source(timer));
	al_register_event_source(evq, al_get_display_event_source(display));

	buffer = al_create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);
	map = al_create_bitmap(SCREEN_WIDTH, SCREEN_HEIGHT);

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_HEIGHT; j++)
		{
			mapArray[i][j] = 0; 
		}
	}

	drawmap();

	al_start_timer(timer); 

	while (!done)
	{
		al_wait_for_event(evq, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{
			if (mapRedraw)
			{
				drawmap();
				mapRedraw = false;
			}

			draw();
		}

		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}

		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
				case ALLEGRO_KEY_ESCAPE:
					done = true; 
					break;

				case ALLEGRO_KEY_F1:
					if (!help)
						help = true;
					else if (help)
						help = false;
					break;

				case ALLEGRO_KEY_W:
					if (!wallMode)
					{
						wallMode = true; 
						floorMode = false;
					}
					else if (wallMode)
					{
						wallMode = false;
					}
					break;

				case ALLEGRO_KEY_F:
					if (!floorMode)
					{
						floorMode = true;
						wallMode = false;
					}
					else if (floorMode)
					{
						floorMode = false;
					}
					break;

				case ALLEGRO_KEY_S:
					save();
					break;
				
				case ALLEGRO_KEY_L:
					load;
					break;
			}				
		}
	}
	
	destroy(display, evq, timer);
}


int destroy(ALLEGRO_DISPLAY *display1, ALLEGRO_EVENT_QUEUE *evqueue, ALLEGRO_TIMER *timer1)
{
	al_destroy_display(display);
	al_destroy_event_queue(evqueue);
	al_destroy_timer(timer);
	
	if (!display && !evqueue && !timer)
		return 0;
	else 
		return -1;
}

void draw()
{
	al_set_target_bitmap(buffer);
	al_clear_to_color(black);
	al_draw_bitmap(map, 1.0f, 1.0f, NULL);
	
	if(help) //do we need to display help?
    {
        std::cout << "Press S to save the map";
        std::cout << "Press L to load a map ";
		std::cout << "Press W to place a Wall";
		std::cout << "Press F to place a Floor";
    }
	else if (!help)
	{
		system("cls");
	}

	if (wallMode)
	{
		al_draw_filled_rectangle(mouse_x - 16, mouse_y - 16, mouse_x + 16, mouse_y + 16, green);
		al_draw_rectangle(mouse_x-16,mouse_y-16,mouse_x+16,mouse_y+16, red, 2);
	}

	else if (floorMode)
	{
		al_draw_filled_rectangle(mouse_x - 16, mouse_y - 16, mouse_x + 16, mouse_y + 16, white);
		al_draw_rectangle(mouse_x-16,mouse_y-16,mouse_x+16,mouse_y+16, red, 2);
	}

	else 
	{
		al_draw_filled_circle(mouse_x, mouse_y, 4, red);
	}

	al_set_target_bitmap(al_get_backbuffer(display); 
	al_draw_bitmap(buffer, 0, 0, NULL);
	al_flip_display();
}

void drawmap()
{
	al_set_target_bitmap(map);
	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			if (mapArray[i][j] == 0)
			{
				al_draw_filled_rectangle(j*32, i*32, j*32+32, i*32+32, red);
			}
			else if (mapArray[i][j])
			{
				al_draw_filled_rectangle(j*32, i*32, j*32+32, i*32+32, red);
			}
		}
	}
}

void edit()
{
	if (mouse_b & 1)
	{
		if (wallMode)
		{
			mapArray[mouse_y / 32][mouse_x / 32] = 1;
		}

		if (floorMode)
		{
			mapArray[mouse_y / 32][mouse_x / 32] = 0; 
		}
		
		mapRedraw = true;
	}
}

void save()
{
	std::ofstream mapOut; 
	mapOut.open("map1.amp");

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			mapOut << mapArray[i][j] << " ";
		}

		mapOut << std::endl;
	}

	mapOut.close();
}

void load()
{
	std::ifstream mapIn;
	mapIn.open("map1.amp");

	for (int i = 0; i < MAP_HEIGHT; i++)
	{
		for (int j = 0; j < MAP_WIDTH; j++)
		{
			mapIn >> mapArray[i][j];
		}
	}

	mapRedraw = true;

	mapIn.close();
}