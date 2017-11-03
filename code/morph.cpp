#include "morph.h"


//--------------------------------------------------------------------------
//------------------------allocPixmap---------------------------------------
//--------------------------------------------------------------------------
void allocPixmap(int w, int h, pixel ** &map){
  //clear the previous pixmap
  delete [] map;
  map = new pixel* [h];
  map[0] = new pixel[w * h];
  
  for(int i = 1; i < h; i++){  
    map[i] = map[i - 1] + w;
  }

  for(int row = 0; row < h; row++)
    for(int col = 0; col < w; col++)
      map[row][col].r = map[row][col].g = map[row][col].b = map[row][col].a = 255;
}

//--------------------------------------------------------------------------
//------------------------imgRead-------------------------------------------
//--------------------------------------------------------------------------
void imgRead(string filename, pixel ** &map, int &imgW, int &imgH){
	if(!filename.empty()){
		image.read(filename);
	}else{
		cout<<"The input file name cannot be empty"<<endl;
		exit(1);
	}

	//because of the difference to display between opengl and image RGBA info
	//we need to flip the RGBA info
	image.flip();

	//get the current image columns and rows
	imgW = image.columns();
	imgH = image.rows();

  allocPixmap(imgW, imgH, map);

	image.write(0,0,imgW,imgH,"RGBA",CharPixel,(void *)map[0]);
	cout<<"Read the image file \""<<filename<<"\" successfully ."<<endl;
}

//--------------------------------------------------------------------------
//------------------------imgWrite-------------------------------------------
//--------------------------------------------------------------------------
void imgWrite(string filename, pixel ** map, int imgW, int imgH){
	if(filename.empty()){
		cout<<"The output file name cannot be empty"<<endl;
		exit(1);
	}

	image.read(imgW, imgH, "RGBA", CharPixel, (void *)map[0]);

	image.flip();

	image.write(filename);
	cout<<"Write the image into \""<<filename<<"\" file successfully."<<endl;
}

//--------------------------------------------------------------------------
//------------------------handleKey-------------------------------------------
//--------------------------------------------------------------------------
void handleKey(unsigned char key, int x, int y)
{  
	switch(key){
	
		case 'q':	// q - quit
		case 'Q':
		case 27:	// esc - quit
		exit(0);
      
		default:	// do nothing to other key
		return;
	}
}

//--------------------------------------------------------------------------
//------------------------handleMouse-------------------------------------------
//--------------------------------------------------------------------------

//
// Routine to adjust mouse coords in case window size has changed
//
void AdjustMouse(int& x, int& y){

  x = x;

  // reverse y, so zero at bottom, and max at top
  y = imgHeightDest - y;
}


//
// Handle mouse motion with a button down
//
void handleMotion(int x, int y){
  if ( currentPhase == 0 )
    return;

  AdjustMouse(x, y);

  if(currentButton != GLUT_LEFT_BUTTON || hitpoint == NULL)
    return;

  if(hitpoint != NULL)
    hitpoint->set(x, y);
  else
    return;

  glutPostRedisplay();
}

//------------------------ Mouse handle on source image -------------------
void srcImgMouseHandle(int button, int state, int x, int y){

  AdjustMouse(x, y);

  if( currentPhase == 0 ){ //the step that add lines
    
    if(button == GLUT_LEFT_BUTTON) {	// left button adds vertex
      currentButton = GLUT_LEFT_BUTTON;
      Vector2d cursor(x, y);

      if(state == GLUT_DOWN){
        if ( startPoint == NULL ) {
          startPoint = new Point;
          *startPoint = cursor;
          glutPostRedisplay();
        }
        else {
          FeatureLine line(*startPoint, cursor);
          srcLines.push_back(line);
          startPoint = NULL;
          glutPostRedisplay();
        }
      }
      else{
        currentButton = 0;
        hitpoint = NULL;
      }
      glutPostRedisplay();
      
    }else if(button == GLUT_MIDDLE_BUTTON) {	// middle button ends vertex entry
      if(state == GLUT_UP){
        currentPhase = 1;
        cout << "********************************************************************************"<<endl;
        cout << "Phase 1 : please Adjust Feature lines in both source Image and destination image." << endl;
        cout << "Then press mouse middle button at destination image for morphing." << endl;
        cout << "********************************************************************************"<<endl;
        glutPostRedisplay();
        //glutMouseFunc(editVertices);
        for ( int i = 0; i < srcLines.size(); i++ ) {
          dstLines.push_back(srcLines[i]);
        }
      }
    }
    
  }else if(currentPhase == 1){ // the step that edit lines
    if(button == GLUT_LEFT_BUTTON) {	// left button edits vertex
      currentButton = GLUT_LEFT_BUTTON;
      Vector2d cursor(x, y);

      if(state == GLUT_DOWN){
        for ( int i = 0; i < srcLines.size(); i++ )
          if ( (hitpoint = srcLines[i].hitvertex(cursor)) != NULL ) break;
      }
      else{
        currentButton = 0;
        hitpoint = NULL;
      }
      glutPostRedisplay();
    }
  } //elsethe step at which the morph finished
    //do nothing
}

