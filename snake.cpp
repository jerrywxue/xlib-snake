/*
CS 349 A1 Skeleton Code - Snake

- - - - - - - - - - - - - - - - - - - - - -

Commands to compile and run:

    g++ -o snake snake.cpp -L/usr/X11R6/lib -lX11 -lstdc++
    ./snake

Note: the -L option and -lstdc++ may not be needed on some machines.
*/

#include <iostream>
#include <string>
#include <list>
#include <cstdlib>
#include <sys/time.h>
#include <math.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sstream>



/*
 * Header files for X functions
 */
#include <X11/Xlib.h>
#include <X11/Xutil.h>

using namespace std;

/*
 * Global game state variables
 */
const int Border = 1;
const int BufferSize = 10;
int FPS = 38;
int moveAmountz = 5;
const int width = 800;
const int height = 600;

/*
 * Information to draw on the window.
 */
struct XInfo {
	Display	 *display;
	int		 screen;
	Window	 window;
	GC		 gc[3];
	int		width;		// size of window
	int		height;
};


/*
 * Function to put out a message on error exits.
 */
void error( string str ) {
  cerr << str << endl;
  exit(0);
}


/*
 * An abstract class representing displayable things.
 */
class Displayable {
	public:
		virtual void paint(XInfo &xinfo) = 0;
};

class Posn{
public:
	int getX() {
		return x;
	}

	int getY() {
		return y;
	}


	Posn(int x, int y){
		this->x = x;
		this->y = y;
	}
private:
		int x;
		int y;
};

/*
 * A text displayable
 */


class Text : public Displayable {
public:
  virtual void paint(XInfo& xinfo) {
    XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0],
                      this->x, this->y, this->s.c_str(), this->s.length() );
  }

	void updatescore(int n, int hs){
		stringstream ss;
		ss << n;
		string scor = ss.str();
		stringstream ss2;
		ss2 << hs;
		string hscor = ss2.str();
		s = "score: "+ scor + " high score: " + hscor;
	}

  // constructor
  Text(int x, int y, string s): x(x), y(y), s(s)  {}

private:
  int x;
  int y;
  string s; // string to show
};

class Fruit : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, blockSize, blockSize);
        }

				int getfx() {
					return x;
				}

				int getfy() {
					return y;
				}

				void fruitrandomize(){
					x = (rand() %  80)* 10;
					y = (rand() %  58)* 10;
				}

        void move(){
          x= x + 10;
          if (x > 800 ){
            x = 0;
            y = (rand() %  58)* 10;
          }
        }

        void movedown(){
          y= y + 10;
          if (y > 570 ){
            x = (rand() %  80)* 10;
            y = 0;
          }
        }


        Fruit() {
            // ** ADD YOUR LOGIC **
            // generate the x and y value for the fruit
						x = (rand() %  80)* 10;
            y = (rand() %  58)* 10;
						blockSize = 10;
        }

        // ** ADD YOUR LOGIC **
        /*
         * The fruit needs to be re-generated at new location every time a snake eats it. See the assignment webpage for more details.
         */
    private:
        int x;
        int y;
				int blockSize;
};


