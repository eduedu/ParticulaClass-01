//copia 04-particulaClass01

#include "ofApp.h"

const int N = 256;		//Number of bands in spectrum
float spectrum[N];	//Smoothed spectrum values
float Rad = 500;		//Cloud raduis parameter
float Vel = 0.1;		//Cloud points velocity parameter
int bandRad = 2;		//Band index in spectrum, affecting Rad value
int bandVel = 100;		//Band index in spectrum, affecting Vel value

const int n = 150;		//Number of cloud points	

						//Offsets for Perlin noise calculation for points
float tx[n], ty[n];
float rx[n], ry[n];
float tmprx[n], tmpry[n];

ofPoint p[n];			//Cloud's points positions

float time0 = 0;		//Time value, used for dt computing
						//float tiempoVibracion = 150;
float mMilis = 0;

int tmpGrupo1;
int tmpGrupo2;
bool mostrarGui = true;

//OSC
int mValorOSC1;

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetFrameRate(30);
	//Set up sound sample
	//sound.loadSound("surface.wav");
	//sound.setLoop(true);
	//sound.play();

	//Set spectrum values to 0
	for (int i = 0; i < N; i++) {
		spectrum[i] = 0.0f;
	}

	//Initialize points offsets by random numbers
	/*
	for (int j = 0; j<n; j++) {
	tx[j] = ofRandom(0, 1000);
	ty[j] = ofRandom(0, 1000);
	}
	*/


	/*
	int separacion = 25;
	int cantidadX = 30;
	int cantidadY = 10;
	int tmpX = 1;
	int tmpY = 1;
	for (int j = 0; j < n; j++) {
	tx[j] = (tmpX*separacion);
	ty[j] = (tmpY*separacion);
	if (tmpX < cantidadX) {
	tmpX++;
	}
	else {
	tmpX = 0;
	tmpY++;
	}
	}
	*/
	//IPV///////////////////////////////////////////////////////////////////////////////////////
	/*
	int deltaX = 500;
	int deltaY = 100;
	int separacionX = 20;
	int separacionY = 20;
	int cantidadX = 8;
	int cantidadY = 8;
	int tmpX = 0;
	int tmpY = 0;
	tmpGrupo1 = (cantidadX*cantidadY);
	for (int j = 0; j < tmpGrupo1; j++) {
		if (tmpX < cantidadX) {
			tmpX++;
		}

		tx[j] = deltaX + separacionX + ((tmpX)*separacionX);
		ty[j] = deltaY + separacionY + ((tmpY + tmpX)*separacionY);
		if (tmpX >= cantidadX) {
			tmpX = 0;
			tmpY++;
		}
	}


	cantidadX = 8;
	cantidadY = 8;
	tmpX = 0;
	tmpY = 0;
	deltaX = 310;
	deltaY = deltaY + separacionY;
	tmpGrupo2 = (cantidadX*cantidadY);
	for (int j = tmpGrupo1; j < (tmpGrupo1 + tmpGrupo2); j++) {
		if (tmpX < cantidadX) {
			tmpX++;
		}

		tx[j] = deltaX + separacionX + ((tmpX)*separacionX);
		//ty[j] = deltaY + separacionY + ((tmpY + tmpX)*separacionY);
		ty[j] = deltaY + separacionY + ((tmpY + (cantidadX - tmpX))*separacionY);
		if (tmpX >= cantidadX) {
			tmpX = 0;
			tmpY++;
		}
	}
	*/
	//LUMINICA///////////////////////////////////////////////////////////////////////////////////////

	int deltaX = 400;
	int deltaY = 120;
	int separacionX = 20;
	int separacionY = 20;
	int cantidadX = 5;
	int cantidadY = 10;
	int tmpX = 0;
	int tmpY = 0;
	tmpGrupo1 = (cantidadX*cantidadY);
	for (int j = 0; j < tmpGrupo1; j++) {
		if (tmpX < cantidadX) {
			tmpX++;
		}

		//tx[j] = deltaX + separacionX + ((tmpX)*separacionX);
		tx[j] = deltaX + separacionX + ((tmpX)*separacionX);
		ty[j] = deltaY + separacionY + ((tmpY + tmpX*0)*separacionY);
		if (tmpX >= cantidadX) {
			tmpX = 0;
			tmpY++;
		}
	}


	cantidadX = 11;
	cantidadY = 4;
	tmpX = 0;
	tmpY = 0;
	deltaX = 400;
	deltaY = 300;// deltaY + separacionY;
	tmpGrupo2 = (cantidadX*cantidadY);
	for (int j = tmpGrupo1; j < (tmpGrupo1 + tmpGrupo2); j++) {
		if (tmpX < cantidadX) {
			tmpX++;
		}

		tx[j] = deltaX + separacionX + ((tmpX)*separacionX);
		//ty[j] = deltaY + separacionY + ((tmpY + tmpX)*separacionY);
		ty[j] = deltaY + separacionY + ((tmpY + (cantidadX - tmpX)*0)*separacionY);
		if (tmpX >= cantidadX) {
			tmpX = 0;
			tmpY++;
		}
	}


	//OSC
	receiver.setup(PORT);
	current_msg_string = 0;

	//GUI
	gui.setup();
	gui.add(globalX.setup("globalX", 0, -500, 500));
	gui.add(globalY.setup("globalY", 0, -500, 500));
	gui.add(globalW.setup("globalW", 1, 0, 2));
	gui.add(globalH.setup("globalH", 1, 0, 2));
	gui.add(tiempoVibracion.setup("tiempoVibracion", 150, 0, 300));
	gui.add(vibracion.setup("Vibracion", 0, 0, 2));
	gui.add(cercania.setup("Cercania", 0, 0, 100));
	gui.add(usarDatosExternos.setup("usarDatosExternos", false));
	gui.loadFromFile("settings.xml");

}