//------------------------ Mouse handle on destination image -------------------
void dstImgMouseHandle(int button, int state, int x, int y){

  AdjustMouse(x, y);

  //dstImgMouseHandle would only happen when currentPhase == 1

  if(currentPhase == 1){ // the step that edit lines
   
    if (button == GLUT_LEFT_BUTTON) {	// left button adds vertex
      
      glutPostRedisplay();
    
      if(state == GLUT_DOWN) {
        currentButton = GLUT_LEFT_BUTTON;
        Vector2d cursor(x, y);
        for ( int i = 0; i < dstLines.size(); i++ )
          if ( (hitpoint = dstLines[i].hitvertex(cursor)) != NULL ) break;
      }else{
        currentButton = 0;
        hitpoint = NULL;
      }
    }else if(button == GLUT_MIDDLE_BUTTON) {	// left button adds vertex
      if(state == GLUT_UP){
        currentPhase = 2;
        glutPostRedisplay();
        //glutMouseFunc(editVertices);
        cout << "********************************************************************************"<<endl;
        for ( int i = 0; i < srcLines.size(); i++ ) {
          cout << "Source Image Feature Lines: ";
          srcLines[i].print();
          cout << "Destitation Image Feature Lines: ";
          dstLines[i].print();
          cout << endl;
        }
        cout << "********************************************************************************"<<endl;
        
        cout << "Please input the time 't' that you want to morph (a number between 0 and 1): ";
        cin >> tStr;
        istringstream ( tStr ) >> t;
        //to keep the t is in the range of 0 to 1
        while(true){
          if(t <= 1 && t >=0){
            break;
          }else{
            cout << "The number you input is not between 0 and 1, please input again!: ";
            cin >> tStr;
            istringstream ( tStr ) >> t;
          }
        }
        
        //cout<<t<<endl;
        cout << "Morph processing ..." << endl;
        doMorph();
        cout << "Morph finished!!" << endl;
        cout << "********************************************************************************"<<endl;
        cout << "Please input the output file name of the morphed image: ";
        cin >> outputFile;
        imgWrite(outputFile, morphMap, imgWidthOrig, imgHeightOrig);
      }
    }
  }else if(currentPhase == 2){
    
    if(button == GLUT_MIDDLE_BUTTON) {	// left button adds vertex
      if(state == GLUT_UP){
        cout << "********************************************************************************"<<endl;
        cout << "Please input the time 't' that you want to morph (a number between 0 and 1): ";
        cin >> tStr;
        istringstream ( tStr ) >> t;
        //to keep the t is in the range of 0 to 1
        while(true){
          if(t <= 1 && t >=0){
            break;
          }else{
            cout << "The number you input is not between 0 and 1, please input again!: ";
            cin >> tStr;
            istringstream ( tStr ) >> t;
          }
        }
        //cout<<t<<endl;
        cout << "Morph processing ..." << endl;
        doMorph();
        cout << "Morph finished!!" << endl;
        cout << "********************************************************************************"<<endl;
        cout << "Please input the output file name of the morphed image: ";
        cin >> outputFile;
        imgWrite(outputFile, morphMap, imgWidthOrig, imgHeightOrig);
      }
    }
  }
}

//--------------------------------------------------------------------------
//------------------------handleReshape-------------------------------------------
//--------------------------------------------------------------------------
void handleReshape(int w, int h){
  // make the viewport the entire window
  glViewport(0, 0, w, h);
  
  // define the drawing coordinate system on the viewport
  // to be measured in pixels
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluOrtho2D(0, w, 0, h);
  glMatrixMode(GL_MODELVIEW);
}

