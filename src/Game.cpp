//
// Created by jibbo on 2/22/21.
//

#include <Game.h>
#include <stdio.h>
#include <iostream>
#include <ResourceManager.h>

// Prints information about our program.
// This is useful for debugging.
void printProgramLog(GLuint program) {
    //Make sure name is shader
    if (glIsProgram(program)) {
        //Program log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetProgramInfoLog(program, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0) {
            //Print Log
            printf("%s\n", infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    }
    else {
        printf("Name %d is not a program\n", program);
    }
}

// Prints information about our shaders.
// This is useful for debugging.
void printShaderLog(GLuint shader) {
    //Make sure name is shader
    if (glIsShader(shader)) {
        //Shader log length
        int infoLogLength = 0;
        int maxLength = infoLogLength;

        //Get info string length
        glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);

        //Allocate string
        char* infoLog = new char[maxLength];

        //Get info log
        glGetShaderInfoLog(shader, maxLength, &infoLogLength, infoLog);
        if (infoLogLength > 0) {
            //Print Log
            printf("%s\n", infoLog);
        }

        //Deallocate string
        delete[] infoLog;
    }
    else {
        printf("Name %d is not a shader\n", shader);
    }
}


int Game::SCREEN_WIDTH = 1080;
int Game::SCREEN_HEIGHT = 720;
float Game::PADDLE_SPEED = 0.8f;
float Game::MAX_SPEED = 10.0f;
int Game::BRICK_TOP_OFFSET = 80;
int Game::BRICK_HEIGHT = 20;
int Game::BRICK_SPACING = 5;

Game::Game() {

}

bool Game::init() {
    printf("Starting init\n");
    //Initialize SDL
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Use OpenGL 3.3 core
//		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
//		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    //Create window
    gWindow = SDL_CreateWindow("Break Joe",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               SCREEN_WIDTH,
                               SCREEN_HEIGHT,
                               SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);
    // If we fail to create a window, then record some error
    // information.
    if (gWindow == NULL) {
        printf("Window could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    //Create context
    gContext = SDL_GL_CreateContext(gWindow);
    if (gContext == NULL) {
        printf("OpenGL context could not be created! SDL Error: %s\n", SDL_GetError());
        return false;
    }

    // Initialize GLAD Library
    gladLoadGLLoader(SDL_GL_GetProcAddress);
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("Shading language: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

    //Use Vsync
    if (SDL_GL_SetSwapInterval(1) < 0) {
        printf("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());
    }


    if (SDL_Init(SDL_INIT_AUDIO) < 0) {
        return false;
    }



    printf("Starting GL\n");

    //Initialize OpenGL
    if (!initGL()) {
        printf("Unable to initialize OpenGL!\n");
        return false;
    }

    printf("init Success\n");

    return true;
}

bool Game::initGL() {
    //Generate program
    gProgramID = glCreateProgram();

    //Create vertex shader
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);

    //Get vertex source
    const GLchar* vertexShaderSource[] = {
            "#version 140\n"
            "void main() {\n"
            "\t//Process vertex\n"
            "\tgl_Position = gl_ProjectionMatrix * gl_ModelViewMatrix * gl_Vertex;\n"
            "}"
    };

    //Set vertex source
    glShaderSource(vertexShader, 1, vertexShaderSource, NULL);

    //Compile vertex source
    glCompileShader(vertexShader);

    //Check vertex shader for errors
    GLint vShaderCompiled = GL_FALSE;
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &vShaderCompiled);
    if (vShaderCompiled != GL_TRUE) {
        printf("Unable to compile vertex shader %d!\n", vertexShader);
        printShaderLog(vertexShader);
        return false;
    }

    //Attach vertex shader to program
    glAttachShader(gProgramID, vertexShader);

    //Create fragment shader
    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);

    //Get fragment source
    const GLchar* fragmentShaderSource[] = {
            "#version 140\nout vec4 LFragment; void main() { LFragment = vec4( 1.0, 1.0, 1.0, 1.0 ); }"
    };

    //Set fragment source
    glShaderSource(fragmentShader, 1, fragmentShaderSource, NULL);

    //Compile fragment source
    glCompileShader(fragmentShader);

    //Check fragment shader for errors
    GLint fShaderCompiled = GL_FALSE;
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &fShaderCompiled);
    if (fShaderCompiled != GL_TRUE) {
        printf("Unable to compile fragment shader %d!\n", fragmentShader);
        printShaderLog(fragmentShader);
        return false;
    }

    //Attach fragment shader to program
    glAttachShader(gProgramID, fragmentShader);

    //Link program
    glLinkProgram(gProgramID);

    //Check for errors
    GLint programSuccess = GL_TRUE;
    glGetProgramiv(gProgramID, GL_LINK_STATUS, &programSuccess);
    if (programSuccess != GL_TRUE) {
        printf("Error linking program %d!\n", gProgramID);
        printProgramLog(gProgramID);
        return false;
    }

    printf("GL Success\n");

    if (!loadMedia()) {
        printf("Could not load media: \n");
        return false;
    }

    return true;
}

