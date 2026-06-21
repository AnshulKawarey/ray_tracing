#ifndef CAMERA_H
#define CAMERA_H

#include "color.h"
#include "hittable.h"
#include "rtinclude.h"
#include "vec3.h"
#include "material.h"
#include "timer.h"
#include <atomic>

class camera {
public:
  // Image
  float aspect_ratio = 1.0;
  int image_width = 100;
  int samples_per_pixel = 10;
  int max_depth = 10;

  void render_tile(int tile_x, int tile_y, int tile_size, const hittable &world, std::vector<color>& pixels) const {
    int x_start = tile_x * tile_size;
    int y_start = tile_y * tile_size;
    int x_end = std::min(x_start + tile_size, image_width);
    int y_end = std::min(y_start + tile_size, image_height);

    for(int j = y_start; j < y_end; j++){
      for(int i = x_start; i<x_end; i++){
        color pixel_color(0, 0, 0);
        for(int sample = 0; sample < samples_per_pixel; sample++){
          ray r = get_ray(i, j);
          pixel_color += ray_color(r, max_depth, world);
        }

        pixels[j * image_width + i] = pixel_samples_scale * pixel_color;
      }
    }
  }

  void render(const hittable &world) {
    initialize();
    // Render

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    std::vector<color> pixels(image_width * image_height);
    const int TILE = 16;
    int num_tiles_x = (image_width + TILE - 1) / TILE;
    int num_tiles_y = (image_height + TILE - 1) / TILE;
    int total_tiles = num_tiles_x * num_tiles_y;

    std::atomic<int> tiles_done{0};

    #pragma omp parallel for schedule(dynamic, 1)
    for (int t = 0; t < total_tiles; ++t) {
      render_tile(t % num_tiles_x, t / num_tiles_x, TILE, world, pixels);
      std::clog << "\r" << ++tiles_done << "/" << total_tiles << " tiles" << std::flush;
    }

    for (int j = 0; j < image_height; j++) {
      for(int i = 0; i < image_width; i++) {
        write_color(std::cout, pixels[j * image_width + i]);
      }
    }
  }

private:
  int image_height;           // Rendered image height
  point3 center;              // Camera center
  point3 pixel00_loc;         // Location of pixel 0, 0
  vec3 pixel_delta_u;         // Offset to pixel to the right
  vec3 pixel_delta_v;         // Offset to pixel below
  float pixel_samples_scale; // Color scale factor for a sum of pixel samples

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
        viewport_height * (float(image_width) / image_height);

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
    return vec3(random_float() - 0.5, random_float() - 0.5, 0);
  }

  color ray_color(const ray &r_in, int depth, const hittable &world) const {
    color accumulated(1.0,1.0,1.0);
    ray current_ray = r_in;
    for(size_t depth = 0; depth < max_depth; depth++){
      hit_record rec;
      if(world.hit(current_ray, interval(0.001, infinity), rec)){
        ray scattered;
        color attenuation;
        if(rec.mat->scatter(current_ray, rec, attenuation, scattered)){
          accumulated = accumulated * attenuation;
          current_ray = scattered;
        }
        else{
          return color(0, 0, 0);    
        }
      }
      else{
        // ray escaped to sky -> apply accumulated attenuation
        vec3 unit_direction = unit_vector(current_ray.direction());
        auto a = 0.5f * (unit_direction.y() + 1.0f);
        color sky_color = (1 - a) * color(1.0f, 1.0f, 1.0f) + a * color(0.5f, 0.7f, 1.0f);
        return accumulated * sky_color;
      }
    }
    return color(0, 0, 0); // exceeded max depth
  }
};

#endif