//--------------------------------------------------------------------------
//------------------------display-------------------------------------------
//--------------------------------------------------------------------------
void display(int imgW, int imgH){
	glClear(GL_COLOR_BUFFER_BIT);

	//get the width and height of current display window
	int w = glutGet(GLUT_WINDOW_WIDTH);
	int h = glutGet(GLUT_WINDOW_HEIGHT);

	// to let the image fit the window when user increase or decrease the window
	if(w >= imgW && h >= imgH){
		glPixelZoom(1,1);
		glRasterPos2i((w-imgW)/2,(h-imgH)/2);
	}else{
		//notice that you need to cast the w/imgWidth or it would be 0 firstly, then be cast to double
		double wZoomfactor = (double)w / (double)imgW;
		double hZoomfactor = (double)h / (double)imgH;
		double zoomfactor;
		if(wZoomfactor <= hZoomfactor){
			zoomfactor = wZoomfactor;
			//cout<<"ZoomFactor:"<<zoomfactor<<"\n"<<endl;
			glPixelZoom(zoomfactor,zoomfactor);
			glRasterPos2i(0,(h-imgH*zoomfactor)/2);
		}else{
			zoomfactor =hZoomfactor;
			//cout<<"ZoomFactor:"<<zoomfactor<<"\n"<<endl;
			glPixelZoom(zoomfactor,zoomfactor);
			glRasterPos2i((w-imgW*zoomfactor)/2,0);
		}
	}
}

void display_original(){
  display(imgWidthOrig, imgHeightOrig);
	glDrawPixels(imgWidthOrig, imgHeightOrig, GL_RGBA, GL_UNSIGNED_BYTE, srcImgMap[0]);
	glFlush();
	
	for ( int i=0; i < srcLines.size(); i++ )
    srcLines[i].draw();

  glutSwapBuffers();
}

void display_dest(){
	display(imgWidthDest, imgHeightDest);
	glDrawPixels(imgWidthDest, imgHeightDest, GL_RGBA, GL_UNSIGNED_BYTE, dstImgMap[0]);
	glFlush();
	
  for ( int i=0; i < dstLines.size(); i++ )
    dstLines[i].draw();

  glutSwapBuffers();
}

//---------------------------------------------------------------
void windowInit(){
  if(!destFlag){
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	  glutInitWindowPosition(0, 100);
	  glutInitWindowSize(imgWidthOrig, imgHeightOrig);
	  glutCreateWindow("Source Image");

	  //display callback, keyboard callback and mouse callback
	  glutKeyboardFunc(handleKey);
	  glutMouseFunc(srcImgMouseHandle);
	  glutMotionFunc(handleMotion);   // mouse motion with button pressed callback
	
	  glutDisplayFunc(&display_original);
    glutReshapeFunc(handleReshape); // window resize callback
    
  }else{
  
    glutInitDisplayMode(GLUT_RGB | GLUT_SINGLE);
	  glutInitWindowPosition(600, 100);
	  glutInitWindowSize(imgWidthDest, imgHeightDest);
	  glutCreateWindow("Destination Image");

	  //display callback, keyboard callback and mouse callback
	  glutKeyboardFunc(handleKey);
	  glutMouseFunc(dstImgMouseHandle);
	  glutMotionFunc(handleMotion);   // mouse motion with button pressed callback
	
	  glutDisplayFunc(&display_dest);
    glutReshapeFunc(handleReshape); // window resize callback
  }  
}

//--------------------------------------------------------------------------
//------------------------coreInit-------------------------------------------
//--------------------------------------------------------------------------
void coreInit(int argc, char * argv[]){

  switch(argc){
    case 3:
      inputFileOrig = argv[1];
      inputFileDest = argv[2];
      imgRead(inputFileOrig, srcImgMap, imgWidthOrig, imgHeightOrig);
      windowInit();
      destFlag = true;
      imgRead(inputFileDest, dstImgMap, imgWidthDest, imgHeightDest);
      windowInit();
      
      // Set up to clear screen to black
      glClearColor(0, 0, 0, 1);
      // Draw with 2 pixel wide lines
      glLineWidth(2);
      currentPhase = 0;
      cout << "********************************************************************************"<<endl;
      cout << "phase " << currentPhase <<" : please add feature line on the source images." << endl;
      cout << "Then you can press middle button of the mouse to enter next phase" << endl;
      cout << "********************************************************************************"<<endl;
      startPoint = NULL;
      hitpoint = NULL;
      
      break;
      
    case 6:
      inputFileOrig = argv[1];
      inputFileDest = argv[2];
      pStr = argv[3];
      aStr = argv[4];
      bStr = argv[5];
      istringstream ( pStr ) >> p;
      istringstream ( aStr ) >> a;
      istringstream ( bStr ) >> b;
      imgRead(inputFileOrig, srcImgMap, imgWidthOrig, imgHeightOrig);
      windowInit();
      destFlag = true;
      imgRead(inputFileDest, dstImgMap, imgWidthDest, imgHeightDest);
      windowInit();
      
      // Set up to clear screen to black
      glClearColor(0, 0, 0, 1);
      // Draw with 2 pixel wide lines
      glLineWidth(2);
      currentPhase = 0;
      cout << "********************************************************************************"<<endl;
      cout << "Current phase " << currentPhase <<" : please add feature line on the source images." << endl;
      cout << "Then you can press middle button of the mouse to enter next phase" << endl;
      cout << "********************************************************************************"<<endl;
      startPoint = NULL;
      hitpoint = NULL;
      
      break;
      
    default:
      cout<<"Usage:"<<endl;
      cout<<"./morph sourceImage.png destinationImage.png [p] [a] [b]"<<endl;
      exit(1); 
  }

}