class Snake : public Displayable {
	public:
		virtual void paint(XInfo &xinfo) {
			XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], x, y, blockSize, blockSize); //this displahy the snake head

			list<Posn *>::const_iterator begin = tail.begin();
			list<Posn *>::const_iterator end = tail.end();

			while( begin != end ) {
				Posn *i = *begin;
				XFillRectangle(xinfo.display, xinfo.window, xinfo.gc[0], i->getX(), i->getY(), blockSize, blockSize);
				begin++;
			}
		}

		void move(XInfo &xinfo) {
			Posn * p = new Posn(this->x, this->y);
			tail.push_front(p);
			if(newtail == 0){
				delete tail.back();
				tail.pop_back();
			} else {
				tailsize++;
				newtail--;
			}

			if (direction == 'e'){
				x = x + moveAmount;
			} else if (direction == 'n'){
				y = y - moveAmount;
			} else if (direction == 's'){
				y = y + moveAmount;
			} else if (direction == 'w'){
				x = x - moveAmount;
			}

            // ** ADD YOUR LOGIC **
            // Here, you will be performing collision detection between the snake,
            // the fruit, and the obstacles depending on what the snake lands on.
		}

		int getX() {
			return x;
		}

		int getY() {
			return y;
		}

    /*

     * ** ADD YOUR LOGIC **
     * Use these placeholder methods as guidance for implementing the snake behaviour.
     * You do not have to use these methods, feel free to implement your own.
     */
    bool didEatFruit(Fruit &fruit) {
			return (x == fruit.getfx() && y==fruit.getfy());
    }

		void addTail(){
			newtail = 5;
		}

		void restart(){
			newtail = 0;
			for (int i = 0; i < tailsize; i++){
				delete tail.back();
				tail.pop_back();
			}
			tail.clear();
			this->x=100;
			this->y=450;
			Posn * p = new Posn(this->x-10, this->y);
			direction = 'e';
			tail.push_front(p);
			newtail = 0;
			tailsize =1;
		}


    bool didHitObstacle() {
			if (x < 0 || x >= width || y < 0 || y >= height-20){
				return true;
			}
			list<Posn *>::const_iterator begin = tail.begin();
			list<Posn *>::const_iterator end = tail.end();

			while( begin != end ) {
				Posn *i = *begin;
				if (x == i->getX() && y == i->getY()){
					return true;
				}
				begin++;
			}
			return false;
    }
    void zoom(int s){
      moveAmount =s;
    }

    void turnLeft() {
			if (direction == 'n'){
				direction = 'w';
			} else if (direction == 'e'){
				direction = 'n';
			} else if (direction == 's'){
				direction = 'e';
			} else if (direction == 'w'){
				direction = 's';
			}
    }

    void turnRight() {
			if (direction == 'n'){
				direction = 'e';
			} else if (direction == 'e'){
				direction = 's';
			} else if (direction == 's'){
				direction = 'w';
			} else if (direction == 'w'){
				direction = 'n';
			}
    }

		char getDirection(){
			return direction;
		}

		Snake(int x, int y, int moveAmount): x(x), y(y), moveAmount(moveAmount) {
      blockSize = 10;
			direction = 'e';
			newtail = 0;
			Posn * p = new Posn(this->x-10, this->y);
			tail.push_front(p);
			tailsize=1;
		}

		~Snake(){
			for (int i = 0; i < tailsize; i++){
				delete tail.back();
				tail.pop_back();
			}
		}

	private:
		int x;
		int y;
		int blockSize;
		int moveAmount;
		int newtail;
		int tailsize;
		char direction;
		list<Posn *> tail;
};

list<Displayable *> dList;           // list of Displayables

Fruit fruit;
Fruit coolfruit;
Fruit funfruit;
int score = 0;
int highscore  = 0;
bool restart =false;
bool pausee = false;
bool splash =true;
bool died=false;
Text stringscore(10, 595, "score : 0 highscore: 0");


/*
 * Initialize X and create a window
 */
