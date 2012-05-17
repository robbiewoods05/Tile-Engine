#ifndef _TILEENGINE_H
#define  _TILEENGINE_H
#include <allegro.h>
#include <allegro_native_dialog.h>

const int WIDTH = 640, HEIGHT = 480; 
 

class Engine
{

 private:  

    bool init();                  
    void loop();              
    void renderFrame();  
    void processInput();  
    void update();
	void isFinished();

 public:  

     Engine();  
    ~Engine();  

    void start();                 
};  

#endif
