/**
* Author: [Elaine Li]
* Assignment: [PathFinder]
* Date due: 2023-12-15, 11:59pm
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/
#include "LevelC.h"
#include "Utility.h"

#define LEVEL_WIDTH 16
#define LEVEL_HEIGHT 10
#define COLLAPSE_BLOCKS 3 
#define KEYS 2


//std::vector <bool> get_key3;
//std::vector <bool> is_collided3;


//unsigned int LEVELC_DATA[] =
//{
//    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,
//    30, 0, 0, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 30,
//    30, 0, 0, 0, 30, 0, 30, 0, 30, 30, 30, 30, 30, 0,0, 30,
//    30, 30, 30, 30, 30, 0, 30, 0, 0, 0, 0, 0, 30, 30, 0, 30,
//    30, 0, 0, 0, 0, 0, 30, 30, 30, 30, 30, 0, 0, 0, 0, 30,
//    30, 0, 30, 30, 30, 30, 30, 0, 0, 0, 30, 0, 30, 30, 30, 30,
//    30, 0, 0, 0, 0, 0, 30, 0, 30, 0, 30, 0, 30, 0, 0, 30,
//    30, 30, 30, 30, 30, 0, 30, 0, 30, 30, 30, 0, 30, 30, 0, 30,
//    30, 0, 0, 0, 0, 0, 30, 0, 0, 0, 0, 0, 0, 0, 0, 30,
//    30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30, 30,30
//};
unsigned int LEVELC_DATA[] =
{
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
    1, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1,
    1, 0, 0, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 0,0, 1,
    1, 1, 1, 1, 1, 0, 1, 0, 0, 0, 0, 0, 1, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 1, 1,1,1,1, 0, 0, 0, 0,1,
    1, 0, 1,1,1,1,1, 0, 0, 0, 1, 0, 1, 1,1,1,
    1, 0, 0, 0, 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 0, 1,
    1, 1,1,1,1, 0, 1, 0, 1, 1, 1, 0, 1, 1, 0, 1,
    1, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1,
    1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

LevelC::~LevelC()
{
    delete[] m_state.enemies;
    delete[] m_state.keys;
    delete[] m_state.fake_map;
    delete m_state.box;
    delete    m_state.player;
    delete    m_state.map;
    m_state.enemies = nullptr;
    m_state.keys = nullptr;
    m_state.fake_map = nullptr;
    m_state.box = nullptr;
    m_state.player = nullptr;
    m_state.map = nullptr;
    Mix_FreeChunk(m_state.jump_sfx);
    Mix_FreeMusic(m_state.bgm);
}

void LevelC::initialise()
{
    m_state.next_scene_id = -1;
    /*GLuint map_texture_id = Utility::load_texture("tileset.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 10, 6);*/
    GLuint map_texture_id = Utility::load_texture("Tile_31.png");
    m_state.map = new Map(LEVEL_WIDTH, LEVEL_HEIGHT, LEVELC_DATA, map_texture_id, 1.0f, 1, 1);


    m_state.fake_map = new Entity[COLLAPSE_BLOCKS];
    /*m_state.fake_map->set_entity_type(FAKEMAP);
    m_state.fake_map->m_texture_id = Utility::load_texture("fake.png");
    m_state.fake_map->set_width(1.0f);
    m_state.fake_map->set_height(1.0f);
    m_state.fake_map->m_animation_indices = new int[2] {0, 1};
    m_state.fake_map->m_animation_cols = 2;
    m_state.fake_map->m_animation_rows = 1;
    m_state.fake_map->m_animation_frames = 1;
    m_state.fake_map->m_animation_index = 0;
    m_state.fake_map->m_animation_time = 0.0f;
    
    for (int i = 0;i < COLLAPSE_BLOCKS;i++)
    {
        m_state.fake_map->is_collided.push_back(false);
    }*/
    for (int i = 0;i < COLLAPSE_BLOCKS;i++)
    {
        m_state.fake_map[i].set_entity_type(FAKEMAP);
        m_state.fake_map[i].m_texture_id = Utility::load_texture("fake.png");
        m_state.fake_map[i].set_width(1.0f);
        m_state.fake_map[i].set_height(1.0f);
        m_state.fake_map[i].m_animation_indices = new int[2] {0, 1};
        m_state.fake_map[i].m_animation_cols = 2;
        m_state.fake_map[i].m_animation_rows = 1;
        m_state.fake_map[i].m_animation_frames = 1;
        m_state.fake_map[i].m_animation_index = 0;
        m_state.fake_map[i].m_animation_time = 0.0f;
        m_state.fake_map->is_collided.push_back(false);

    }
    m_state.fake_map[0].set_position(glm::vec3(4.0f, -1.0f, 0.0f));
    m_state.fake_map[1].set_position(glm::vec3(12.0f, -4.0f, 0.0f));
    m_state.fake_map[2].set_position(glm::vec3(7.0f, -7.0f, 0.0f));




    // Code from main.cpp's initialise()
    /**
     George's Stuff
     */
     // Existing
    m_state.player = new Entity();
    m_state.player->set_entity_type(PLAYER);
    m_state.player->set_position(glm::vec3(5.0f, -7.0f, 0.0f));
    m_state.player->set_movement(glm::vec3(0.0f));
    m_state.player->set_speed(4.0f);
    //m_state.player->set_acceleration(glm::vec3(0.0f, -5.0f, 0.0f));
    m_state.player->m_texture_id = Utility::load_texture("spritesheet.png");

    // Walking
    m_state.player->m_walking[m_state.player->UP] = new int[12] {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11};
    m_state.player->m_walking[m_state.player->DOWN] = new int[12] {13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 22};
    m_state.player->m_walking[m_state.player->RIGHT] = new int[12] { 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};
    m_state.player->m_walking[m_state.player->LEFT] = new int[12] {35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46};



    m_state.player->m_animation_indices = m_state.player->m_walking[m_state.player->RIGHT];
    m_state.player->m_animation_frames = 1;
    m_state.player->m_animation_index = 0;
    m_state.player->m_animation_time = 0.0f;
    m_state.player->m_animation_cols = 6;
    m_state.player->m_animation_rows = 8;
    m_state.player->set_height(0.6f);
    m_state.player->set_width(0.4f);





    m_state.keys = new Entity[KEYS];
    m_state.keys[0].set_position(glm::vec3(7.0f, -3.0f, 0.0f));
    m_state.keys[1].set_position(glm::vec3(1.0f, -1.0f, 0.0f));
    //m_state.keys[2].set_position(glm::vec3(14.0f, -1.0f, 0.0f));




    for (int i = 0; i < KEYS; i++) {

        m_state.keys[i].set_entity_type(KEY);
        m_state.keys[i].m_texture_id = Utility::load_texture("Rune.png");
        m_state.keys[i].m_animation_indices = new int[4] {0, 1, 2, 3};
        m_state.keys[i].m_animation_cols = 4;
        m_state.keys[i].m_animation_rows = 1;
        m_state.keys[i].m_animation_frames = 4;
        m_state.keys[i].m_animation_index = 0;
        m_state.keys[i].m_animation_time = 0.0f;
        m_state.fake_map->get_key.push_back(false);
    }

    m_state.box = new Entity();
    m_state.box->set_entity_type(BOX);
    m_state.box->set_position(glm::vec3(9.0, -6.0, 0.0));
    m_state.box->m_texture_id = Utility::load_texture("Chest.png");
    m_state.box->m_animation_frames = 4;
    m_state.box->m_animation_cols = 4;
    m_state.box->m_animation_rows = 1;
    m_state.box->m_animation_indices = new int[4] {0, 1, 2, 3};
    m_state.box->m_animation_index = 0;
    m_state.box->m_animation_time = 0.0f;




    /**
     Enemies' stuff */

    m_state.enemies = new Entity[ENEMY_COUNT];
    m_state.enemies[0].set_entity_type(ENEMY);
    m_state.enemies[0].set_ai_type(GUARD);
    m_state.enemies[0].set_ai_state(PATROLLING);

    m_state.enemies[0].set_position(glm::vec3(9.0f, -1.0f, 0.0f));
    m_state.enemies[0].set_movement(glm::vec3(0.0f));
    m_state.enemies[0].set_speed(4.0f);
    m_state.enemies[0].set_acceleration(glm::vec3(0.0f, -5.0f, 0.0f));
    GLuint enemy_texture_id = Utility::load_texture("Flight.png");
    m_state.enemies[0].m_texture_id = enemy_texture_id;
    m_state.enemies->m_walking[m_state.enemies->LEFT] = new int[8] {0, 1, 2, 3, 4, 5, 6, 7};
    m_state.enemies->m_walking[m_state.enemies->RIGHT] = new int[8] {8,9,10,11,12,13,14,15};
    m_state.enemies->m_animation_indices = m_state.enemies->m_walking[m_state.enemies->UP];
    m_state.enemies->m_animation_frames = 8;
    m_state.enemies->m_animation_index = 0;
    m_state.enemies->m_animation_time = 0.0f;
    m_state.enemies->m_animation_cols = 8;
    m_state.enemies->m_animation_rows = 2;
    m_state.enemies->set_hori_endpoints(9, 14);


    /**
     BGM and SFX
     */
    //Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 4096);

    //m_state.bgm = Mix_LoadMUS("Adventures in Adventureland.mp3");
    //Mix_PlayMusic(m_state.bgm, -1);
    //Mix_VolumeMusic(MIX_MAX_VOLUME / 4.0f);

    m_state.hit_sfx = Mix_LoadWAV("punch.wav");
}


