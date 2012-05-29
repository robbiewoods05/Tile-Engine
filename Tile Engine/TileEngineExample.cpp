//(c) Zaharia Radu
// Tilemap Editor

#include <allegro5.h>   
#include <fstream> // wee need the filestream to save and load maps

using namespace std;

const int SCR_W = 640; //screen width
const int SCR_H = 480; //screen height
BITMAP *buffer; // the buffer we draw to
BITMAP *map;    // we draw the map separately in another bitmap, only when we need to
int MapArray[15][20]; //the 2d array that holds the map values
void draw(); //the drawing function
void drawmap(); //the mapdrawing function
void edit(); //the map editing function
void load(); //the load function
void save(); //the save function
bool mapredraw=false; //do we need to redraw the map?
bool help=false;      //display help?
bool wallmode = false;// put walls
bool floormode = false; // put floors
// you can add other bools for other tilemodes

volatile int count=0 //timer counter,we use this to regulate fps
int other_count=0; // related

 void timer_handle() //timer fucntion
 {
         count++; //Make it short and sweet, just increment a variable
 }END_OF_FUNCTION(timer_handle); //End of Function

int main()
{
    allegro_init(); //initialize allegro
    install_keyboard(); // we're going to use the keyboard
    install_mouse(); //and the mouse
    install_timer(); // and the timer
         LOCK_VARIABLE(counter); //we lock the variable counter, so it doesn't get modified when it shouldn't
         LOCK_FUNCTION(timer_handle);
         install_int_ex(timer_handle, BPS_TO_TIMER(30)); //make the function be called 30 times per second
    set_gfx_mode( GFX_AUTODETECT_WINDOWED, SCR_W, SCR_H, 0, 0);
    buffer = create_bitmap(SCR_W,SCR_H); //create bitmaps for map and buffer
    map = create_bitmap(SCR_W,SCR_H);

    for(int i=0;i<15;i++)           // Fill the map array with 0
        for(int j=0;j<20;j++)
        {
            MapArray[i][j]=0;
        }

    drawmap(); //draw the map once

    while(!key[KEY_ESC]) //run game code
    {    
        while(other_count < count) //run logic separate from drawing, so that the game runs at the same speed regardless of machine
        {                 
            //Logic, to be updated until we catch up to the timer, update drawing buffers, etc.
        other_count++;
        edit();  //the edit function is called
        if(key[KEY_F1] && !help) //if you press F1 and help is not on
            help=true;            // toggle help on
        else if(key[KEY_F1] && help)
            help=false;            //toggle help off
        if(key[KEY_W] && !wallmode) //toggle wallplacement
        {
            wallmode=true;
            floormode=false;
        }
        else if(key[KEY_W] && wallmode)
            wallmode=false;
        if(key[KEY_F] && !floormode) //toggle floorplacement
        {
            floormode=true;
            wallmode=false;
        }
        else if(key[KEY_F] && floormode)
            floormode=false;
        // add other toggles for other tiles modes

        if(key[KEY_S])
            save(); //call save function
        if(key[KEY_L])
            load();    //call load function
        }    

        if(mapredraw) //do we need to redraw the map?
        {
            drawmap(); //yes
            mapredraw=false; //make sure we don't redraw it again without reason
        }
        draw();     //do the normal drawing
    }

    return 0;

}   
END_OF_MAIN(); 

void draw() //the draw function
{
    acquire_screen(); //acquire the screen to draw on
    clear_to_color(buffer,makecol(0,0,0)); //clear the buffer
    draw_sprite(buffer,map,0,0); //draw the map bitmap on the buffer
 /*   if(help) //do we need to display help?
    {
        textout(buffer,font,"Press S to save the map",50,50,makecol(0,0,255));
        textout(buffer,font,"Press L to load a map ",50,100,makecol(0,0,255));
        textout(buffer,font,"Press W to place a Wall",50,150,makecol(0,0,255));
        textout(buffer,font,"Press F to place a Floor",50,200,makecol(0,0,255));
    }*/
    if(wallmode) // Make the cursor look like a wallblock
    {
        rectfill(buffer,mouse_x-16,mouse_y-16,mouse_x+16,mouse_y+16,makecol(100,100,100));
        rect(buffer,mouse_x-16,mouse_y-16,mouse_x+16,mouse_y+16,makecol(255,0,0));
    }
    else if(floormode) //Make the cursor look like a floorblock
    {
        rectfill(buffer,mouse_x-16,mouse_y-16,mouse_x+16,mouse_y+16,makecol(255,255,255));
        rect(buffer,mouse_x-16,mouse_y-16,mouse_x+16,mouse_y+16,makecol(255,0,0));
    }
    //add other modes for the cursor
    else  //otherwhise make it look like a circle
    {
        circlefill(buffer,mouse_x,mouse_y,4,makecol(255,0,0));
    }
    draw_sprite(screen,buffer,0,0); //draw the buffer bitmap to the screen   
    release_screen(); //release the screen
}

void drawmap() //the map drawing function
{
    acquire_bitmap(map); //we acquire the map bitmap to draw on
    for(int i=0;i<15;i++)  //we go trough the map array and draw tiles as they are requires
        for(int j=0;j<20;j++)
        {
         if(MapArray[i][j]==0) //empty/floor
            rectfill(map,j*32,i*32,j*32+32,i*32+32,makecol(255,255,255)); //draw a white rectangle
         //code might look a bit confusing but it's some basic logic + math
         if(MapArray[i][j]==1) //wall
             rectfill(map,j*32,i*32,j*32+32,i*32+32,makecol(100,100,100));//draw a gray rectangle

         //add other tiles to be drawn here
        }
    release_bitmap(map);
}

void edit()//the edit function
{
        if(mouse_b & 1)    // is the mouse button 1 being pressed?
        {
        if(wallmode) // are we in wall placement mode?
        {
            MapArray[mouse_y/32][mouse_x/32]=1; //modify the array at the mouse coordonate to the required block
            // by doing mouse_y/32 we divide the mouse's Y(vertical position) by 32(the size of a tile) to see on which tile the mouse is on
            // same for horizontal tiles
        }
        else if(floormode)
        {
            MapArray[mouse_y/32][mouse_x/32]=0;
        }
        mapredraw=true; // tell the game it needs to redraw the map
        }
        //add other tile modes

}

void save() //the save function
{
    ofstream mapout; //create a output stream to save the map
    mapout.open("map01.map"); //we save it as map01.map, later we might add a map naming dialog
                              //maybe even a HUD, get wild
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<20;j++)
        {
            mapout << MapArray[i][j] <<" "; // output each maptile followed by a space so it can be easily read later
        }
        mapout << endl; //go to the next line as for the next line of array
    }
    mapout.close(); //close the file
}
void load() // the load function
{
    ifstream mapin; //input file stream
    mapin.open("map01.map"); //we open the map file, again a dialog might be added later
    for(int i=0;i<15;i++)
    {
        for(int j=0;j<20;j++)
        {
            mapin >>  MapArray[i][j]; //read each map tile

        }
    }
    mapredraw=true; //redraw the map
    mapin.close(); //close the file
}