//--------------------------------------------------------------
void ofApp::update() {
	//OSC
	// hide old messages
	for (int i = 0; i < NUM_MSG_STRINGS; i++) {
		if (timers[i] < ofGetElapsedTimef()) {
			msg_strings[i] = "";
		}
	}
	// check for waiting messages
	while (receiver.hasWaitingMessages()) {
		// get the next message
		ofxOscMessage m;
		receiver.getNextMessage(m);

		// unrecognized message: display on the bottom of the screen
		string msg_string;
		//msg_string = m.getAddress();
		//msg_string += ": ";
		msg_string = "";
		for (int i = 0; i < m.getNumArgs(); i++) {
			// get the argument type
			//msg_string += m.getArgTypeName(i);
			//msg_string += ":";
			// display the argument - make sure we get the right type
			if (m.getArgType(i) == OFXOSC_TYPE_INT32) {
				msg_string += ofToString(m.getArgAsInt32(i));
			}
			else if (m.getArgType(i) == OFXOSC_TYPE_FLOAT) {
				msg_string += ofToString(m.getArgAsFloat(i));
			}
			else if (m.getArgType(i) == OFXOSC_TYPE_STRING) {
				msg_string += m.getArgAsString(i);
			}
			else {
				msg_string += "unknown";
			}
		}
		mValorOSC1 = ofToInt(msg_string);

		// add to the list of strings to display
		msg_strings[current_msg_string] = msg_string;
		timers[current_msg_string] = ofGetElapsedTimef() + 5.0f;
		current_msg_string = (current_msg_string + 1) % NUM_MSG_STRINGS;
		// clear the next line
		msg_strings[current_msg_string] = "";
	}


	//Update sound engine
	//ofSoundUpdate();

	//Get current spectrum with N bands
	float *val = ofSoundGetSpectrum(N);
	//We should not release memory of val,
	//because it is managed by sound engine

	//Update our smoothed spectrum,
	//by slowly decreasing its values and getting maximum with val
	//So we will have slowly falling peaks in spectrum
	for (int i = 0; i < N; i++) {
		spectrum[i] *= 0.97;	//Slow decreasing
		spectrum[i] = max(spectrum[i], val[i]);
	}

	//Update particles using spectrum values

	//Computing dt as a time between the last
	//and the current calling of update() 	
	float time = ofGetElapsedTimef();
	float dt = time - time0;
	dt = ofClamp(dt, 0.0, 0.1);
	//dt = 0.1;
	time0 = time; //Store the current time	

				  //Update Rad and Vel from spectrum
				  //Note, the parameters in ofMap's were tuned for best result
				  //just for current music track
	Rad = ofMap(spectrum[bandRad], 1, 3, 400, 800, true);
	Vel = ofMap(spectrum[bandVel], 0, 0.1, 0.05, 0.5);

	//Update particles positions
	Rad = 600;
	//Vel = 0.5;
	//int dt2 = 2;

	if ((mMilis + tiempoVibracion) < ofGetElapsedTimeMillis()) {
		mMilis = ofGetElapsedTimeMillis();


		for (int j = 0; j < n; j++) {
			float tmpRandomSigno1 = ofRandom(0, 10);
			float tmpRandomSigno2 = ofRandom(0, 10);
			float tmpSigno1 = 1;
			if (tmpRandomSigno1 > 4) {
				tmpSigno1 = -1;
			}
			float tmpSigno2 = 1;
			if (tmpRandomSigno2 > 4) {
				tmpSigno2 = -1;
			}
			rx[j] = ofRandom(0, 10);
			ry[j] = ofRandom(0, 10);
			tmprx[j] = rx[j] * tmpSigno1;
			tmpry[j] = ry[j] * tmpSigno2;
		}
	}
	//else {
	for (int j = 0; j < n; j++) {
		//int tmpx = tx[j] + ofRandom(0, 10);
		//int tmpy = ty[j] + ofRandom(0, 10);
		float tmpRandom = ofRandom(0, 10);

		if (tmpRandom > 6) {
			tmprx[j] = tmprx[j] * vibracion;
			tmpry[j] = tmpry[j] * vibracion;
			p[j].x = tx[j] + tmprx[j];
			p[j].y = ty[j] + tmpry[j];
		}
	}
	//}

}