bool Game::loadMedia() {
    printf("Loading Media\n");
    ilInit();
    iluInit();
    ilClearColour( 255, 255, 255, 000 );

    //Enable texturing
    glEnable( GL_TEXTURE_2D );

    //Set blending
    glEnable( GL_BLEND );
    glDisable( GL_DEPTH_TEST );
    glBlendFunc( GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA );

    //Check for error
    ILenum ilError = ilGetError();
    if( ilError != IL_NO_ERROR )
    {
        printf( "Error initializing DevIL! ");
        return false;
    }

    FT_Library ft;
    if (FT_Init_FreeType(&ft))
    {
        std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
        return false;
    }

    ResourceManager *rm = ResourceManager::getInstance();
    if (!rm->loadFont(ft, "Assets/SGK100.ttf")) {
        printf("Font Failed to Load\n");
        return false;
    }

    FT_Done_FreeType(ft);

    glClearColor(0.f, 0.f, 0.f, 1.f);

    printf("Media Success\n");
    return true;
}


void Game::handleKey(SDL_Keycode key, bool down) {
    ResourceManager * rm = ResourceManager::getInstance();
    rm->updateKey(key, down);

    if (rm->menu) {
        if (rm->getKey(SDLK_a) && down) {
            rm->menuIncrement(-1);
        }

        if (rm->getKey(SDLK_d) && down) {
            rm->menuIncrement(1);
        }

        if (rm->getKey(SDLK_RETURN) && down) {
            rm->menuFunction();
        }
    }
}

void Game::input() {
    ResourceManager * rm = ResourceManager::getInstance();
    if (rm->getKey(SDLK_a)) {
        rm->player->vel.x -= PADDLE_SPEED;
    }

    if (rm->getKey(SDLK_d)) {
        rm->player->vel.x += PADDLE_SPEED;
    }

    if (rm->getKey(SDLK_q)) {
        quit = true;
    }

    if (rm->getKey(SDLK_SPACE) && rm->ballCaptured) {
        rm->ball->vel = rm->shootVector + rm->player->vel;
        rm->ballCaptured = false;
    }
}


