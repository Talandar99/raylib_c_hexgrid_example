#include <raylib.h>

#include "math.h"
#include "stdlib.h"

struct GridTile {
    Vector2 screen_coordinates;
    bool is_visible;
};

void GenerateHexGrid(double radius, double grid_width, double grid_height,
                     struct GridTile** grid) {
    double offset_x = -((radius * 2)) * ((grid_height) * 0.25);
    double offset_y = 100;
    float horiz = sqrtf(3) * radius;
    float vert = (3.0 / 2.0) * radius;
    float triangle_radius = radius * 1.1;
    float triangle_horiz = sqrtf(3) * triangle_radius;
    float triangle_vert = (3.0 / 2.0) * triangle_radius;

    for (int y = 0; y < grid_height; y++) {
        for (int x = -(int)((grid_height - y) + 8);
             x < grid_width - (int)(y * 0.5); x++) {
            int grid_x = x - (int)((grid_height - y) * 0.5);
            if (grid_x >= 0) {
                double hex_x = (horiz * (x + (y * 0.5))) + offset_x;
                double hex_y = (vert * y) + offset_y;
                // set coordinates
                grid[x][y].screen_coordinates.x = hex_x;
                grid[x][y].screen_coordinates.y = hex_y;
                grid[x][y].is_visible = true;
                // set visibility for right edge to be simetrical
                if ((y % 2 == 1) && (x + 1 == grid_width - (int)(y * 0.5))) {
                    grid[x][y].is_visible = false;
                }
            }
        }
    }
};

void DrawVisibleFields(double radius, double width, double height,
                       struct GridTile** grid) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[x][y].is_visible) {
                DrawPoly((Vector2){grid[x][y].screen_coordinates.x,
                                   grid[x][y].screen_coordinates.y},
                         6, radius, 90, BEIGE);
            }
        }
    }
};
void DEBUG_DrawCoordinatesOnHexGrid(double radius, double width, double height,
                                    struct GridTile** grid) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[x][y].is_visible) {
                double hex_x = grid[x][y].screen_coordinates.x;
                double hex_y = grid[x][y].screen_coordinates.y;
                DrawText(TextFormat("y: %d", (int)y), hex_x - (radius / 2),
                         hex_y, 20, LIME);
                DrawText(TextFormat("x: %d", (int)x), hex_x - (radius / 2),
                         hex_y - (radius / 2), 20, VIOLET);
            }
        }
    }
};

void DrawHexGridOutline(double radius, double width, double height,
                        struct GridTile** grid) {
    for (int y = 0; y < height; y++) {
        for (int x = 0; x < width; x++) {
            if (grid[x][y].is_visible) {
                double hex_x = grid[x][y].screen_coordinates.x;
                double hex_y = grid[x][y].screen_coordinates.y;
                DrawPolyLines((Vector2){hex_x, hex_y}, 6, radius, 90, BROWN);
            }
        }
    }
};
int main(void) {
    // Initialization
    // memory allocation
    // consts
    // 720p
    const int screenWidth = 1280;
    const int screenHeight = 720;
    const double radius = 40;
    const int width = 16;
    const int height = 9;
    const int range_in_hexes = 2;
    const int grid_width = width + (height * 0.5);
    const int grid_height = height;
    //----------------------------------------------------------------------------------
    // memory allocation
    //----------------------------------------------------------------------------------
    struct GridTile** grid =
        (struct GridTile**)malloc(grid_width * sizeof(struct GridTile*));
    for (int x = 0; x < grid_width; x++) {
        grid[x] =
            (struct GridTile*)malloc(grid_height * sizeof(struct GridTile));
    }

    //----------------------------------------------------------------------------------
    // Setup
    //----------------------------------------------------------------------------------
    for (int y = 0; y < grid_height; y++) {
        for (int x = 0; x < grid_width; x++) {
            // toggle off visibility;
            grid[x][y].is_visible = false;
        }
    }
    GenerateHexGrid(radius, grid_width, grid_height, grid);
    InitWindow(screenWidth, screenHeight, "hex grid demo");
    SetTargetFPS(60);
    while (!WindowShouldClose()) {
        //----------------------------------------------------------------------------------
        // Update
        //----------------------------------------------------------------------------------
        //
        //----------------------------------------------------------------------------------
        //  Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BROWN);

        DrawVisibleFields(radius, grid_width, grid_height, grid);
        DrawHexGridOutline(radius, grid_width, grid_height, grid);
        DEBUG_DrawCoordinatesOnHexGrid(radius, grid_width, grid_height, grid);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }
    CloseWindow();
    return 0;
}
