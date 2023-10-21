/**
* Author: [Elaine Li]
* Assignment: Pong Clone
* Date due: 2023-10-21, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/


#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#include <Windows.h>
#include <iostream>
#include <SDL.h>
#include <SDL_opengl.h>
#include "cmath"

#define GL_SILENCE_DEPRECATION
#define GL_GLEXT_PROTOTYPES 1
#define STB_IMAGE_IMPLEMENTATION
#define LOG(argument) std::cout << argument << '\n'

#include "glm/mat4x4.hpp"                
#include "glm/gtc/matrix_transform.hpp"  
#include "ShaderProgram.h"
#include "stb_image.h"

const int WINDOW_WIDTH = 640,
WINDOW_HEIGHT = 480;
const float BG_RED = 0.4f,
BG_BLUE = 0.9f,
BG_GREEN = 0.9059f,
BG_OPACITY = 0.2f;

SDL_Window* g_display_window;
bool g_game_is_running = true;


const int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;

const char V_SHADER_PATH[] = "shaders/vertex_textured.glsl";
const char F_SHADER_PATH[] = "shaders/fragment_textured.glsl";
const char PLAYER1_SPRITE_FILEPATH[] = "happy cat.png",
PLAYER2_SPRITE_FILEPATH[] = "banana cat.png",
BALL_SPRITE_FILEPATH[] = "ball.png",
WALL_HORI_SPRITE_FILEPATH[] = "wall horizontal.png",
WALL_VERT_SPRITE_FILEPATH[] = "wall vertical.png",
PLAYER1_WON_SPRITE_FILEPATH[] = "happy won.jpg",
PLAYER2_WON_SPRITE_FILEPATH[] = "banana won.jpg";

ShaderProgram g_shader_program;

glm::mat4 g_view_matrix;
glm::mat4 g_model_matrix;
glm::mat4 g_other_model_matrix;
glm::mat4 g_projection_matrix;
glm::mat4 g_ball_matrix;
glm::mat4 g_upper_wall_matrix,
g_lower_wall_matrix,
g_left_wall_matrix,
g_right_wall_matrix;
glm::mat4 UI_message_matrix;


//delta-time
float g_triangle_x = 0.0f;
float g_previous_ticks = 0.0f;
float g_triangle_rotate = 0.0f;

const float TRIANGLE_RED = 1.0f,
TRIANGLE_BLUE = 0.4f,
TRIANGLE_GREEN = 0.4f,
TRIANGLE_OPACITY = 1.0f;

bool left_won = false;
bool right_won = false;
bool endgame = false;

bool one_player_mode = false;

int g_frame_counter = 0;
bool g_is_growing = true;

float g_x_player = 0.0f;

glm::vec3 PADDLE_SCALE = glm::vec3(0.8f, 1.2f, 0.0f),
BALL_SCALE = glm::vec3(0.2f, 0.2f, 0.0f),
HORI_WALL_SCALE = glm::vec3(5.0f, 0.2f, 0.0f),
VERT_WALL_SCALE = glm::vec3(0.2f, 3.75f, 0.0f),
UI_SCALE = glm::vec3(2.0f, 1.0f, 0.0f);


glm::vec3 g_upper_wall_position = glm::vec3(0.0f, 3.75f, 0.0f),
g_lower_wall_position = glm::vec3(0.0f, -3.75f, 0.0f),
g_left_wall_position = glm::vec3(-5.0f, 0.0f, 0.0f),
g_right_wall_position = glm::vec3(5.0f, 0.0f, 0.0f);

const float wall_thickness = HORI_WALL_SCALE.y;
const float paddle_length = PADDLE_SCALE.y;

const float UPPER_BOUND = g_upper_wall_position.y - wall_thickness / 2;
const float LOWER_BOUND = g_lower_wall_position.y + wall_thickness / 2;

glm::vec3 INIT_BALL_POS = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_ball_position = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_ball_movement = glm::vec3(1.0f, 1.0f, 0.0f);

glm::vec3 g_position = glm::vec3(-4.0f, 0.0f, 0.0f);
glm::vec3 g_movement = glm::vec3(0.0f, 0.0f, 0.0f);


glm::vec3 g_other_position = glm::vec3(4.0f, 0.0f, 0.0f);
glm::vec3 g_other_movement = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 g_other_move_up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 g_other_move_down = glm::vec3(0.0f, -1.0f, 0.0f);



bool paddle_moving_up = true;
bool ball_moving_up = true;
bool ball_moving_right = true;


const float g_other_model_x = 4.0f;

const float g_player_speed = 4.0f;
const float g_ball_speed = 2.0f;

const float MILLISECONDS_IN_SECOND = 1000.0;
const float MINIMUM_COLLISION_DISTANCE = 1.0f;



const float MOVE_ANGLE = glm::radians(45.0f);

GLuint g_player_texture_id,
g_other_texture_id,
g_ball_texture_id,
g_wall_hori_texture_id,
g_wall_vert_texture_id,
g_player1won_texture_id,
g_player2won_texture_id;


const int   NUMBER_OF_TEXTURES = 1;
const GLint LEVEL_OF_DETAIL = 0;
const GLint TEXTURE_BORDER = 0;


//translation
const float TRAN_VALUE = 0.025f;




GLuint load_texture(const char* filepath)
{
    int width, height, number_of_components;
    unsigned char* image = stbi_load(filepath, &width, &height, &number_of_components, STBI_rgb_alpha);

    if (image == NULL)
    {
        LOG("Unable to load image. Make sure the path is correct.");
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


void initialise() {

    std::cout << "HELLO" << std::endl;

    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("Pong Clone",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);

#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);


    //left  paddle
    g_model_matrix = glm::mat4(1.0f);
    g_model_matrix = glm::translate(g_model_matrix, g_position);
    g_model_matrix = glm::scale(g_model_matrix, PADDLE_SCALE);

    //right paddle
    g_other_model_matrix = glm::mat4(1.0f);
    g_other_model_matrix = glm::translate(g_other_model_matrix, g_other_position);
    g_other_model_matrix = glm::scale(g_other_model_matrix, PADDLE_SCALE);

    //ball
    g_ball_matrix = glm::mat4(1.0f);
    g_ball_matrix = glm::translate(g_ball_matrix, INIT_BALL_POS);
    g_ball_matrix = glm::scale(g_ball_matrix, BALL_SCALE);

    //walls
    g_upper_wall_matrix = glm::mat4(1.0f);
    g_lower_wall_matrix = glm::mat4(1.0f);
    g_left_wall_matrix = glm::mat4(1.0f);
    g_right_wall_matrix = glm::mat4(1.0f);

    g_upper_wall_matrix = glm::translate(g_upper_wall_matrix, g_upper_wall_position);
    g_lower_wall_matrix = glm::translate(g_lower_wall_matrix, g_lower_wall_position);
    g_left_wall_matrix = glm::translate(g_left_wall_matrix, g_left_wall_position);
    g_right_wall_matrix = glm::translate(g_right_wall_matrix, g_right_wall_position);
    g_upper_wall_matrix = glm::scale(g_upper_wall_matrix, HORI_WALL_SCALE);
    g_lower_wall_matrix = glm::scale(g_lower_wall_matrix, HORI_WALL_SCALE);
    g_left_wall_matrix = glm::scale(g_left_wall_matrix, VERT_WALL_SCALE);
    g_right_wall_matrix = glm::scale(g_right_wall_matrix, VERT_WALL_SCALE);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-5.0f, 5.0f, -3.75f, 3.75f, -1.0f, 1.0f);

    //UI
    UI_message_matrix = glm::mat4(1.0f);
    UI_message_matrix = glm::translate(UI_message_matrix, glm::vec3(0.0f, 0.0f, 0.0f));
    UI_message_matrix = glm::scale(UI_message_matrix, UI_SCALE);


    g_player_texture_id = load_texture(PLAYER1_SPRITE_FILEPATH);
    g_other_texture_id = load_texture(PLAYER2_SPRITE_FILEPATH);
    g_ball_texture_id = load_texture(BALL_SPRITE_FILEPATH);
    g_wall_hori_texture_id = load_texture(WALL_HORI_SPRITE_FILEPATH);
    g_wall_vert_texture_id = load_texture(WALL_VERT_SPRITE_FILEPATH);
    g_player1won_texture_id = load_texture(PLAYER1_WON_SPRITE_FILEPATH);
    g_player2won_texture_id = load_texture(PLAYER2_WON_SPRITE_FILEPATH);
    

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());
    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


}


void process_input()
{
    if (!endgame) {
        g_movement = glm::vec3(0.0f);
        g_other_movement = glm::vec3(0.0f);

        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch (event.type)
            {
            case SDL_QUIT:
            case SDL_WINDOWEVENT_CLOSE:
                g_game_is_running = false;
                break;

            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_q:
                    g_game_is_running = false;
                    break;

                default:
                    break;
                }

            default:
                break;
            }
        }

        const Uint8* key_state = SDL_GetKeyboardState(NULL);

        if (key_state[SDL_SCANCODE_T])
        {
            one_player_mode = true;
            LOG("PRESS T");
        }


        if (key_state[SDL_SCANCODE_W] && g_position.y + paddle_length / 2 < UPPER_BOUND)
        {
            g_movement.y = 1.0f;
        }
        else if (key_state[SDL_SCANCODE_S] && g_position.y - paddle_length / 2 > LOWER_BOUND)
        {
            g_movement.y = -1.0f;
        }
        if (key_state[SDL_SCANCODE_UP] && g_other_position.y + paddle_length / 2 < UPPER_BOUND)
        {
            g_other_movement.y = 1.0f;
        }
        else if (key_state[SDL_SCANCODE_DOWN] && g_other_position.y - paddle_length / 2 > LOWER_BOUND)
        {
            g_other_movement.y = -1.0f;
        }

        if (glm::length(g_movement) > 1.0f)
        {
            g_movement = glm::normalize(g_movement);
        }

        if (glm::length(g_other_movement) > 1.0f)
        {
            g_other_movement = glm::normalize(g_other_movement);
        }
    }
    
}

void draw_object(glm::mat4& object_model_matrix, GLuint& object_texture_id)
{
    g_shader_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6);
}

//void show_endgame_UI(const char filepath[], float delta_time) {
//
//    UI_message_matrix = glm::mat4(1.0f);
//    UI_message_matrix = glm::scale(UI_message_matrix, UI_SCALE);
//
//    UI_texture_id = load_texture(filepath);
//    
//    draw_object(UI_message_matrix, UI_texture_id);
//    g_cumulative_time += delta_time;
//
//}


void update() {

    if (!endgame) {
        float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
        float delta_time = ticks - g_previous_ticks;
        g_previous_ticks = ticks;

        
        g_model_matrix = glm::mat4(1.0f);

        UI_message_matrix = glm::mat4(1.0f);

        g_position += g_movement * g_player_speed * delta_time;
        g_model_matrix = glm::translate(g_model_matrix, g_position);
        g_movement = glm::vec3(0.0f, 0.0f, 0.0f);

        g_model_matrix = glm::scale(g_model_matrix, PADDLE_SCALE);

        g_other_model_matrix = glm::mat4(1.0f);



        if (one_player_mode == true) {

            if (paddle_moving_up) {
                g_other_position += g_other_move_up * g_player_speed * delta_time;
            }
            else {
                g_other_position += g_other_move_down * g_player_speed * delta_time;
            }
            if (g_other_position.y + paddle_length / 2 >= UPPER_BOUND || g_other_position.y - paddle_length / 2 <= LOWER_BOUND) {
                paddle_moving_up = !paddle_moving_up;
            }
        }
        else {
            g_other_position += g_other_movement * g_player_speed * delta_time;
        }

        g_other_model_matrix = glm::translate(g_other_model_matrix, g_other_position);
        g_other_model_matrix = glm::scale(g_other_model_matrix, PADDLE_SCALE);

        g_other_movement = glm::vec3(0.0f, 0.0f, 0.0f);

        g_ball_matrix = glm::mat4(1.0f);

        //ball-paddle distance
        float left_x_distance = fabs(g_ball_position.x - g_position.x) - ((BALL_SCALE.x + PADDLE_SCALE.x) / 2.0f);
        float left_y_distance = fabs(g_ball_position.y - g_position.y) - ((BALL_SCALE.y + PADDLE_SCALE.y) / 2.0f);
        float right_x_distance = fabs(g_ball_position.x - g_other_position.x) - ((BALL_SCALE.x + PADDLE_SCALE.x) / 2.0f);
        float right_y_distance = fabs(g_ball_position.y - g_other_position.y) - ((BALL_SCALE.y + PADDLE_SCALE.y) / 2.0f);


        //ball-wall distance
        float upper_distance = fabs(g_ball_position.y - g_upper_wall_position.y) - ((BALL_SCALE.y + wall_thickness) / 2.0f);
        float lower_distance = fabs(g_ball_position.y - g_lower_wall_position.y) - ((BALL_SCALE.y + wall_thickness) / 2.0f);

        float left_distance = fabs(g_ball_position.x - g_left_wall_position.x) - ((BALL_SCALE.x + wall_thickness) / 2.0f);
        float right_distance = fabs(g_ball_position.x - g_right_wall_position.x) - ((BALL_SCALE.x + wall_thickness) / 2.0f);



        if (((right_x_distance < 0.0f && right_y_distance < 0.0f)) || (left_x_distance < 0.0f && left_y_distance < 0.0f)) {
            ball_moving_right = !ball_moving_right;
        }
        if (left_distance < 0.0f) {

            right_won = true;
            endgame = true;


        }
        if (right_distance < 0.0f) {
            
            left_won = true;
            endgame = true;
            

        }

        if (upper_distance < 0.0f || lower_distance < 0.0f) {
            ball_moving_up = !ball_moving_up;
        }

        if (ball_moving_right) {
            g_ball_movement.x = 1.0f;
        }
        else {
            g_ball_movement.x = -1.0f;
        }

        if (ball_moving_up) {
            g_ball_movement.y = 1.0f;
        }
        else {
            g_ball_movement.y = -1.0f;
        }

        if (glm::length(g_ball_movement) > 1.0f)
        {
            g_ball_movement = glm::normalize(g_ball_movement);
        }


        g_ball_position += g_ball_movement * g_ball_speed * delta_time;
        g_ball_matrix = glm::translate(g_ball_matrix, g_ball_position);
        g_ball_movement = glm::vec3(0.0f, 0.0f, 0.0f);
        g_ball_matrix = glm::scale(g_ball_matrix, BALL_SCALE);
    }
    


}



void render() {

    
    glClear(GL_COLOR_BUFFER_BIT);


    float vertices[] = {
        -1.0f, -1.0f, 1.0f, -1.0f, 1.0f, 1.0f,
        -1.0f, -1.0f, 1.0f, 1.0f, -1.0f, 1.0f
    };


    float texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,
    };

    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());


    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());
    draw_object(g_model_matrix, g_player_texture_id);
    draw_object(g_other_model_matrix, g_other_texture_id);
    draw_object(g_ball_matrix, g_ball_texture_id);
    draw_object(g_upper_wall_matrix, g_wall_hori_texture_id);
    draw_object(g_lower_wall_matrix, g_wall_hori_texture_id);
    draw_object(g_left_wall_matrix, g_wall_vert_texture_id);
    draw_object(g_right_wall_matrix, g_wall_vert_texture_id);
    if (left_won) {
        draw_object(UI_message_matrix, g_player1won_texture_id);
    }
    if (right_won) {
        draw_object(UI_message_matrix, g_player2won_texture_id);
    }    

    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

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