//--------------------------------------------------------------------------------------------------
//--------------------------line interpolating function---------------------------------------------
//--------------------------------------------------------------------------------------------------
void lineInterpolate(const vector<FeatureLine>& sourceLines, 
                     const vector<FeatureLine>& destLines , vector<FeatureLine>& interLines, float t)
{
  interLines.reserve(sourceLines.size());
	int i;
	for(i=0;i<sourceLines.size();i++){
	  interLines[i].startPoint = (1-t)*(sourceLines[i].startPoint) + t*(destLines[i].startPoint);
	  interLines[i].endPoint = (1-t)*(sourceLines[i].endPoint) + t*(destLines[i].endPoint);
	}
}
//---------------------------------------------------------------------------



//--------------------------------------------------------------------------------------------------
//--------------------------warp function-----------------------------------------------------------
//--------------------------------------------------------------------------------------------------
/* warping function (backward mapping)
   input:
   interPt = the point in the intermediary image
   interLines = given line in the intermediary image
   srcLines = given line in the source image
   p, a, b = parameters of the weight function
   output:
   src = the corresponding point */
void warp(const Vector2d& interPt, const vector<FeatureLine>& interLines,
          const vector<FeatureLine>& sourceLines,
          float p, float a, float b, Vector2d& src)
{
	int i, j;
	float interLength, srcLength;
  float weight, weightSum, dist;
  float sum_x, sum_y; // weighted sum of the coordination of the point "src"
	float u, v;
	Vector2d pd, pq, qd;
  float X, Y;

  sum_x = 0;
  sum_y = 0;
  weightSum = 0;

	for (i=0; i<sourceLines.size(); i++) {
		pd.x = interPt.x - interLines[i].startPoint.x;
		pd.y = interPt.y - interLines[i].startPoint.y;
		pq.x = interLines[i].endPoint.x - interLines[i].startPoint.x;
		pq.y = interLines[i].endPoint.y - interLines[i].startPoint.y;
		interLength = pq.x * pq.x + pq.y * pq.y;
		u = (pd.x * pq.x + pd.y * pq.y) / interLength;

    interLength = sqrt(interLength); // length of the vector PQ
    
		v = (pd.x * pq.y - pd.y * pq.x) / interLength;

		pq.x = sourceLines[i].endPoint.x - sourceLines[i].startPoint.x;
		pq.y = sourceLines[i].endPoint.y - sourceLines[i].startPoint.y;

		srcLength = sqrt(pq.x * pq.x + pq.y * pq.y); // length of the vector P'Q'
		// corresponding point based on the ith line
		X = sourceLines[i].startPoint.x + u * pq.x + v * pq.y / srcLength;
		Y = sourceLines[i].startPoint.y + u * pq.y - v * pq.x / srcLength;

        // the distance from the corresponding point to the line P'Q'
        if (u < 0)
          dist = sqrt(pd.x * pd.x + pd.y * pd.y);
        else if (u > 1) {
          qd.x = interPt.x - interLines[i].endPoint.x;
		      qd.y = interPt.y - interLines[i].endPoint.y;
          dist = sqrt(qd.x * qd.x + qd.y * qd.y);
        }else{
          dist = abs(v);
        }
        
        weight = pow(pow(interLength, p) / (a + dist), b);
        sum_x += X * weight;
        sum_y += Y * weight;
        weightSum += weight;
	}

    src.x = sum_x / weightSum;
    src.y = sum_y / weightSum;
}

