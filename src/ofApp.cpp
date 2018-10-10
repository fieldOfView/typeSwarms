#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {

	//	ofEnableSmoothing();

	ofBackground(0, 0, 0);
	font.loadFont("helvetica.ttf", 300, true, true, true);

	ofSetVerticalSync(true);
	ofSetFrameRate(60);

	// instatiates the class particle and send them to their positions
	for (int i = 0; i < 400; i++) {

		particle myParticle;
		myParticle.setInitialCondition(ofRandom(0, ofGetWidth()), ofRandom(0, ofGetHeight()), 0, 0);
		particles.push_back(myParticle);

	}

	// define repel and radius
	strength = 0.5f;
	radius = 600;

	activateDraw = false;
}

//--------------------------------------------------------------
void ofApp::update() {

	for (int i = 0; i < particles.size(); i++) {
		particles[i].resetForce();
	}

	for (int i = 0; i < particles.size(); i++) {
		for (int j = 0; j < particles.size(); j++) {
			if (i != j) {
				particles[i].addForFlocking(particles[j]);
			}
		}

		particles[i].addRepulsionForce(mouseX, mouseY, 150, 0.4);
	}

	for (int i = 0; i < particles.size(); i++) {
		particles[i].addFlockingForce();
		particles[i].addDampingForce();
		particles[i].bounceOffWalls();
		particles[i].update();
	}

	writeShape();

}


void ofApp::writeShape() {

	if (!activateDraw)
		return;

	int widthStep = 0;
	int nLetters = word.length();
	if (nLetters == 0) {
		return;
	}

	// create a temporary array of stroke objects.  this would be better not made temporary...
	int resampleSize = particles.size() / (float)nLetters;
	int particleCounter = 0;

	for (int m = 0; m < nLetters; m++){
		ofTTFCharacter character;
		character = font.getCharacterAsPoints(word[m]);

		path.clear();	// clear it out
		ofRectangle characterBoundingBox;

		const vector<ofPolyline> outlines = character.getOutline();
		if (outlines.size() == 0) {
			continue;
		}
		for (int k = 0; k < 1; k++) {  // only do outside...
			if (k == 0) {
				characterBoundingBox = outlines[k].getBoundingBox();
			}
			else {
				characterBoundingBox = characterBoundingBox.getUnion(outlines[k].getBoundingBox());
			}

			vector<ofPoint> outsidePoints = outlines[k].getVertices();
			if (outsidePoints.size() == 0) {
				continue;
			}

			for(int i = 0; i < outsidePoints.size(); i++){
				path.addPoint(outsidePoints[i]);	// add into the stroke object
			}
			path.addPoint(outsidePoints[0]);	   /// add point 0 again (to close the shape)
		}

		if (path.getLength() == 0) {
			continue;
		}

		path.resample(resampleSize);  // resample so that we are now 100 points exactly....

		for (int i = 0; i < path.pts.size(); i++){
			particles[particleCounter].addAttractionForce( path.pts[i].x + ofGetWidth()/8 + widthStep ,  path.pts[i].y + ofGetHeight()/2 - characterBoundingBox.height/2 + 300 , radius, strength);

			particles[particleCounter].update();
			particleCounter ++;
		}

		widthStep += characterBoundingBox.width;

	}

}


//--------------------------------------------------------------
void ofApp::draw(){

	for (int i = 0; i < particles.size(); i++){
		particles[i].draw();
	}

}

//--------------------------------------------------------------
void ofApp::keyPressed  (int key){
	
	if(key <= 255) {
		word.push_back(char(key));
		activateDraw = true;
	}

	if(key == OF_KEY_RETURN)  {
		word = "";
		activateDraw = false;
	}
	if(key == OF_KEY_BACKSPACE)  {
		if (word.size() > 1) {
			word.erase(word.size() - 2);
		}
		else {
			word = "";
			activateDraw = false;
		}
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased  (int key){
}


//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){
	word = "";
	activateDraw = false;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(){

}
