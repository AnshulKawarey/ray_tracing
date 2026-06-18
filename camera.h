#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "rtinclude.h"
#include "vec3.h"
#include "material.h"

class camera {
public:
  // Image
  double aspect_ratio = 1.0;
  int image_width = 100;
  int samples_per_pixel = 10;
  int max_depth = 10;

  void render(const hittable &world) {
    initialize();
    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
      std::clog << "\rScanning remaining: " << (image_height - j) << ' '
                << std::flush;
      for (int i = 0; i < image_width; i++) {
        color pixel_color(0, 0, 0);
        for(int sample = 0; sample < samples_per_pixel; sample++){
          ray r = get_ray(i, j);
          pixel_color += ray_color(r, max_depth, world);
        }

        write_color(std::cout, pixel_samples_scale * pixel_color);
      }
    }
  }

private:
  int image_height;           // Rendered image height
  point3 center;              // Camera center
  point3 pixel00_loc;         // Location of pixel 0, 0
  vec3 pixel_delta_u;         // Offset to pixel to the right
  vec3 pixel_delta_v;         // Offset to pixel below
  double pixel_samples_scale; // Color scale factor for a sum of pixel samples

  void initialize() {
    image_height = int(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    pixel_samples_scale = 1.0 / samples_per_pixel;

    // Camera
    auto focal_length = 1.0;
    center = point3(0, 0, 0);

    // Viewport
    auto viewport_height = 2.0;
    auto viewport_width =
        viewport_height * (double(image_width) / image_height);

    // Calc the vectors to go down and left-right on the Viewport (based on the
    // global coordinate system)
    auto viewport_u = vec3(viewport_width, 0, 0);
    auto viewport_v = vec3(0, -viewport_height, 0);

    // Calc the delta vectors using the viewport vectors and image dimensions
    pixel_delta_u = viewport_u / image_width;
    pixel_delta_v = viewport_v / image_height;

    // Calc the position of the top left pixel on the viewport
    // Camera is aligned with the centre of the viewport and the focal length is
    // how much out of the screen the Camera is placed. So we subtract the
    // distance we are out of the screen since the Z axis goes into the plane.
    auto viewport_upper_left =
        center - vec3(0, 0, focal_length) - (viewport_u / 2) - (viewport_v / 2);
    pixel00_loc = viewport_upper_left + 0.5 * (pixel_delta_u + pixel_delta_v);
  }

  ray get_ray(int i, int j) const {
    // generate camera originating rays spread randomly around the target i, j
    auto offset = sample_square();
    auto pixel_sample = pixel00_loc 
                        + ((i + offset.x()) * pixel_delta_u) 
                        + ((j + offset.y()) * pixel_delta_v);
    
    auto ray_origin = center;
    auto ray_direction = pixel_sample - ray_origin;

    return ray(ray_origin, ray_direction);
  }

  vec3 sample_square() const {
    return vec3(random_double() - 0.5, random_double() - 0.5, 0);
  }

  color ray_color(const ray &r, int depth, const hittable &world) const {
    if(depth <= 0){
      return color(0, 0, 0);
    }

    hit_record rec;

    if (world.hit(r, interval(0.001, infinity), rec)) {
      ray scattered;
      color attenuation;
      if(rec.mat->scatter(r, rec, attenuation, scattered)){
        return attenuation * ray_color(scattered, depth-1, world);
      }
      return color(0,0,0);
    }

    vec3 unit_direction = unit_vector(r.direction());
    // Since unit_dir ranges from [-1,1] basically -1 being down and 1 being up,
    // we need to shoft range from [-1,1] to [0,1]
    // so we add 1 to unit_dir's y component (with intent of creating a vertical
    // gradient, can take x if want horizontal gradient) then we divide by 2
    // since adding 1 makes range [0,2]
    auto a = 0.5 * (unit_direction.y() + 1.0);

    // now we use this 'a' to create a gradient,
    // to do that we return a color which will create a gradual shift between
    // the 2 colors we want in this case we will do it from white to sky blue,
    // i.e. (1, 1, 1) to (0.5, 0.7, 1.0)
    // and we make the value using the formula
    // (1-a)*color1 + a*color2
    return (1 - a) * color(1.0, 1.0, 1.0) + a * color(0.5, 0.7, 1.0);
  }
};

#endif