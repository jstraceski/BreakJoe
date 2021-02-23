//
// Created by jibbo on 2/22/21.
//

#include <ResourceManager.h>
#include <string>
#include <LOpenGL.h>
#include <iostream>
#include <Game.h>
#include <Clip.h>
#include <cmath>
#include <fstream>
#include <streambuf>
#include <regex>


ResourceManager::ResourceManager() {

}

ResourceManager* ResourceManager::getInstance() {
    static ResourceManager instance;
    return &instance;
}


void ResourceManager::playSound(const std::string& key) {
    Clip * clip = soundLookup.at(key);
    clip->active = true;
    clip->trackPos = 0;
}

void ResourceManager::menuIncrement(int i) {
    menuIndex += i;
    if (menuIndex < 0) {
        menuIndex = menuOptions.size() - 1;
    } else if (menuIndex > menuOptions.size() - 1) {
        menuIndex = 0;
    }
}

void ResourceManager::loadLanguage(std::string path) {
    std::string keys[] = {"score", "lives", "you_win", "you_lose", "next_level", "level"};
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());
    std::string delimiter = "\n";

    int i = 0;

    std::stringstream ss(str);
    std::string to;

    while (std::getline(ss, to, '\n')) {
        messageLookup.insert({keys[i], to});
        ++i;
    }
}

void ResourceManager::loadLevel(std::string path) {
    std::ifstream t(path);
    std::string str((std::istreambuf_iterator<char>(t)),std::istreambuf_iterator<char>());

    int yIdx = 0;

    int pos;

    std::stringstream ss(str);
    std::string to;

    while (std::getline(ss, to, '\n')) {
        std::regex newlines_re("[\n\r ]+");
        to = std::regex_replace(to, newlines_re, "");

        float brickSpace = (float) Game::SCREEN_WIDTH - (float) (to.size() + 1) * (float) Game::BRICK_SPACING;
        float brickWidth = brickSpace / (float) to.size();

        float yPos = (float) Game::BRICK_TOP_OFFSET
                + (float) Game::BRICK_HEIGHT / 2.0f
                + (float) (Game::BRICK_SPACING * (yIdx + 1))
                + (float) (Game::BRICK_HEIGHT * yIdx);

        ++yIdx;
        for (int i = 0; i < to.size(); ++i) {
            int n = std::stoi(to.substr(i, 1));
            if (n > 0) {
                float xPos = (float) (Game::BRICK_SPACING * (i + 1))
                             + brickWidth / 2.0f
                             + (brickWidth * (float) i);

                Entity *e = new Entity();
                e->pos = Vector3D(xPos, (float) Game::SCREEN_HEIGHT - yPos, 0);
                e->width = brickWidth;
                e->height = (float) Game::BRICK_HEIGHT;
                e->typeId = 2;
                e->hits = n;
                entities.emplace_back(e);
            }
        }
    }

}

Clip* ResourceManager::loadSound(const char *path, const std::string& key) {
    Clip * clip = new Clip();
    if(SDL_LoadWAV(path, &clip->spec, &clip->data, &clip->clipLen) == NULL ){
        fprintf(stderr, "Could not open file: %s\n", SDL_GetError());
        return NULL;
    }

    soundLookup.insert({key, clip});
    Clip::sounds.emplace_back(clip);
    return clip;
}

void ResourceManager::clearLevel() {
    for (int i = 0; i < entities.size(); ++i) {
        Entity * entity = entities.at(i);
        if (entity->typeId == 2) {
            entities.erase(entities.begin() + i);
            --i;
            delete entity;
        }
    }
}

void ResourceManager::levelUpdate(bool win) {
    ballCaptured = true;
    ball->vel = Vector3D(0, 0, 0);
    clearLevel();

    if (win && levelId == levels.size() - 1) {
        pauseText = getText("you_win");
        pauseTimer = PAUSE_DELAY;
        end = true;
    } else if (win) {
        pauseText = getText("next_level");
        ++levelId;
        playerLives = 3;
        loadLevel(levels.at(levelId));
    } else {
        pauseText = getText("you_lose");
        score = 0;
        playerLives = 3;
        loadLevel(levels.at(levelId));
    }

    pauseTimer = PAUSE_DELAY;
}

