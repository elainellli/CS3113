/**
* Author: [Elaine Li]
* Assignment: Simple 2D Scene
* Date due: 2023-09-30, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION

#define LOG(argument) std::cout << argument << '\n'

#include "glm/mat4x4.hpp"                
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"             
#include "stb_image.h"

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl";  
const char F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

const char PLAYER_SPRITE_FILEPATH1[] = "C:/Users/Lenovo/source/repos/Simple 2D Scene/Simple 2D Scene/V.png";
const char PLAYER_SPRITE_FILEPATH2[] = "C:/Users/Lenovo/source/repos/Simple 2D Scene/Simple 2D Scene/wolves.png";

const int WINDOW_WIDTH = 640,
WINDOW_HEIGHT = 480;

const float BG_RED = 0.4f,
BG_BLUE = 0.9f,
BG_GREEN = 0.9059f,
BG_OPACITY = 0.2f;

const int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;


SDL_Window* g_display_window;
bool g_game_is_running = true;

ShaderProgram g_program;

glm::mat4 g_view_matrix;        
glm::mat4 g_square_matrix, g_hexagon_matrix;       
glm::mat4 g_projection_matrix;

float g_model_x = 0.0f;
float g_model_y = 0.0f;
float g_previous_ticks = 0.0f;

float g_angle = 0.0f;
float g_radius = 2.0f;
float g_hexagon_rotate = 0.0f;

float cumulative_scale = 1.0f;
float SHRINK_FACTOR = 0.99f;
float GROWTH_FACTOR = 1.01f;
const float MAX_SCALE = 1.2f;
const float MIN_SCALE = 0.3f;

const int MAX_FRAME = 2000;
int g_frame_counter = 0;
bool g_is_growing = true;

const float ROT_ANGLE = glm::radians(1.5f);

const int NUMBER_OF_TEXTURES = 1;
const GLint LEVEL_OF_DETAIL = 0;
const GLint TEXTURE_BORDER = 0;

GLuint g_square_texture_id, g_hexagon_texture_id;


GLuint load_texture(const char* filepath)
{
    // STEP 1: Loading the image file
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);

    if (image == NULL)
    {
        ("Unable to load image. Make sure the path is correct.");
        assert(false);
    }

    GLuint textureID;
    glGenTextures(NUMBER_OF_TEXTURES, &textureID);
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, LEVEL_OF_DETAIL, GL_RGBA, width, height, TEXTURE_BORDER, GL_RGBA, GL_UNSIGNED_BYTE, image);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    stbi_image_free(image);

    return textureID;
}

void draw_object(glm::mat4& object_model_matrix, GLuint& object_texture_id)
{
    g_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

void initialise() {
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("HW1",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);
    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif
    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);
    g_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_square_matrix = glm::mat4(1.0f);
    g_hexagon_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    g_program.set_view_matrix(g_view_matrix);
    g_program.set_projection_matrix(g_projection_matrix);

    glUseProgram(g_program.get_program_id());

    g_square_texture_id = load_texture(PLAYER_SPRITE_FILEPATH1);
    g_hexagon_texture_id = load_texture(PLAYER_SPRITE_FILEPATH2);

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void process_input() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
            g_game_is_running = false;
        }
    }
}

void update() {


    float ticks = (float)SDL_GetTicks() / 1000.0f;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    if (g_frame_counter >= MAX_FRAME) {
        g_is_growing = !g_is_growing;
        g_frame_counter = 0;
    
    }
    
    if (g_is_growing) {
        if (cumulative_scale < MAX_SCALE) {
            cumulative_scale += GROWTH_FACTOR * delta_time ;
        }       
    }
    else {
        if (cumulative_scale > MIN_SCALE) {
            cumulative_scale -= SHRINK_FACTOR * delta_time;
        }        
    }
    
    g_frame_counter += 1;

    g_angle += 1.0f * delta_time;
    g_hexagon_rotate += 90.0f * delta_time;
    g_model_x =  glm::cos(g_angle) * g_radius;
    g_model_y = glm::sin(g_angle) * g_radius;
    g_square_matrix = glm::mat4(1.0f);
    g_square_matrix = glm::translate(g_square_matrix, glm::vec3(g_model_x, g_model_y, 0.0f));
       
    glm::vec3 scale_vector = glm::vec3(cumulative_scale, cumulative_scale, 1.0f);
    g_square_matrix = glm::scale(g_square_matrix, scale_vector);    

    g_hexagon_matrix = glm::mat4(1.0f);
    g_hexagon_matrix = glm::translate(g_square_matrix, glm::vec3(1.5*g_model_x, 1.5*g_model_y, 0.0f));
    g_hexagon_matrix = glm::rotate(g_hexagon_matrix, glm::radians(g_hexagon_rotate), glm::vec3(0.0f, 0.0f, 1.0f));
    

}

void render() {

    glClear(GL_COLOR_BUFFER_BIT);

    g_program.set_model_matrix(g_square_matrix);
    g_program.set_model_matrix(g_hexagon_matrix);
    
    float vertices1[] =
    {
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
    };

    float texture_coordinates_1[] = {

        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };

    float vertices2[] =
    {
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
    };

    float texture_coordinates_2[] = {

        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };

    //First object
    glVertexAttribPointer(g_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices1);
    glEnableVertexAttribArray(g_program.get_position_attribute());

    glVertexAttribPointer(g_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates_1);
    glEnableVertexAttribArray(g_program.get_tex_coordinate_attribute());
    draw_object(g_square_matrix, g_square_texture_id);
    glDisableVertexAttribArray(g_program.get_position_attribute());

    //Second object
    glVertexAttribPointer(g_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices2);
    glEnableVertexAttribArray(g_program.get_position_attribute());
    
    glVertexAttribPointer(g_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates_2);
    glEnableVertexAttribArray(g_program.get_tex_coordinate_attribute());
    draw_object(g_hexagon_matrix, g_hexagon_texture_id);
    glDisableVertexAttribArray(g_program.get_position_attribute());
    glDisableVertexAttribArray(g_program.get_tex_coordinate_attribute());
    
    SDL_GL_SwapWindow(g_display_window);
}

void shutdown() {
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    initialise();

    while (g_game_is_running) {

        process_input();

        update();

        render();
    }
    shutdown();
    return 0;
}