void initX(int argc, char *argv[], XInfo &xInfo, Snake &snake) {


	XSizeHints hints;
	unsigned long white, black;

   /*
	* Display opening uses the DISPLAY	environment variable.
	* It can go wrong if DISPLAY isn't set, or you don't have permission.
	*/

	xInfo.display = XOpenDisplay( "" ); //this opens the display
	if ( !xInfo.display )	{
		error( "Can't open display." );
	}

   /*
	* Find out some things about the display you're using.
	*/
	xInfo.screen = DefaultScreen( xInfo.display ); //this is an int

	white = XWhitePixel( xInfo.display, xInfo.screen );
	black = XBlackPixel( xInfo.display, xInfo.screen );

	hints.x = 100;
	hints.y = 100;
	hints.width = 800;
	hints.height = 600;
	hints.flags = PPosition | PSize | PMinSize | PMaxSize;
	hints.min_width = 800;
	hints.min_height = 600;
	hints.max_width = 800;
	hints.max_height = 600;

	xInfo.window = XCreateSimpleWindow(
		xInfo.display,				// display where window appears
		DefaultRootWindow( xInfo.display ), // window's parent in window tree
		hints.x, hints.y,			// upper left corner location
		hints.width, hints.height,	// size of the window
		Border,						// width of window's border
		black,						// window border colour
		white );					// window background colour

	XSetStandardProperties(
		xInfo.display,		// display containing the window
		xInfo.window,		// window whose properties are set
		"animation",		// window's title
		"Animate",			// icon's title
		None,				// pixmap for the icon
		argv, argc,			// applications command line args
		&hints );			// size hints for the window

	/*
	 * Create Graphics Contexts
	 */
	int i = 0;
	xInfo.gc[i] = XCreateGC(xInfo.display, xInfo.window, 0, 0);
	XSetForeground(xInfo.display, xInfo.gc[i], BlackPixel(xInfo.display, xInfo.screen));
	XSetBackground(xInfo.display, xInfo.gc[i], WhitePixel(xInfo.display, xInfo.screen));
	XSetFillStyle(xInfo.display, xInfo.gc[i], FillSolid);
	XSetLineAttributes(xInfo.display, xInfo.gc[i],
	                     1, LineSolid, CapButt, JoinRound);

	XSelectInput(xInfo.display, xInfo.window,
		ButtonPressMask | KeyPressMask |
		PointerMotionMask |
		EnterWindowMask | LeaveWindowMask |
		StructureNotifyMask
	);  // for resize events
	/*
	 * Put the window on the screen.
	 */

	XMapRaised( xInfo.display, xInfo.window );
	XFlush(xInfo.display);
}


void drawscore(XInfo &xinfo){
	XDrawLine(xinfo.display, xinfo.window, xinfo.gc[0], 0, height-20, width, height-20);
}

/*
 * Function to repaint a display list
 */
void repaint( XInfo &xinfo, Snake &snake) {
	list<Displayable *>::const_iterator begin = dList.begin();
	list<Displayable *>::const_iterator end = dList.end();

	//XClearWindow(xinfo.display, xinfo.window);

	// get height and width of window (might have changed since last repaint)

	XWindowAttributes windowInfo;
	XGetWindowAttributes(xinfo.display, xinfo.window, &windowInfo);
	unsigned int height = windowInfo.height;
	unsigned int width = windowInfo.width;

	// big black rectangle to clear background


	// draw display list
	XClearWindow(xinfo.display, xinfo.window);

	if (splash){
		string s="Name: Jerry William Xue";
		XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0], 100, 290, s.c_str(), s.length() );
		s="userID: mzxue";
		XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0], 100, 305, s.c_str(), s.length() );
		s="The objective of the snake is to eat the target fruit, which makes it grow in length.";
		XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0], 100, 320, s.c_str(), s.length() );
	  s="Use WASD to control the snake";
		XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0], 100, 335, s.c_str(), s.length() );
		s="Press any key to start the game";
		XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0], 100, 350, s.c_str(), s.length() );
		//sleep(1);
  }

  if (died){
    string s="You have died. Press r to restart the game. Press q to quit the game";
    XDrawImageString( xinfo.display, xinfo.window, xinfo.gc[0], 100, 290, s.c_str(), s.length() );
  }

	while( begin != end && !splash && !died) {
		Displayable *d = *begin;
		d->paint(xinfo);
		begin++;
	}

	if(!splash){
		drawscore(xinfo); //draw the line
    stringscore.paint(xinfo);
	}

	XFlush(xinfo.display);
}

void handleKeyPress(XInfo &xinfo, XEvent &event , Snake &snake) {
	KeySym key;
	char text[BufferSize];

	int i = XLookupString(
		(XKeyEvent *)&event, 	// the keyboard event
		text, 					// buffer when text will be written
		BufferSize, 			// size of the text buffer
		&key, 					// workstation-independent key symbol
		NULL );					// pointer to a composeStatus structure (unused)
	if ( i == 1) {
		printf("Got key press -- %c\n", text[0]);

		if (splash && i){
			if(text[0] == 'q'){
				exit(0);
			}
			splash= false;
		} else{
			if (text[0] == 'q') {
				exit(0);
			} else if (text[0] == 'w'){ //going north
				if (snake.getDirection() == 'w'){
					snake.turnRight();
				} else if(snake.getDirection() == 'e'){
					snake.turnLeft();
				}
			} else if (text[0] == 'd'){ //going east
				if (snake.getDirection() == 'n'){
					snake.turnRight();
				} else if(snake.getDirection() == 's'){
					snake.turnLeft();
				}
			} else if (text[0] == 's'){ //going south
				if (snake.getDirection() == 'w'){
					snake.turnLeft();
				} else if(snake.getDirection() == 'e'){
					snake.turnRight();
				}
			} else if (text[0] == 'a'){ //going west
				if (snake.getDirection() == 'n'){
					snake.turnLeft();
				} else if(snake.getDirection() == 's'){
					snake.turnRight();
				}
			} else if (text[0] == 'p'){
				pausee = !pausee;
			} else if (text[0] == 'r'){
				score =0;
				stringscore.updatescore(score,highscore);
				snake.restart();
        died=false;
			}
		}

	}
}

