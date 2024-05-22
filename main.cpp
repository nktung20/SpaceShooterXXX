#include "mainFunc.h"

#include "Player.h"
#include "Enemy.h"
#include "Explosion.h"

Objects g_background;
Objects g_background2;
Objects g_menu;
Objects g_death;
Objects g_death_menu;
Objects g_pause;

Mix_Chunk *gMusic = NULL;
Mix_Chunk *enemy_expolosion_sound = NULL;
Mix_Chunk *player_explosion_sound = NULL;
Mix_Chunk *enemy_shooting_sound = NULL;
Mix_Chunk *player_shooting_sound = NULL;
Mix_Chunk *player_death_sound = NULL;

TTF_Font *gFont = NULL;

bool loadProperty()
{
    bool ret = g_background.loadImg("assets/background.jpg", g_screen);
    g_background2.loadImg("assets/background.jpg", g_screen);
    g_menu.loadImg("assets/menu.png", g_screen);
    g_death.loadImg("assets/death.png", g_screen);
    g_death_menu.loadImg("assets/death_menu.png", g_screen);
    g_pause.loadImg("assets/pause.png", g_screen);

    gMusic = Mix_LoadWAV("assets/space-asteroids.wav");
    player_shooting_sound = Mix_LoadWAV("assets/shot.wav");
    enemy_expolosion_sound = Mix_LoadWAV("assets/explosion.wav");
    player_explosion_sound = Mix_LoadWAV("assets/p_explosion.wav");
    player_death_sound = Mix_LoadWAV("assets/deathSound.wav");

    gFont = TTF_OpenFont("assets/Galaxus.ttf", 28);

    if (gFont == NULL)
        return false;

    if (gMusic == NULL || player_shooting_sound == NULL || enemy_expolosion_sound == NULL)
        return false;

    if (ret == false)
        return false;

    return true;
}
void closeMusic()
{
    TTF_CloseFont(gFont);
    gFont = NULL;
    if (gMusic != NULL)
    {
        Mix_FreeChunk(gMusic);
        gMusic = NULL;
    }
    if (enemy_expolosion_sound != NULL)
    {
        Mix_FreeChunk(enemy_expolosion_sound);
        enemy_expolosion_sound = NULL;
    }
    if (player_shooting_sound != NULL)
    {
        Mix_FreeChunk(player_shooting_sound);
        player_shooting_sound = NULL;
    }
}

int readHighScore()
{
    std::ifstream file("highScore.txt");
    int highScore = 0;
    if (file.is_open())
    {
        file >> highScore;
        file.close();
    }
    return highScore;
}
void writeHighScore(int h)
{
    std::ofstream file("highScore.txt");
    if (file.is_open())
    {
        file << h;
        file.close();
    }
}