//--------------------------------------------------------------
void ofApp::draw() {
	//ofBackground(255, 255, 255);	//Set up the background
	ofBackground(0);
	/*
	//Draw background rect for spectrum
	ofSetColor(230, 230, 230);
	ofFill();
	ofRect(10, 700, N * 6, -100);

	//Draw spectrum
	ofSetColor(0, 0, 0);
	for (int i = 0; i < N; i++) {
	//Draw bandRad and bandVel by black color,
	//and other by gray color
	if (i == bandRad || i == bandVel) {
	ofSetColor(0, 0, 0); //Black color
	}
	else {
	ofSetColor(128, 128, 128); //Gray color
	}
	ofRect(10 + i * 5, 700, 3, -spectrum[i] * 100);
	}
	*/
	//Draw cloud

	//Move center of coordinate system to the screen center
	ofPushMatrix();
	//ofTranslate(ofGetWidth() / 2, ofGetHeight() / 2);

	ofPushMatrix();
	ofScale(globalW, globalH, 1);
	ofTranslate(globalX, globalY);

	//Draw points
	ofSetColor(0, 100, 150);
	ofFill();
	for (int i = 0; i < n; i++) {
		if (i < 64) {
			ofSetColor(58, 160, 62);
		}
		else {
			ofSetColor(43, 153, 211);
		}

		ofCircle(p[i], 1);
	}

	//Draw lines between near points
	//float dist = 40;	//Threshold parameter of distance
	float dist = 0;
	if (!usarDatosExternos) {
		dist = cercania;
	}
	else {
		dist = ofMap(mValorOSC1, 0, 300, 0, 60);
		dist = mValorOSC1;
	}
	//GRUPOS SEPARADOS
	/*
	for (int j = 0; j < tmpGrupo1; j++) {
		for (int k = j + 1; k < tmpGrupo1; k++) {
			if (ofDist(p[j].x, p[j].y, p[k].x, p[k].y)
				< dist) {
				//ofSetColor(58, 160, 62);
				//ofSetColor(0, 200, 62);
				ofSetColor(0, 255, 0);
				ofLine(p[j], p[k]);
			}
		}
	}
	for (int j = tmpGrupo1; j < (tmpGrupo1 + tmpGrupo2); j++) {
		for (int k = j + 1; k < (tmpGrupo1 + tmpGrupo2); k++) {
			if (ofDist(p[j].x, p[j].y, p[k].x, p[k].y)
				< dist) {
				//ofSetColor(43, 153, 211);
				ofSetColor(0, 255, 0);
				//ofSetColor(200, 53, 100);
				ofLine(p[j], p[k]);
			}
		}
	}
	*/
	//LOS 2 GRUPOS JUNTOS:
	for (int j = 0; j < tmpGrupo1+tmpGrupo2; j++) {
		for (int k = j + 1; k < tmpGrupo1 + tmpGrupo2; k++) {
			if (ofDist(p[j].x, p[j].y, p[k].x, p[k].y)
				< dist) {
				//ofSetColor(58, 160, 62);
				//ofSetColor(0, 200, 62);
				ofSetColor(0, 255, 0);
				ofLine(p[j], p[k]);
			}
		}
	}
	ofPopMatrix();
	//Restore coordinate system
	ofPopMatrix();
	if (mostrarGui) {
		gui.draw();
		//OSC
		ofSetColor(0, 255, 0);
		for (int i = 0; i < NUM_MSG_STRINGS; i++) {
			ofDrawBitmapString(msg_strings[i], 10, 40 + 15 * i);
		}
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'f') ofToggleFullscreen();
	if (key == 'o') {
		if (mostrarGui) {
			mostrarGui = false;
		}
		else {
			mostrarGui = true;
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}
void ofApp::exit() {
	gui.saveToFile("settings.xml");
}