//--------------------------------------------------------------------------------------------------
//--------------------------bilinear interpolation--------------------------------------------------
//--------------------------------------------------------------------------------------------------
void bilinear(pixel ** &Im, float row, float col, pixel& pix)
{
	int cm, cn, fm, fn;
	double alpha, beta;

	cm = (int)ceil(row);
	fm = (int)floor(row);
	cn = (int)ceil(col);
	fn = (int)floor(col);
	alpha = ceil(row) - row;
	beta = ceil(col) - col;
	
	pix.r = (unsigned int)( alpha*beta*Im[fm][fn].r
	                          + (1-alpha)*beta*Im[cm][fn].r 
	                          + alpha*(1-beta)*Im[fm][cn].r
	                          + (1-alpha)*(1-beta)*Im[cm][cn].r );
	pix.g = (unsigned int)( alpha*beta*Im[fm][fn].g
		                        + (1-alpha)*beta*Im[cm][fn].g 
		                        + alpha*(1-beta)*Im[fm][cn].g
		                        + (1-alpha)*(1-beta)*Im[cm][cn].g );
	pix.b = (unsigned int)( alpha*beta*Im[fm][fn].b
		                        + (1-alpha)*beta*Im[cm][fn].b 
		                        + alpha*(1-beta)*Im[fm][cn].b
		                        + (1-alpha)*(1-beta)*Im[cm][cn].b );
  pix.a = 255;
}
//---------------------------------------------------------------------------

//--------------------------------------------------------------------------------------------------
//--------------------------color interpolation function--------------------------------------------
//--------------------------------------------------------------------------------------------------
void ColorInterPolate(const Vector2d& Src_P, 
                      const Vector2d& Dest_P, 
                      float t, pixel ** &imgSrc,
                      pixel ** &imgDest, pixel& rgb)
{
    pixel srcColor, destColor;

    //bilinear(imgSrc, Src_P.x, Src_P.y, srcColor);
    //bilinear(imgDest, Dest_P.x, Dest_P.y, destColor);
    bilinear(imgSrc, Src_P.y, Src_P.x, srcColor);
    bilinear(imgDest, Dest_P.y, Dest_P.x, destColor);

    rgb.b = srcColor.b*(1-t)+ destColor.b*t;
    rgb.g = srcColor.g*(1-t)+ destColor.g*t;
    rgb.r = srcColor.r*(1-t)+ destColor.r*t;
}
//---------------------------------------------------------------------------


//--------------------------------------------------------------------------------------------------
//--------------------------morph-------------------------------------------------------------------
//--------------------------------------------------------------------------------------------------
void doMorph(){

    // assign to new IMAGE
    allocPixmap(imgWidthOrig, imgHeightOrig, morphMap);

  	int i, j; //for the for loop
  	Vector2d dest, src;
    Vector2d q;
    pixel interColor;

    // line interpolation
  	lineInterpolate(srcLines, dstLines, morphLines, t);

  	for (i=0; i<imgHeightOrig; i++) {
	    for (j=0; j<imgWidthOrig; j++) {
          q.x = j;
          q.y = i;
          // warping
    	   	warp(q, morphLines, srcLines, p, a, b, src);
	       	warp(q, morphLines, dstLines, p, a, b, dest);

              if (src.x < 0)
                  src.x = 0;
              if (src.x > imgWidthOrig-1)
                  src.x = imgWidthOrig - 1;
              if (src.y < 0)
                  src.y = 0;
              if (src.y > imgHeightOrig-1)
                  src.y = imgHeightOrig - 1;
              if (dest.x < 0)
                  dest.x = 0;
              if (dest.x > imgWidthDest-1)
                  dest.x = imgWidthDest - 1;
              if (dest.y < 0)
                  dest.y = 0;
              if (dest.y > imgHeightDest-1)
                  dest.y = imgHeightDest-1;

          // color interpolation
          ColorInterPolate(src, dest, t, srcImgMap, dstImgMap, interColor);
          
          morphMap[i][j].r = interColor.r;
          morphMap[i][j].g = interColor.g;
          morphMap[i][j].b = interColor.b;
          morphMap[i][j].a = interColor.a;
	    }
   }
}

//------------main function----------------------------
int main(int argc,char *argv[]){
	glutInit(&argc,argv);

  //to initial the program based on different arguments
	coreInit(argc,argv);

	//start the main loop of glut
	glutMainLoop();

	return 0;
}

