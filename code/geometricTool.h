#ifndef _H_GEOMETRICTOOL
#define _H_GEOMETRICTOOL

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#define HIT_RADIUS  4.0

#include "Utility.h"


/* Vector Descriptions and Operations */


class Vector2d {
public:
  double x, y;

  Vector2d(double vx = 0, double vy = 0);
  Vector2d(const Vector2d &v);

  double& operator[](int i);
  const double& operator[](int i) const;
  
  //operator Vector();

  void print() const;
  void print(int w, int p) const;	// print with width and precision

  double norm() const;			// magnitude of vector
  double normsqr() const;		// magnitude squared
  Vector2d normalize() const;		// normalize

  void set(double vx = 0, double vy = 0);	// set assuming y = 0
  void set(const Vector2d &v);

  /* Vector2d operator prototypes */
  friend Vector2d operator-(const Vector2d& v1); // unary negation of vector
  friend Vector2d operator+(const Vector2d& v1, const Vector2d& v2);//addition 
  friend Vector2d operator-(const Vector2d& v1, const Vector2d& v2);//subtract
  friend Vector2d operator*(const Vector2d& v, double s); // scalar mult
  friend Vector2d operator*(double s, const Vector2d& v);
  friend double   operator*(const Vector2d& v1, const Vector2d& v2); // dot
  friend Vector2d operator^(const Vector2d& v1, const Vector2d& v2); //compt *
  //friend Vector3d operator%(const Vector2d& v1, const Vector2d& v2); // cross
  friend Vector2d operator/(const Vector2d& v, double s); // division by scalar
  friend short    operator==(const Vector2d& one, const Vector2d& two); // eq
};

void Disk(double x, double y, double r);

//
// Point class is derived from Vector class and provides ability to draw
// the point and to test for a hit on the point
//
class Point: public Vector2d{
public:
  Point();
  Point(const Vector2d &v);
  int hit(const Vector2d &cursor) const;
  void draw() const;
};

//The FeatureLine class---------------------

class FeatureLine {
public:
  Point startPoint;
  Point endPoint;

  FeatureLine();   // default constructor
  FeatureLine(const Point& start, const Point& end);   // convert constructor

  // Feature line vector coordinate
  Vector2d coordinate();
  Point* hitvertex(const Vector2d &cursor);         // detect whether cursor hit the vertex
  void draw();                                      // draw the feature line
  void print();                                     // print the information of feature line

  // Operators defination
  FeatureLine& operator=(const FeatureLine& line);
  bool operator==(const FeatureLine& line);
};


#endif
