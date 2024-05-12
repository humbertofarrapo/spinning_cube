#include <math.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

float angle_a;
float angle_b;
float angle_c;
float horizontal_offset;

const float INCREMENT_SPEED = 0.6;
const int WINDOW_WIDTH = 150;
const int WINDOW_HEIGHT = 34;

float depth_buffer[150 * 34];
char render_buffer[150 * 34];

float calculate_x(int i, int j, int k)
{
    return (
        j * sin(angle_a) * sin(angle_b) * cos(angle_c) - k * cos(angle_a) * sin(angle_b) * cos(angle_c) +
        j * cos(angle_a) * sin(angle_c) + k * sin(angle_a) * sin(angle_c) + i * cos(angle_b) * cos(angle_c));
}

float calculate_y(int i, int j, int k)
{
    return (
        j * cos(angle_a) * cos(angle_c) + k * sin(angle_a) * cos(angle_c) -
        j * sin(angle_a) * sin(angle_b) * sin(angle_c) + k * cos(angle_a) * sin(angle_b) * sin(angle_c) -
        i * cos(angle_b) * sin(angle_c));
}

float calculate_z(int i, int j, int k)
{
    return (
        k * cos(angle_a) * cos(angle_b) -
        j * sin(angle_a) * cos(angle_b) + i * sin(angle_b));
}

void calculate_for_surface(float cube_x, float cube_y, float cube_z, int ch)
{
    const int DISTANCE_FROM_CAMERA = 100;
    float x = calculate_x(cube_x, cube_y, cube_z);
    float y = calculate_y(cube_x, cube_y, cube_z);
    float z = calculate_z(cube_x, cube_y, cube_z) + DISTANCE_FROM_CAMERA;

    float inverted_z = 1 / z;

    int projected_x = (int)(WINDOW_WIDTH / 2 + horizontal_offset + 40 * inverted_z * x * 2);
    int projected_y = (int)(WINDOW_HEIGHT / 2 + 40 * inverted_z * y);

    int buffer_index = projected_x + projected_y * WINDOW_WIDTH;
    if (buffer_index >= 0 && buffer_index < WINDOW_WIDTH * WINDOW_HEIGHT)
    {
        if (inverted_z > depth_buffer[buffer_index])
        {
            depth_buffer[buffer_index] = inverted_z;
            render_buffer[buffer_index] = ch;
        }
    }
}

int main()
{
    printf("\x1b[2J");
    while (1)
    {
        float cube_width;
        int background_ascii_code = ' ';
        memset(render_buffer, background_ascii_code, WINDOW_WIDTH * WINDOW_HEIGHT);
        memset(depth_buffer, 0, WINDOW_WIDTH * WINDOW_HEIGHT * 4);

        cube_width = 15;
        horizontal_offset = -2 * cube_width;

        for (float cube_x = -cube_width; cube_x < cube_width; cube_x += INCREMENT_SPEED)
        {
            for (float cube_y = -cube_width; cube_y < cube_width;
                 cube_y += INCREMENT_SPEED)
            {
                calculate_for_surface(cube_x, cube_y, -cube_width, '@');
                calculate_for_surface(cube_width, cube_y, cube_x, '$');
                calculate_for_surface(-cube_width, cube_y, -cube_x, '~');
                calculate_for_surface(-cube_x, cube_y, cube_width, '#');
                calculate_for_surface(cube_x, -cube_width, -cube_y, ';');
                calculate_for_surface(cube_x, cube_width, cube_y, '+');
            }
        }

        cube_width = 10;
        horizontal_offset = 1 * cube_width;

        for (float cube_x = -cube_width; cube_x < cube_width; cube_x += INCREMENT_SPEED)
        {
            for (float cube_y = -cube_width; cube_y < cube_width;
                 cube_y += INCREMENT_SPEED)
            {
                calculate_for_surface(cube_x, cube_y, -cube_width, '@');
                calculate_for_surface(cube_width, cube_y, cube_x, '$');
                calculate_for_surface(-cube_width, cube_y, -cube_x, '~');
                calculate_for_surface(-cube_x, cube_y, cube_width, '#');
                calculate_for_surface(cube_x, -cube_width, -cube_y, ';');
                calculate_for_surface(cube_x, cube_width, cube_y, '+');
            }
        }

        cube_width = 5;
        horizontal_offset = 8 * cube_width;

        for (float cube_x = -cube_width; cube_x < cube_width; cube_x += INCREMENT_SPEED)
        {
            for (float cube_y = -cube_width; cube_y < cube_width;
                 cube_y += INCREMENT_SPEED)
            {
                calculate_for_surface(cube_x, cube_y, -cube_width, '@');
                calculate_for_surface(cube_width, cube_y, cube_x, '$');
                calculate_for_surface(-cube_width, cube_y, -cube_x, '~');
                calculate_for_surface(-cube_x, cube_y, cube_width, '#');
                calculate_for_surface(cube_x, -cube_width, -cube_y, ';');
                calculate_for_surface(cube_x, cube_width, cube_y, '+');
            }
        }

        printf("\x1b[H");
        for (int k = 0; k < WINDOW_WIDTH * WINDOW_HEIGHT; k++)
        {
            if (k % WINDOW_WIDTH)
            {
                putchar(render_buffer[k]);
            }
            else
            {
                putchar(10);
            }
        }

        angle_a += 0.05;
        angle_b += 0.05;
        angle_c += 0.01;

        usleep(8000 * 2);
    }

    return 0;
}