void Game::update() {
    ResourceManager * rm = ResourceManager::getInstance();

    if (rm->pauseTimer > 0 && !rm->end) {
        rm->pauseTimer -= (float) SCREEN_TICKS_PER_FRAME / 1000.0f;
        return;
    }

    if (rm->menu) {
        return;
    }

    if (rm->ball->pos.y < rm->player->pos.y - rm->player->height/2) {
        rm->playerLives -= 1;
        rm->ballCaptured = true;
    }

    if (rm->ballCaptured) {
        rm->ball->pos = rm->player->pos + Vector3D(0, 10, 0);
        rm->ball->vel.x = 0;
        rm->ball->vel.y = 0;
        rm->ball->vel.z = 0;
    }

    for (Entity * entity : rm->entities) {
        entity->vel *= entity->drag;

        if (Magnitude(entity->vel) > MAX_SPEED) {
            entity->vel = Normalize(entity->vel) * MAX_SPEED;
        }

        entity->f_pos = entity->pos + entity->vel;

        Vector3D* normal = NULL;
        if (entity->shapeId == 0) {
            if (entity->f_pos.x + entity->width / 2.0f > (float) Game::SCREEN_WIDTH) {
                entity->f_pos.x = (float) Game::SCREEN_WIDTH - entity->width / 2.0f;
                normal = new Vector3D(-1, 0, 0);
            }

            if (entity->f_pos.x - entity->width / 2.0f  < 0) {
                entity->f_pos.x = entity->width / 2.0f;
                normal = new Vector3D(1, 0, 0);
            }

            if (entity->f_pos.y + entity->height > (float) Game::SCREEN_HEIGHT) {
                entity->f_pos.y = (float) Game::SCREEN_HEIGHT - entity->height;
                normal = new Vector3D(0, -1, 0);
            }

            if (entity->f_pos.y - entity->height / 2.0f  < 0) {
                entity->f_pos.y = entity->height / 2.0f;
                normal = new Vector3D(0, 1, 0);
            }
        } else {
            if (entity->f_pos.x + entity->radius > (float) Game::SCREEN_WIDTH) {
                entity->f_pos.x = (float) Game::SCREEN_WIDTH - entity->radius;
                normal = new Vector3D(-1, 0, 0);
            }

            if (entity->f_pos.x - entity->radius < 0) {
                entity->f_pos.x = entity->radius;
                normal = new Vector3D(1, 0, 0);
            }

            if (entity->f_pos.y + entity->radius > (float) Game::SCREEN_HEIGHT) {
                entity->f_pos.y = (float) Game::SCREEN_HEIGHT - entity->radius;
                normal = new Vector3D(0, -1, 0);
            }

            if (entity->f_pos.y - entity->radius < 0) {
                entity->f_pos.y = entity->radius;
                normal = new Vector3D(0, 1, 0);
            }
        }

        if (normal != NULL) {
            if (entity->reflects) {
                entity->vel -= Project(entity->vel, *normal) * 2;
            } else {
                entity->vel -= Project(entity->vel, *normal);
            }
        }
    }

    for (int i = 0; i < rm->entities.size(); ++i) {
        Entity * a = rm->entities.at(i);

        for (int j = i; j < rm->entities.size(); ++j) {
            Entity * b = rm->entities.at(j);

            if (a->shapeId != b->shapeId && a->active && b->active) {
                Entity * rect = a;
                Entity * ball = b;
                if (b->shapeId == 0) {
                    rect = b;
                    ball = a;
                }

                Vector3D topLeft = rect->f_pos + Vector3D(-rect->width / 2.0f, rect->height / 2.0f, 0);
                Vector3D topRight = rect->f_pos + Vector3D(rect->width / 2.0f, rect->height / 2.0f, 0);
                Vector3D bottomRight = rect->f_pos + Vector3D(rect->width / 2.0f, -rect->height / 2.0f, 0);
                Vector3D bottomLeft = rect->f_pos + Vector3D(-rect->width / 2.0f, -rect->height / 2.0f, 0);

                Vector3D top = PointToLine(ball->f_pos, topLeft, topRight);
                Vector3D right = PointToLine(ball->f_pos, topRight, bottomRight);
                Vector3D bottom = PointToLine(ball->f_pos, bottomRight, bottomLeft);
                Vector3D left = PointToLine(ball->f_pos, bottomLeft, topLeft);

                Vector3D points[] = {top, right, bottom, left};

                float min = -1;
                Vector3D closest = Vector3D(0, 0, 0);
                for (const Vector3D& point : points) {
                    float val = MagnitudeSqr(point - ball->f_pos);
                    if (min == -1 || val < min) {
                        min = val;
                        closest.x = point.x;
                        closest.y = point.y;
                    }
                }

                if (sqrtf(min) < ball->radius && !rm->ballCaptured) {
                    rm->playSound("hit");
                    Vector3D toBall = ball->f_pos - closest;
                    Vector3D normal = Normalize(toBall);

                    ball->f_pos = closest + normal * (ball->radius * 1.1f);

                    if (rect->typeId == 0) {
                        normal = Normalize(ball->f_pos - (rect->f_pos + Vector3D(0, -rect->height * 10, 0)));
                    }

                    if (Dot(normal, ball->vel) < 0) {
                        ball->vel -= Project(ball->vel, normal) * 2;
                    } else {
                        ball->vel += (normal * Magnitude(rect->vel));
                    }
                    ball->vel += rect->vel * 0.5;

                    if (rect->typeId == 2) {
                        rm->score += rect->hits;
                        rect->hits -= 1;
                        if (rect->hits == 0) {
                            rect->active = false;
                        }
                    }
                }
            }
        }
    }

    bool bricksLeft = false;
    for (Entity * entity : rm->entities) {
        entity->pos = entity->f_pos;
        if (entity->typeId == 2 && entity->active && entity->hits > 0) {
            bricksLeft = true;
        }
    }



    if (!bricksLeft) {
        rm->levelUpdate(true);
    }

    if (rm->playerLives == 0) {
        rm->levelUpdate(false);
    }
}

