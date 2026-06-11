#include "rtinclude.h"
#include "sphere.h"
#include "hittable.h"
#include "hittable_list.h"
#include "vec3.h"
#include "color.h"
#include "camera.h"


int main(){
    // Coordinate system is defined with 
    // x going left-right, 
    // y going down-up, 
    // z going into the screen
    
    // World
    hittable_list world;
    
    world.add(make_shared<sphere>(point3(0,0,-1), 0.5));
    // world.add(make_shared<sphere>(point3(0,10,-10), 0.5));
    // world.add(make_shared<sphere>(point3(10,0,-1), 0.5));
    world.add(make_shared<sphere>(point3(0,-100.5,-1), 100));
    
    camera cam;
    // Image
    cam.aspect_ratio= 16.0 / 9.0;
    cam.image_width = 400;
    
    cam.render(world);

    
    std::clog << "\rDone.                          \n"; 
}