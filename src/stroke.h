#ifndef _STROKE_H
#define _STROKE_H

#include "ofMain.h"


class stroke {
	
	public:
		
		void	addPoint(float x, float y);
		void	clear();
		void	draw();
		float	getLength();
		void	resample(int newNumPts);
		
		vector <ofVec2f> pts;
};

#endif
	
