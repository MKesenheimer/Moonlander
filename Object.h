#ifndef OBJECT_H
#define OBJECT_H

#include <vector>

class Object {
  public:
    //the constructor
    Object(float x, float y, float angle);
    //the destructor
    ~Object();
    
    //gives the x and y position in the 2 dimensional world
    float x();
    float y();
    //gives the velocity in x- and y-direction of the object
    float vx();
    float vy();
    //get the angle in respect to the y-axis
    float phi();
    //get the number of points
    float npoints();
    
    //set the x and y position in the 2 dimensional world
    void set_pos(float x, float y);
    void set_v(float vx, float vy);
    
    //set the objects angle
    void set_angle(float angle);
    
    //update the new position of the Object
    void update_position(float dt);
    
    //save a new coordinate and remember the index
    void new_point(float x, float y);
    std::vector<float> get_point(int n);
    void modify_point(float x, float y, int n);

    //update all relevant points in respect to the x- and y-coordinates
    void update_all_points(float dt);

  private:
    float x_;
    float y_;
    float vx_;
    float vy_;
    float phi_; //angle in degree (°)
    float old_phi_; 
    //whenever save_point() is called, we increment this numbers
    int npoints_;
    std::vector<float> points;
};

#endif