void LevelC::update(float delta_time)
{
    //m_state.fake_map->m_animation_frames = 1;

    
    m_state.player->update(delta_time, m_state.player, m_state.enemies, ENEMY_COUNT, m_state.map, m_state.fake_map, COLLAPSE_BLOCKS);
    m_state.box->update(delta_time, m_state.player, 0, 0, m_state.map, m_state.fake_map, COLLAPSE_BLOCKS);
    for (int i = 0; i < KEYS;i++) {
        m_state.keys[i].update(delta_time, m_state.player, 0, 0, m_state.map, m_state.fake_map, COLLAPSE_BLOCKS);
        if (m_state.player->check_collision(&m_state.keys[i]))
        {
            m_state.fake_map->get_key[i] = true;
            m_state.keys[i].deactivate();
            //m_state.fake_map[i].m_animation_frames = 2;
        }
        if (m_state.fake_map->get_key[i] == true && m_state.player->check_collision(&m_state.fake_map[i]))
        {
            m_state.fake_map[i].deactivate();
        }
    }
    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].update(delta_time, m_state.player, 0, 0, m_state.map, m_state.fake_map, COLLAPSE_BLOCKS);
    }
    for (int i = 0;i < COLLAPSE_BLOCKS;i++)
    {
        //m_state.fake_map[i].m_animation_frames = 1;
        m_state.fake_map[i].update(delta_time, m_state.player, 0, 0, m_state.map, m_state.fake_map, COLLAPSE_BLOCKS);
        if (m_state.player->check_collision(&m_state.fake_map[i]))
        {
            m_state.fake_map[i].m_animation_frames = 2;
            m_state.fake_map->is_collided[i] = true;
        }
        if (i<=1&& m_state.fake_map->get_key[i] == false)
        {
            m_state.player->check_collision_x(&m_state.fake_map[i], COLLAPSE_BLOCKS);
            m_state.player->check_collision_y(&m_state.fake_map[i], COLLAPSE_BLOCKS);

        }

    }
    //if (m_state.player->get_position() == glm::vec3(14.0, -1.0, 0.0))
    //{
    //    m_state.enemies->set_movement(glm::vec3(1.0, 0.0, 0.0));
    //}


    //for (int i = 0; i < KEYS;i++)
    //{
    //    if (m_state.player->check_collision(&m_state.keys[i]))
    //    {
    //        m_state.fake_map->get_key[i] = true;
    //        m_state.keys[i].deactivate();
    //        //m_state.fake_map[i].m_animation_frames = 2;
    //    }
    //    if (m_state.fake_map->get_key[i] == true && m_state.player->check_collision(&m_state.fake_map[i]))
    //    {
    //        m_state.fake_map[i].deactivate();
    //    }
    //}
    //for (int i = 0; i < KEYS;i++)
    //{
    //    if (m_state.fake_map->get_key[i] == true && m_state.player->check_collision(&m_state.fake_map[i]))
    //    {
    //        m_state.fake_map[i].deactivate();
    //    }
    //}

    if (m_state.enemies[0].m_is_defeated)
    {
        m_state.fake_map[COLLAPSE_BLOCKS - 1].deactivate();
    }
    if (m_state.player->check_collision(m_state.box))
    {
        m_state.player->touch_chest = true;
    }
    if (m_state.player->touch_chest) m_state.player->win = true;

}


void LevelC::render(ShaderProgram* program)
{
    GLuint text_id = Utility::load_texture("font1.png");
    m_state.map->render(program);
    m_state.player->render(program);

    m_state.box->render(program);
    for (int i = 0;i < COLLAPSE_BLOCKS; i++) {
        m_state.fake_map[i].render(program);
    }
    for (int i = 0; i < KEYS;i++) {
        //if (m_state.fake_map->is_collided[i])
        //{
            m_state.keys[i].render(program);
        //}
    }

    for (int i = 0; i < ENEMY_COUNT; i++)
    {
        m_state.enemies[i].render(program);
    }

}