int ResourceManager::loadFont(FT_Library ft, const char * path) {
    // MODIFIED from https://learnopengl.com/In-Practice/Text-Rendering
    FT_Face face;
    if (FT_New_Face(ft, path, 0, &face))
    {
        std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
        return false;
    }

    FT_Set_Pixel_Sizes(face, 48, 48);

    glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // disable byte-alignment restriction

    for (unsigned char c = 0; c < 255; c++)
    {
        // load character glyph
        if (FT_Load_Char(face, c, FT_LOAD_RENDER))
        {
            std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
            continue;
        }
        // generate texture
        unsigned int texture;
        glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_ALPHA,
                face->glyph->bitmap.width, face->glyph->bitmap.rows,
                0, GL_ALPHA, GL_UNSIGNED_BYTE,
                face->glyph->bitmap.buffer
        );
        // set texture options
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        // now store character for later use
        Glyph character = {
                texture,
                face->glyph->bitmap.width, face->glyph->bitmap.rows,
                face->glyph->bitmap_left, face->glyph->bitmap_top,
                face->glyph->advance.x
        };
        glyphLookup.insert(std::pair<char, Glyph>(c, character));
    }

    FT_Done_Face(face);

    return true;
}

void ResourceManager::menuFunction() {
    loadLanguage(menuLookup.at(menuOptions.at(menuIndex)));
    loadLevel(levels.at(levelId));
    menu = false;
}

int ResourceManager::startUp() {
    player = new Entity();
    player->pos = Vector3D(((float) Game::SCREEN_WIDTH) / 2.0f, ((float)Game::SCREEN_HEIGHT) * 1.0f / 5.0f, 0);
    player->width = 100;
    player->height = 10;

    ball = new Entity();
    ball->pos = Vector3D(((float) Game::SCREEN_WIDTH) / 2.0f, ((float)Game::SCREEN_HEIGHT) / 2.0f, 0);
    ball->shapeId = 1;
    ball->radius = 10;
    ball->drag = 1.0f;
    ball->reflects = true;
    ball->vel = Vector3D(2.0f, 2.0f, 0.0f);

    entities.emplace_back(player);
    entities.emplace_back(ball);


    Clip* background = loadSound("Assets/piano2.wav", "background");
    loadSound("Assets/beep2.wav", "hit");
    background->active = true;
    background->loop = true;

    SDL_AudioSpec fmt;

    fmt.freq = 44100;
    fmt.format = AUDIO_S16;
    fmt.channels = 2;
    fmt.samples = 4096;
    fmt.callback = Clip::callback;
    fmt.userdata = NULL;

    std::string s = "fran";
    char c = 231;
    s.push_back(c);
    s = s + "ais";

    menuLookup.insert({"english", "Assets/english.txt"});
    menuLookup.insert({s, "Assets/french.txt"});

    menuOptions.emplace_back("english");
    menuOptions.emplace_back(s);

    levels.emplace_back("Assets/level1.txt");
    levels.emplace_back("Assets/level2.txt");
    levels.emplace_back("Assets/level3.txt");

    /* Open the audio device */
    if ( SDL_OpenAudio(&fmt, NULL) < 0 ){
        fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
        return false;
    }

    /* Start playing */
    SDL_PauseAudio(0);

    return true;
}

void ResourceManager::updateKey(SDL_Keycode key, bool state) {
    auto it = keyState.find(key);
    if (it != keyState.end()) {
        it->second = state;
    } else {
        keyState.insert({key, state});
    }
}

bool ResourceManager::getKey(SDL_Keycode key) {
    auto it = keyState.find(key);
    if (it != keyState.end()) {
        return it->second;
    }
    return false;
}

std::string ResourceManager::getText(const std::string& key) {
    return messageLookup.at(key);
}