void handleAnimation(XInfo &xinfo, int inside , Snake &snake) {
    /*
     * ADD YOUR OWN LOGIC
     * This method handles animation for different objects on the screen and readies the next frame before the screen is re-painted.
     */
    coolfruit.move();
    funfruit.movedown();

	snake.move(xinfo); //updates snake location
	if (snake.didEatFruit(fruit) ||snake.didEatFruit(coolfruit) || snake.didEatFruit(funfruit)){

		snake.addTail();

    if(snake.didEatFruit(fruit)){
      score++;
      fruit.fruitrandomize();
    }
    if(snake.didEatFruit(coolfruit)){
      score++;
      score++;
      coolfruit.fruitrandomize();
    }

    if(snake.didEatFruit(funfruit)){
      score++;
      score++;
      funfruit.fruitrandomize();
    }

    if (score > highscore){
      highscore = score;
    }

    stringscore.updatescore(score,highscore);

	}

	if (snake.didHitObstacle()){
		score = 0;
    snake.restart();
    died=true;
	}
}

// get microseconds
unsigned long now() {
	timeval tv;
	gettimeofday(&tv, NULL);
	return tv.tv_sec * 1000000 + tv.tv_usec;
}



void eventLoop(XInfo &xinfo , Snake &snake) {
	// Add stuff to paint to the display list
	dList.push_front(&snake);
  dList.push_front(&fruit);
  dList.push_front(&coolfruit);
  dList.push_front(&funfruit);

	XEvent event;
	unsigned long lastRepaint = 0;
  unsigned long lastMove = 0;
	int inside = 0;

	while( true ) {

		if (XPending(xinfo.display) > 0) { // any events pending?
			XNextEvent(xinfo.display, &event);
			switch(event.type) {
				case KeyPress:
					handleKeyPress(xinfo, event,snake);
					break;
				case EnterNotify:
					inside = 1;
					break;
				case LeaveNotify:
					inside = 0;
					break;
			}
		}

    unsigned long end = now(); //now is the time now

    if (end - lastMove > 1000000/(moveAmountz*6)){ //1000000/FPS is a span of time
      if (!splash && !pausee && !died){
          handleAnimation(xinfo, inside,snake); //update animation objects
        }
        lastMove = now(); //remember when the paint happened

    }

    if (end - lastRepaint > 1000000/FPS){ //1000000/FPS is a span of time
	      repaint(xinfo,snake); //my repaint
	      lastRepaint = now(); //remember when the paint happened
    }

    if(XPending(xinfo.display) == 0){
      usleep(1000000 / FPS - (end - lastRepaint));
    }
	}
}


/*
 * Start executing here.
 *	 First initialize window.
 *	 Next loop responding to events.
 *	 Exit forcing window manager to clean up - cheesy, but easy.
 */
int main ( int argc, char *argv[] ) {

  stringstream ss;
  stringstream ss2;
  if(argc >= 3){
    ss << argv[1];
    ss >> FPS;
    //FPS = atoi(argv[1]);
    ss2 << argv[2];
    int sas  =0;
    ss2 >> sas;
    moveAmountz = sas;
  }

  Snake snake(100, 450, 10);
	XInfo xInfo;

	initX(argc, argv, xInfo, snake);
	eventLoop(xInfo,snake);
	XCloseDisplay(xInfo.display);
}