void renderText(const std::string &text, int x, int y)
{
    SDL_Color textColor = {255, 255, 255, 255};
    SDL_Surface *textsurface = TTF_RenderText_Solid(gFont, text.c_str(), textColor);
    if (textsurface != NULL)
    {
        SDL_Texture *mTexture = SDL_CreateTextureFromSurface(g_screen, textsurface);
        SDL_FreeSurface(textsurface);
        if (mTexture != NULL)
        {
            SDL_Rect renderQuad = {x, y, textsurface->w, textsurface->h};
            SDL_RenderCopy(g_screen, mTexture, NULL, &renderQuad);
            SDL_DestroyTexture(mTexture);
        }
    }
}
int main(int argc, char *argv[])
{
    bool playAgain = true;
    bool is_quit = false;
    int highScore = readHighScore();
    if (init() == false)
        return -1;

    while (playAgain && !is_quit)
    {
        int x = 1100;
        double v = 4;
        double bullet_v = 5;
        int kill = 0;
        if (loadProperty() == false)
            return -1;
        srand(time(NULL));
        Mix_VolumeChunk(player_shooting_sound, 32);
        Mix_VolumeChunk(player_death_sound, 64);
        Mix_PlayChannel(-1, gMusic, -1);
        bool isInMenu = true;
        while (isInMenu && !is_quit)
        {
            SDL_RenderClear(g_screen);
            g_menu.renderTexture(g_screen, NULL);
            SDL_RenderPresent(g_screen);
            while (SDL_PollEvent(&g_event) != 0)
            {
                if (g_event.type == SDL_QUIT)
                {

                    isInMenu = false;
                    is_quit = true;
                }
                if (g_event.type == SDL_KEYDOWN && g_event.key.keysym.sym == SDLK_p)
                {
                    g_menu.free();
                    isInMenu = false;
                }
            }
        }
        Player p_player;
        p_player.loadImg("assets/player.png", g_screen);
        Uint64 startTime = SDL_GetTicks64();

        std::vector<Enemy *> enemies;
        std::vector<Enemy *> bosses;
        std::vector<Bullet *> e_bullets;
        std::vector<Bullet *> b_bullets;
        std::vector<Explosion *> e_explosions;
        std::vector<Explosion *> b_explosions;
        int backgroundScrollSpeed = 2;
        g_background.setRect(g_background.getRect().x, 0);                           // background 1 ở đầu
        g_background2.setRect(g_background2.getRect().x, -g_background.getRect().h); // background 2 ở trên background 1

        bool gameOver = false;
        bool pause = false;

        while (!is_quit && !gameOver)
        {
            while (SDL_PollEvent(&g_event) != 0)
            {
                if (g_event.type == SDL_QUIT)
                {
                    is_quit = true;
                }
                p_player.handleInput(g_event, g_screen);
                if (p_player.value == 1)
                    Mix_PlayChannel(-1, player_shooting_sound, 0);

                if (g_event.type == SDL_KEYDOWN)
                {
                    if (g_event.key.keysym.sym == SDLK_s)
                    {
                        pause = true;
                        Mix_Pause(-1);
                    }
                }
            }

            if (pause)
            {
                g_pause.setRect(386, 216);
                g_pause.renderTexture(g_screen);
                SDL_RenderPresent(g_screen);

                while (pause)
                {
                    while (SDL_PollEvent(&g_event) != 0)
                    {
                        if (g_event.type == SDL_QUIT)
                        {
                            is_quit = true;
                            pause = false;
                        }
                        if (g_event.type == SDL_KEYDOWN && g_event.key.keysym.sym == SDLK_e)
                        {
                            pause = false;
                            Mix_Resume(-1);
                        }
                    }
                }
            }
            if (pause == false)
            {
                SDL_SetRenderDrawColor(g_screen, 255, 255, 255, 255);
                SDL_RenderClear(g_screen);
                g_background.setRect(g_background.getRect().x, g_background.getRect().y + backgroundScrollSpeed);
                g_background2.setRect(g_background2.getRect().x, g_background2.getRect().y + backgroundScrollSpeed);

                // nếu 1 ra khỏi màn hình, reset vị trí lên trên 2
                if (g_background.getRect().y >= g_background.getRect().h)
                {
                    g_background.setRect(g_background.getRect().x, g_background2.getRect().y - g_background.getRect().h);
                }

                // nếu 2 ra khỏi màn hình, reset vị trí lên trên 1
                if (g_background2.getRect().y >= g_background2.getRect().h)
                {
                    g_background2.setRect(g_background2.getRect().x, g_background.getRect().y - g_background2.getRect().h);
                }

                // Render cả 2
                g_background.renderTexture(g_screen);
                g_background2.renderTexture(g_screen);

                p_player.renderTexture(g_screen);
                p_player.handleBullet(g_screen);
                p_player.movePlayer();
                p_player.Update();
                float player_pos_x = p_player.getRect().x;
                float player_pos_y = p_player.getRect().y;

                if (kill <= 15)
                {
                    for (int i = 0; i < int(enemies.size()); i++)
                    {
                        enemies[i]->handleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                        if (checkCollision(enemies[i]->getRect(), p_player.getRect()) == true)
                        {
                            p_player.free();
                            Mix_HaltChannel(-1);
                            Mix_PlayChannel(-1, player_explosion_sound, 0);
                            Mix_PlayChannel(-1, player_death_sound, 0);

                            Uint64 _deathTime = SDL_GetTicks64();
                            while (SDL_GetTicks64() - _deathTime <= 2000)
                            {
                                g_death.renderTexture(g_screen, NULL);
                                SDL_RenderPresent(g_screen);
                            }
                            gameOver = true;
                            continue;
                        }
                        if (p_player.checkBulletToEnemy(enemies[i]->getRect()) == true)
                        {
                            Explosion::createExplosion(enemies[i]->getRect().x, enemies[i]->getRect().y, e_explosions);

                            Mix_PlayChannel(-1, enemy_expolosion_sound, 0);
                            enemies[i]->free();
                            enemies.erase(enemies.begin() + i);
                            i--;
                            kill++;
                            bullet_v += 0.05;
                            x -= 10;
                            continue;
                        }
                        enemies[i]->createBullet(SCREEN_WIDTH, SCREEN_HEIGHT, g_screen, player_pos_x, player_pos_y, e_bullets, bullet_v);
                        if (enemies[i]->get_pos_y() > SCREEN_HEIGHT)
                        {
                            enemies.erase(enemies.begin() + i);
                            i--;
                            continue;
                        }
                    }
                }
                else if (kill > 15)
                {
                    for (int i = 0; i < int(bosses.size()); i++)
                    {
                        bosses[i]->handleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                        if (checkCollision(bosses[i]->getRect(), p_player.getRect()) == true)
                        {
                            p_player.free();
                            Mix_HaltChannel(-1);
                            Mix_PlayChannel(-1, player_explosion_sound, 0);
                            Mix_PlayChannel(-1, player_death_sound, 0);
                            Uint64 _deathTime = SDL_GetTicks64();
                            while (SDL_GetTicks64() - _deathTime <= 2000)
                            {
                                g_death.renderTexture(g_screen, NULL);
                                SDL_RenderPresent(g_screen);
                            }
                            gameOver = true;
                            continue;
                        }
                        if (p_player.checkBulletToEnemy(bosses[i]->getRect()) == true)
                        {
                            Mix_PlayChannel(-1, enemy_expolosion_sound, 0);
                            Explosion::createBossExplosion(bosses[i]->getRect().x, bosses[i]->getRect().y, b_explosions);
                            bosses[i]->free();
                            bosses.erase(bosses.begin() + i);
                            i--;
                            kill++;
                            if (kill <= 30)
                            {
                                bullet_v += 0.05;
                                x -= 10;
                            }
                            else if (30 < kill <= 45 && x >= 300)
                            {
                                bullet_v += 0.1;
                                x -= 15;
                            }
                            else if (45 < kill <= 60 && x >= 300)
                            {
                                bullet_v += 0.2;
                                x -= 20;
                            }
                            continue;
                        }
                        bosses[i]->B_createBullet(SCREEN_WIDTH, SCREEN_HEIGHT, g_screen, player_pos_x, player_pos_y, b_bullets, bullet_v);
                        if (bosses[i]->get_pos_y() > SCREEN_HEIGHT)
                        {
                            bosses.erase(bosses.begin() + i);
                            i--;
                            continue;
                        }
                    }
                }
                int flag = 1;

                if ((SDL_GetTicks64() - startTime) >= x)
                {
                    if (kill <= 15)
                    {
                        int x = rand() % (SCREEN_WIDTH - Enemy_width);
                        Enemy *newEnemy = new Enemy(x, v);
                        newEnemy->loadImg("assets/enemy.png", g_screen);
                        enemies.push_back(newEnemy);
                        startTime = SDL_GetTicks64();
                    }
                    else if (kill > 15)
                    {
                        x = 900;
                        Uint64 waitTime = SDL_GetTicks64();
                        enemies.clear();
                        if (flag == 1)
                        {
                            if (SDL_GetTicks64() - waitTime >= 5000)
                            {
                                flag = 0;
                                continue;
                            }
                        }
                        int x = rand() % (SCREEN_WIDTH - Boss_width);
                        Enemy *newEnemy = new Enemy(x, v);
                        newEnemy->loadImg("assets/boss.png", g_screen);
                        bosses.push_back(newEnemy);
                        startTime = SDL_GetTicks64();
                    }
                }
                for (auto enemy : enemies)
                {
                    enemy->renderTexture(g_screen);
                }
                for (auto enemy : bosses)
                {
                    enemy->renderTexture(g_screen);
                }
                for (int i = 0; i < int(e_bullets.size()); i++)
                {
                    Bullet *e_bullet = e_bullets.at(i);
                    if (e_bullet != NULL)
                    {
                        if (e_bullet->getmoving() == true)
                        {
                            e_bullet->handleEnemy(SCREEN_WIDTH, SCREEN_HEIGHT);
                            if (checkCollision(e_bullets[i]->getRect(), p_player.getRect()))
                            {
                                p_player.free();
                                Mix_HaltChannel(-1);
                                Mix_PlayChannel(-1, player_explosion_sound, 0);
                                Mix_PlayChannel(-1, player_death_sound, 0);
                                Uint64 deathTime = SDL_GetTicks64();
                                while (SDL_GetTicks64() - deathTime <= 2000)
                                {
                                    g_death.renderTexture(g_screen, NULL);
                                    SDL_RenderPresent(g_screen);
                                }
                                gameOver = true;
                            }
                            e_bullet->renderTexture(g_screen);
                        }
                        else
                        {
                            e_bullets.erase(e_bullets.begin() + i);
                            if (e_bullet != NULL)
                            {
                                e_bullet = NULL;
                            }
                        }
                    }
                }
                for (int i = 0; i < int(b_bullets.size()); i++)
                {
                    Bullet *b_bullet = b_bullets.at(i);
                    if (b_bullet != NULL)
                    {
                        if (b_bullet->getmoving() == true)
                        {

                            b_bullet->handleEnemy(SCREEN_WIDTH, SCREEN_HEIGHT);
                            if (checkCollision(b_bullets[i]->getRect(), p_player.getRect()))
                            {
                                p_player.free();
                                Mix_HaltChannel(-1);
                                Mix_PlayChannel(-1, player_explosion_sound, 0);
                                Mix_PlayChannel(-1, player_death_sound, 0);
                                Uint64 _deathTime = SDL_GetTicks64();
                                while (SDL_GetTicks64() - _deathTime <= 2000)
                                {
                                    g_death.renderTexture(g_screen, NULL);
                                    SDL_RenderPresent(g_screen);
                                }
                                gameOver = true;
                                continue;
                            }
                            b_bullet->renderTexture(g_screen);
                        }
                        else
                        {
                            b_bullets.erase(b_bullets.begin() + i);
                            if (b_bullet != NULL)
                            {
                                b_bullet = NULL;
                            }
                        }
                    }
                }
                for (int i = 0; i < int(e_explosions.size()); i++)
                {
                    e_explosions[i]->updateFrame();
                    SDL_Rect currentClip = (EXPLOSION_CLIPS[e_explosions[i]->frame / 5]);
                    if (e_explosions[i]->valid == false)
                        e_explosions.erase(e_explosions.begin() + i);
                    e_explosions[i]->renderTexture(g_screen, &currentClip);
                }
                for (int i = 0; i < int(b_explosions.size()); i++)
                {
                    b_explosions[i]->updateFrame();
                    SDL_Rect B_currentClip = (B_EXPLOSION_CLIPS[b_explosions[i]->b_frame / 5]);
                    if (b_explosions[i]->b_valid == false)
                        b_explosions.erase(b_explosions.begin() + i);
                    b_explosions[i]->renderTexture(g_screen, &B_currentClip);
                }

                std::string scoreText = "Score: " + std::to_string(kill);
                std::string highScoreText = "High Score: " + std::to_string(highScore);
                if (kill > highScore)
                {
                    highScore = kill; // update high score
                    writeHighScore(highScore);
                }
                renderText(scoreText, 10, 10);
                renderText(highScoreText, 10, 40);

                // present
                SDL_RenderPresent(g_screen);
            }
        }
        // game over
        bool isInGameOver = true;
        while (isInGameOver && !is_quit)
        {
            while (SDL_PollEvent(&g_event) != 0)
            {
                if (g_event.type == SDL_QUIT)
                {
                    playAgain = false;
                    is_quit = true;
                }
                if (g_event.type == SDL_KEYDOWN)
                {
                    if (g_event.key.keysym.sym == SDLK_p)
                    {
                        playAgain = true;
                        isInGameOver = false;
                    }
                    else if (g_event.key.keysym.sym == SDLK_x)
                    {
                        playAgain = false;
                        isInGameOver = false;
                    }
                }
            }
            SDL_RenderClear(g_screen);
            g_death_menu.renderTexture(g_screen, NULL);
            SDL_RenderPresent(g_screen);
        }
        closeMusic();
    }
    close();
    return 0;
}
