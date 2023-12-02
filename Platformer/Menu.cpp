/**
* Author: [Elaine Li]
* Assignment: Platformer
* Date due: 2023-12-02, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "Menu.h"
#include "Utility.h"



Menu::~Menu()
{
    //delete[] m_state.enemies;
    //delete    m_state.player;
    //delete    m_state.map;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void Menu::initialise()
{
    m_state.next_scene_id = -1;
    //GLuint map_texture_id = Utility::load_texture("map.jpg");
    //m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELA_DATA, map_texture_id, 1.0f, 4, 1);

    /**
     BGM and SFX
     */
    

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    m_state.bgm = Mix_LoadMUS("Adventures in Adventureland.mp3");
    Mix_PlayMusic(m_state.bgm, -1);
    Mix_VolumeMusic(0.0f);


}

void Menu::update(float delta_time)
{
    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        // ！！！！！ KEYSTROKES ！！！！！ //
        switch (event.type) 
        {
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym) {
            case SDLK_RETURN:
                m_state.next_scene_id = 1;
            }
        }
    }
    //if (key_state[SDLK_RETURN]) {
    //    m_state.next_scene_id = 1;
    //}
}


void Menu::render(ShaderProgram* program)
{
    GLuint text_id = Utility::load_texture("font1.png");
    Utility::draw_text(program, text_id, "Platformer", 1, 0, glm::vec3(-4.5, 0.0, 0.0));
    Utility::draw_text(program, text_id, "Press enter to start", 0.4, 0, glm::vec3(-4.0, -1.0, 0.0));
}