void ResourceManager::drawEntity(const Entity *e) {

    float xScale = 2.0f / (float) Game::SCREEN_WIDTH;
    float xShift = ((float) Game::SCREEN_WIDTH) / -2.0f;

    float yScale = 2.0f / (float) Game::SCREEN_HEIGHT;
    float yShift = ((float) Game::SCREEN_HEIGHT) / -2.0f;

    if (e->shapeId == 0) { // brick
        if (e->typeId == 2) {
            if (e->hits == 3) {
                glColor3f(181/255.0f, 250/255.0f, 255/255.0f);
            } else if (e->hits == 2) {
                glColor3f(255.0f/255.0f, 249/255.0f, 181/255.0f);
            } else if (e->hits == 1) {
                glColor3f(1.0f, 1.0f, 1.0f);
            } else {
                return;
            }
        } else {
            glColor3f(1.0f, 1.0f, 1.0f);
        }

        glBegin(GL_QUADS);
            glVertex2f((e->pos.x - e->width/2 + xShift) * xScale, (e->pos.y + e->height/2 + yShift) * yScale);
            glVertex2f((e->pos.x + e->width/2 + xShift) * xScale, (e->pos.y + e->height/2 + yShift) * yScale);
            glVertex2f((e->pos.x + e->width/2 + xShift) * xScale, (e->pos.y - e->height/2 + yShift) * yScale);
            glVertex2f((e->pos.x - e->width/2 + xShift) * xScale, (e->pos.y - e->height/2 + yShift) * yScale);
        glEnd();
    } else {
        int div = 16;
        float ang = 360 / (float) div;

        glBegin(GL_TRIANGLE_FAN);
        glVertex2f((e->pos.x + xShift) * xScale, (e->pos.y + yShift) * yScale);

        for (int i = 0; i <= div; i++) {
            float divAng = (ang * (float) i);
            float xpos = e->pos.x + e->radius * sinf(divAng / 180 * M_PIf32) + xShift;
            float ypos = e->pos.y + e->radius * cosf(divAng / 180 * M_PIf32) + yShift;
            glVertex2f(xpos * xScale, ypos * yScale);
        }

        glEnd();
    }
}


void ResourceManager::drawText(const std::string& text, const Vector3D& pos, float scale, int alignment) {
    float xScale = 2.0f / (float) Game::SCREEN_WIDTH;
    float xShift = ((float) Game::SCREEN_WIDTH) / -2.0f;

    float yScale = 2.0f / (float) Game::SCREEN_HEIGHT;
    float yShift = ((float) Game::SCREEN_HEIGHT) / -2.0f;

    float x = pos.x;
    float y = pos.y;
    float textShift = 0;
    float textWidth = 0;

    glEnable(GL_TEXTURE_2D);

    if (alignment > 0) {
        std::string::const_iterator c;
        for (c = text.begin(); c != text.end(); c++) {
            Glyph ch = glyphLookup[*c];
            textWidth += (float) (ch.advance >> 6) * scale;
        }
        if (alignment == 1) {
            textShift = (int) (-textWidth/2.0f);
        } else {
            textShift = (int) (-textWidth);
        }
    }

    // MODIFIED from https://learnopengl.com/In-Practice/Text-Rendering
    std::string::const_iterator c;
    for (c = text.begin(); c != text.end(); c++)
    {
        Glyph ch = glyphLookup[*c];

        float xpos = x + (float) ch.x * scale + textShift;
        float ypos = y - (float) (ch.height - ch.y) * scale;

        float w = (float) ch.width * scale;
        float h = (float) ch.height * scale;

        glBindTexture(GL_TEXTURE_2D, ch.texId);

        float uScale = 1.0f;
        float vScale = 1.0f;

        glBegin(GL_QUADS);
        glTexCoord2f(0,1.0*vScale);
        glVertex2f((xpos + xShift) * xScale, (ypos + yShift) * yScale);

        glTexCoord2f(1.0*uScale,1.0*vScale);
        glVertex2f((xpos + w + xShift) * xScale, (ypos + yShift) * yScale);


        glTexCoord2f(1.0*uScale,0);
        glVertex2f((xpos + w + xShift) * xScale, (ypos + h + yShift) * yScale);

        glTexCoord2f(0,0);
        glVertex2f((xpos + xShift) * xScale, (ypos + h + yShift) * yScale);

        glEnd();

        x += (float) (ch.advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
    }
    glBindTexture(GL_TEXTURE_2D, 0);
    glDisable(GL_TEXTURE_2D);
}


int ResourceManager::shutDown() {
    clearLevel();
    delete player;
    delete ball;
    entities.clear();

    for(auto & it : glyphLookup) {
        glDeleteTextures(1, &it.second.texId);
    }

    for(Clip* clip : Clip::sounds) {
        delete clip;
    }

    return true;
}