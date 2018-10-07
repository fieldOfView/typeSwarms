#include "ofMain.h"
#include "particle.h"
#include "stroke.h"

class ofApp : public ofBaseApp{

public:

	void setup();
	void update();
	void draw();

	void keyPressed  (int key);
	void keyReleased (int key);

	void writeShape();

	void mouseMoved(int x, int y );
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased();

	bool activateDraw;

	stroke	path;

	vector <particle> particles;
	string word;

	float	strength;

	ofTrueTypeFont font;
};

