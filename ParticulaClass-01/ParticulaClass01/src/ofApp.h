//copia 04-particulaClass01
#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "ofxOsc.h"

// listen on port 12345
#define PORT 12345
#define NUM_MSG_STRINGS 20

/*
This example draws points cloud and plays music track.
Also it analyzes music spectrum and use this data for controlling
the radius and shuffle of the cloud.
It's the example 06-DancingCloud from the book
"Mastering openFrameworks: Creative Coding Demystified",
Chapter 6 - Working with Sounds
Music track "surface.wav" by Ilya Orange (soundcloud.com/ilyaorange)
*/

class ofApp : public ofBaseApp {
public:
	void setup();
	void update();
	void draw();
	void mousePressed(int x, int y, int button);

	ofSoundPlayer sound;	//Sound sample

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);

	ofxPanel gui;
	ofxFloatSlider cercania;
	ofxToggle usarDatosExternos;

	ofxFloatSlider globalX;
	ofxFloatSlider globalY;
	ofxFloatSlider globalW;
	ofxFloatSlider globalH;
	ofxFloatSlider tiempoVibracion;
	ofxFloatSlider vibracion;

	void exit();

	//OSC
	ofxOscReceiver receiver;
	int current_msg_string;
	string msg_strings[NUM_MSG_STRINGS];
	float timers[NUM_MSG_STRINGS];
};
