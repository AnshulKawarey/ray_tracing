#include "vec3.h"
#include "color.h"
#include "ray.h"
#include <cmath>


double hit_sphere(const point3& center, double radius, const ray& r){
    // The math behond it is the sphere equation of x^2 + y^2 + z^2 = r^2,
    // Which with 2 points just becomes (x1-x2)^2 and resp.for y and z,
    // so when we put it in Vector terms with 3 Dimensions it is simply (A-B).(A-B) = r^2
    // which when put in our terms is C (center) and P (point) so (C-P).(C-P) = r^2 so every P which satisfies this lies on the sphere
    // if our ray P(t)=Q+td 
    // (C−P(t))⋅(C−P(t))=r^2 which can be found by replacing P(t)
    // with its expanded form: (C−(Q+td))⋅(C−(Q+td))=r^2
    // instead of putting this equation and finding 6 resultant vectors 
    // we can solve for t since that is the only unknown in this equation
    // t^2d⋅d−2td⋅(C−Q)+(C−Q)⋅(C−Q)−r^2=0
    // here to see if it hits or not we just check the determinant of b^2 - 4ac to be 0 or greater
    // and to return the point, we calc -b-discriminant / 2 a
    // a = d * d
    // b = -2 * d * (C-Q)
    // but we can simplify by considering b = -2h
    vec3 oc = center - r.origin();
    auto a = r.direction().length_squared();
    auto h = dot(r.direction(), oc);
    auto c = oc.length_squared() - radius*radius;

    auto discriminant = h*h - a*c;

    if(discriminant<0){
        return -1.0;
    }
    else{
        return (h - std::sqrt(discriminant)) / a;
    }
}


color ray_color(const ray& r){
    // color the sphere red
    auto t = hit_sphere(point3(0, 0, -1), 0.5, r);
    if(t > 0.0){
        // Normal Vector
        vec3 N = unit_vector(r.at(t) - vec3(0,0,-1));
        return 0.5 * color(N.x()+1, N.y()+1, N.z()+1);
    }
    
   
   
    vec3 unit_direction = unit_vector(r.direction());
    // Since unit_dir ranges from [-1,1] basically -1 being down and 1 being up,
    // we need to shoft range from [-1,1] to [0,1]
    // so we add 1 to unit_dir's y component (with intent of creating a vertical gradient, can take x if want horizontal gradient)
    // then we divide by 2 since adding 1 makes range [0,2]
    auto a = 0.5 * (unit_direction.y() + 1.0);
    
    // now we use this 'a' to create a gradient,
    // to do that we return a color which will create a gradual shift between the 2 colors we want
    // in this case we will do it from white to sky blue,
    // i.e. (1, 1, 1) to (0.5, 0.7, 1.0)
    // and we make the value using the formula
    // (1-a)*color1 + a*color2
    return (1 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
}



int main(){
    // Coordinate system is defined with 
    // x going left-right, 
    // y going down-up, 
    // z going into the screen
    
    
    // Image
    auto aspect_ratio= 16.0 / 9.0;
    int image_width = 400;

    int image_height = int(image_width/aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // Camera
    auto focal_length = 1.0;
    auto camera_center = point3(0,0,0);
    
    // Viewport
    auto viewport_height = 2.0;
    auto viewport_width = viewport_height * (double(image_width)/image_height);

    // Calc the vectors to go down and left-right on the Viewport (based on the global coordinate system)
    auto viewport_u = vec3(viewport_width, 0 ,0); 
    auto viewport_v = vec3(0 , -viewport_height ,0); 

    // Calc the delta vectors using the viewport vectors and image dimensions
    auto pixel_delta_u = viewport_u / image_width;
    auto pixel_delta_v = viewport_v / image_height;

    // Calc the position of the top left pixel on the viewport
    // Camera is aligned with the centre of the viewport and the focal length is how much out of the screen the Camera is placed. 
    // So we subtract the distance we are out of the screen since the Z axis goes into the plane.
    auto viewport_upper_left = camera_center - vec3(0, 0, focal_length) - (viewport_u/2) - (viewport_v/2);
    auto pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);

    // Render 

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for(int j = 0; j < image_height; j++){
        std::clog << "\rScanning remaining: " << (image_height-j) << ' ' <<std::flush;
        for(int i = 0; i < image_width; i++){
            auto pixel_center = pixel00_loc + (i * pixel_delta_u) + (j * pixel_delta_v);
            auto ray_direction = pixel_center - camera_center; // destination - source of 2 points gives a vector direction
            ray r = ray(camera_center, ray_direction);

            color pixel_color = ray_color(r);

            write_color(std::cout, pixel_color);
        }
    }
    std::clog << "\rDone.                          \n"; 
}