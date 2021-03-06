/* A program where the physics of different materials
 * represented as pixels are simulated
 * (one of those sandbox games)
 */
#include <iostream>
#include <cstdint>
#include <cstdlib>  // for int rand(void)


// Materials
const uint8_t specials_start = 0;
const uint8_t solid_start = 2;
const uint8_t fluid_start = 4;

enum materials {
  // specials
  air,  // nothing
  static_pixel,  // unbreakable
  // solid
  sand,
  dirt,
  // fluid
  water
  // gases (not yet implemented)
  // also air is not a gas because it's meant to be nothing
};

// properties for materials
const uint8_t adhesion[fluid_start - solid_start] = {  // only solid materials
  1,  // sand
  3   // dirt
};


class World {
private:
  int world_x;
  int world_y;
  materials *world;
public:
  World(int x, int y);
  ~World();
  void update();
  void spawn_material(materials material, int x, int y);
  void fill_material(materials material);
  void show();
};

World::World(int x, int y) {
  world_x = x;
  world_y = y;
  this->world = new materials[world_y * world_x];
  fill_material(air);
}

World::~World() {
  delete[] this->world;
}

// TODO: update isn't behaving as intended, the sand and dirt aren't falling
void World::update() {
  materials current_material;
  uint8_t current_adhesion;
  for (int i = world_y - 1; i > 1; i--) {  // start from the bottom
    for (int j = 1; j < world_x - 1; j++) {
      current_material = this->world[i*world_x + j];
      if (current_material == air) {
        // do nothing
      } else if (solid_start <= current_material && current_material < fluid_start) {
        current_adhesion = adhesion[current_material - solid_start];
        if (!(solid_start <= this->world[(i+1)*world_x + j] && this->world[(i+1)*world_x + j] < fluid_start) &&
        this->world[(i+1)*world_x + j] != static_pixel) {
          this->world[i*world_x + j] = this->world[(i+1)*world_x + j];
          this->world[(i+1)*world_x + j] = current_material;
        } else if (rand() % current_adhesion == 0) {
          if (!(solid_start <= this->world[(i+1)*world_x + (j-1)] &&
              this->world[(i+1)*world_x + (j-1)] < fluid_start) &&
              this->world[(i+1)*world_x + (j-1)] != static_pixel) {
            this->world[i*world_x + j] = this->world[(i+1)*world_x + (j-1)];
            this->world[(i+1)*world_x + (j-1)] = current_material;
          } else if (!(solid_start <= this->world[(i+1)*world_x + (j+1)] &&
                     this->world[(i+1)*world_x + (j+1)] < fluid_start) &&
                     this->world[(i+1)*world_x + (j+1)] != static_pixel) {
            this->world[i*world_x + j] = this->world[(i+1)*world_x + (j+1)];
            this->world[(i+1)*world_x + (j+1)] = current_material;
          }
        }
      } else if (fluid_start <= current_material) {
        /* code */
      }
    }
  }
}

void World::spawn_material(materials material, int x, int y) {
  if ((0 < x < world_x) && (0 < y < world_y)) {
    this->world[y*world_x + x] = material;
  } else {
    std::cout << "position out of range\n";
  }

}

void World::fill_material(materials material) {
  for (int i = 0; i < world_y; i++) {
    for (int j = 0; j < world_x; j++) {
      // set borders to static_pixel
      if (i == 0 || i == world_y - 1) {
        this->world[i*world_x + j] = static_pixel;
      } else if (j == 0 || j == world_x - 1) {
        this->world[i*world_x + j] = static_pixel;
      } else {
        this->world[i*world_x + j] = material;
      }
    }
  }
}

void World::show() {
  for (int i = 0; i < world_y; i++) {
    for (int j = 0; j < world_x; j++) {
      std::cout << this->world[i*world_x + j];
    }
  std::cout << std::endl;
  }
}

int main() {
  World test(80, 24);
  test.show();
  for (int i = 0; i < 20; i++) {
    test.spawn_material(sand, 10, 2);
    test.spawn_material(dirt, 20, 2);
    test.update();
    test.show();
  }
  for (int i = 0; i < 20; i++) {
    test.update();
    test.show();
  }
  return 0;
}
