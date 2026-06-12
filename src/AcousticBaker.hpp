#include <algorithm>
#include <cmath>
#include <vector>

// Our discrete acoustic map grid
// 0 = air, 1 = solid barrier
std::vector<uint8_t> acoustic_grid;
int grid_width = 0;
int grid_height = 0;

// Size of each grid square in engine coordinate units
const float TILE_SIZE = 32.0f;

void bake_acoustic_grid(const cosmos &world, float map_pixel_width,
                        float map_pixel_height) {
  // Determine grid resolution
  grid_width = static_cast<int>(map_pixel_width / TILE_SIZE);
  grid_height = static_cast<int>(map_pixel_height / TILE_SIZE);

  // Initialize with zeros (clear air)
  acoustic_grid.assign(grid_width * grid_height, 0);

  // Loop through every entity possessing a polygon shape component
  world.for_each_having<invariants::shape_polygon>([&](const auto &entity) {
    // Access the custom shape definition inside the component
    const auto &polygon_component =
        entity.template get<invariants::shape_polygon>();
    const auto &poly_shape = polygon_component.shape;

    /*
     *         Hypersomnia's basic_convex_partitioned_shape template usually
     * exposes either an explicit bounding box helper or an accessible
     * collection of vertices.
     *
     *         If 'poly_shape' has a bounding box helper function, we grab its
     * min/max boundaries directly. If it uses raw vertices, a bounding box can
     * be extracted by finding the min/max X and Y:
     */

    float min_x = 999999.0f, max_x = -999999.0f;
    float min_y = 999999.0f, max_y = -999999.0f;

    // Note: Replace this placeholder loop with the engine's exact vertex loop
    // structure (e.g., poly_shape.get_vertices() or poly_shape.vertices) if a
    // direct bounding box method is unavailable.
    for (const auto &vertex : poly_shape.vertices) {
      if (vertex.x < min_x)
        min_x = vertex.x;
      if (vertex.x > max_x)
        max_x = vertex.x;
      if (vertex.y < min_y)
        min_y = vertex.y;
      if (vertex.y > max_y)
        max_y = vertex.y;
    }

    // Convert the structural bounds to discrete tile indexes
    int start_x = std::max(0, static_cast<int>(min_x / TILE_SIZE));
    int start_y = std::max(0, static_cast<int>(min_y / TILE_SIZE));
    int end_x = std::min(grid_width - 1, static_cast<int>(max_x / TILE_SIZE));
    int end_y = std::min(grid_height - 1, static_cast<int>(max_y / TILE_SIZE));

    // Fill the flat 1D grid representation for this barrier
    for (int y = start_y; y <= end_y; ++y) {
      for (int x = start_x; x <= end_x; ++x) {
        int linear_index = (y * grid_width) + x;
        acoustic_grid[linear_index] = 1; // Mark voxel as a solid wall
      }
    }
  });
}
struct GridPos {
  int x, y;
};

void flood_fill_outdoor_space() {
  // 1. Establish a queue for Breadth-First Search (BFS)
  std::queue<GridPos> search_queue;

  // 2. Seed the queue from the edges of the map (assuming edges are outdoors)
  // We will push the top and bottom rows as starting seed points
  for (int x = 0; x < grid_width; ++x) {
    if (acoustic_grid[x] == 0) { // Top row
      acoustic_grid[x] = 2;      // Mark as Outdoor
      search_queue.push({x, 0});
    }
    int bottom_idx = ((grid_height - 1) * grid_width) + x;
    if (acoustic_grid[bottom_idx] == 0) { // Bottom row
      acoustic_grid[bottom_idx] = 2;
      search_queue.push({x, grid_height - 1});
    }
  }

  // Direction arrays for traveling North, South, East, West
  const int dx[] = {0, 0, 1, -1};
  const int dy[] = {1, -1, 0, 0};

  // 3. Process the queue
  while (!search_queue.empty()) {
    GridPos current = search_queue.front();
    search_queue.pop();

    // Check all 4 neighboring tiles
    for (int i = 0; i < 4; ++i) {
      int next_x = current.x + dx[i];
      int next_y = current.y + dy[i];

      // Ensure neighbor is inside the map bounds
      if (next_x >= 0 && next_x < grid_width && next_y >= 0 &&
          next_y < grid_height) {
        int neighbor_idx = (next_y * grid_width) + next_x;

        // If the neighbor is unmarked air (0), it's connected to the outside!
        if (acoustic_grid[neighbor_idx] == 0) {
          acoustic_grid[neighbor_idx] = 2;     // Tag as Outdoor
          search_queue.push({next_x, next_y}); // Expand search
        }
      }
    }
  }

  /* At this point:
   *      - 1 = Solid Walls
   *      - 2 = Continuous Outdoor Air
   *      - Any remaining 0s = Completely isolated Indoor rooms!
   */
}