void Game::render() {
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glLoadIdentity();
    glColor3f(1.0f, 1.0f, 1.0f);



    ResourceManager * rm = ResourceManager::getInstance();

    if (rm->pauseTimer > 0) {
        rm->drawText(rm->pauseText, Vector3D((float) SCREEN_WIDTH / 2.0f, (float) SCREEN_HEIGHT/2.0f, 0), 3.0f, 1);
    } else if (rm->menu) {
        int idx = 0;
        int div = SCREEN_WIDTH / rm->menuOptions.size();
        for (std::string option : rm->menuOptions){
            float scale = (idx == rm->menuIndex) ? 2.0f : 1.0f;
            rm->drawText(option, Vector3D(div/2 + div * idx, (float) SCREEN_HEIGHT/2.0f, 0), scale, 1);
            ++idx;
        }

    } else {
        for (Entity *entity : rm->entities) {
            ResourceManager::drawEntity(entity);
        }

        glColor3f(1.0f, 1.0f, 1.0f);

        rm->drawText(rm->getText("score") + " " + std::to_string(rm->score),
                     Vector3D(20, (float) SCREEN_HEIGHT - 40.0f, 0), 1.0f, 0);
        rm->drawText(rm->getText("level") + " " + std::to_string(rm->levelId + 1),
                     Vector3D((float) SCREEN_WIDTH / 2.0f, (float) SCREEN_HEIGHT - 40.0f, 0), 1.0f, 1);
        rm->drawText(rm->getText("lives") + " " + std::to_string(rm->playerLives),
                     Vector3D((float) SCREEN_WIDTH - 20, (float) SCREEN_HEIGHT - 40.0f, 0), 1.0f, 2);
    }
}

void Game::sync() {
    afterTick = fpsTimer.getTicks();
    int timeDiff = afterTick - beforeTick;
    if (timeDiff < SCREEN_TICKS_PER_FRAME) {
        //Wait remaining time
        SDL_Delay(SCREEN_TICKS_PER_FRAME - timeDiff);
    }

    beforeTick = fpsTimer.getTicks();
    int extra = ((SCREEN_TICKS_PER_FRAME - timeDiff) + afterTick) - beforeTick;
    beforeTick = beforeTick - extra;
}


void Game::run() {
    printf("Running\n");

    ResourceManager * rm = ResourceManager::getInstance();
    rm->startUp();

    fpsTimer.start();
    rm->menu = true;
    rm->ballCaptured = true;

    //Event handler
    SDL_Event e;

    //Enable text input
    SDL_StartTextInput();
    printf("Entering Game Loop\n");

    //While application is running
    while (!quit) {
        //Handle events on queue
        while (SDL_PollEvent(&e) != 0) {
            //User requests quit
            if (e.type == SDL_QUIT) {
                quit = true;
            }

            //Handle keypress with current mouse position
            if (e.type == SDL_KEYDOWN) {
                handleKey(e.key.keysym.sym, true);
            }

            if (e.type == SDL_KEYUP) {
                handleKey(e.key.keysym.sym, false);
            }
        }

        input();
        update();
        render();

        //Update screen
        SDL_GL_SwapWindow(gWindow);
        sync();
    }

    //Disable text input
    SDL_StopTextInput();

    //Free resources and close SDL
    close();
}

void Game::close() {
    ResourceManager *rm = ResourceManager::getInstance();
    rm->shutDown();

    //Deallocate program
    glDeleteProgram(gProgramID);

    //Destroy window
    SDL_DestroyWindow(gWindow);
    gWindow = NULL;

    //Quit SDL subsystems
    SDL_Quit();
}
