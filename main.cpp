#include "mainFunc.h"

#include "Player.h"
#include "Enemy.h"
#include "Explosion.h"

Objects g_background;
Objects g_background2;
Objects g_menu;
Objects g_death;
Objects g_death_menu;

bool loadBackground()
{
    bool ret = g_background.loadImg("assets/background.jpg", g_screen);
    g_background2.loadImg("assets/background.jpg", g_screen);
    g_menu.loadImg("assets/menu.png", g_screen);
    g_death.loadImg("assets/death.png", g_screen);
    g_death_menu.loadImg("assets/death_menu.png", g_screen);
    if (ret == false)
        return false;
    return true;
}
int main(int argc, char *argv[])
{
    bool playAgain = true;
    bool is_quit = false;
    int x=1100;
    int v;
    while (playAgain && !is_quit)
    {
        close();
        int kill = 0;
        srand(time(NULL));
        if (init() == false)
            return -1;
        if (loadBackground() == false)
            return -1;

        bool isInMenu = true;
        while (isInMenu)
        {
            g_menu.renderTexture(g_screen, NULL);
            SDL_RenderPresent(g_screen);
            while (SDL_PollEvent(&g_event) != 0)
            {
                if (g_event.type == SDL_QUIT){

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

        while (!is_quit && !gameOver)
        {

            while (SDL_PollEvent(&g_event) != 0)
            {
                if (g_event.type == SDL_QUIT)
                    is_quit = true;
                p_player.handleInput(g_event, g_screen);
            }
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

            if (kill <= 3)
            {
                for (int i = 0; i < int(enemies.size()); i++)
                {
                    enemies[i]->handleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                    if (checkCollision(enemies[i]->getRect(), p_player.getRect()) == true)
                    {
                        p_player.free();
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
                        enemies[i]->free();
                        enemies.erase(enemies.begin() + i);
                        i--;
                        kill++;
                        v+=0.1;
                        x-=15;
                        continue;
                    }
                    enemies[i]->createBullet(SCREEN_WIDTH, SCREEN_HEIGHT, g_screen, player_pos_x, player_pos_y, e_bullets);
                    if (enemies[i]->get_pos_y() > SCREEN_HEIGHT)
                    {
                        enemies.erase(enemies.begin() + i);
                        i--;
                        continue;
                    }
                }
            }
            else if (kill > 3)
            {
                for (int i = 0; i < int(bosses.size()); i++)
                {
                    bosses[i]->handleMove(SCREEN_WIDTH, SCREEN_HEIGHT);
                    if (checkCollision(bosses[i]->getRect(), p_player.getRect()) == true)
                    {
                        p_player.free();
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
                        Explosion::createBossExplosion(bosses[i]->getRect().x, bosses[i]->getRect().y, b_explosions);
                        bosses[i]->free();
                        bosses.erase(bosses.begin() + i);
                        i--;
                        kill++;
                        continue;
                    }
                    bosses[i]->B_createBullet(SCREEN_WIDTH, SCREEN_HEIGHT, g_screen, player_pos_x, player_pos_y, b_bullets);
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
                if (kill <= 3)
                {
                    int x = rand() % (SCREEN_WIDTH - Enemy_width);
                    v = 4;

                    Enemy *newEnemy = new Enemy(x, v);
                    newEnemy->loadImg("assets/enemy.png", g_screen);
                    enemies.push_back(newEnemy);
                    startTime = SDL_GetTicks64();
                }
                else if (kill > 3)
                {
                    x=900;
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
                    v = 4;
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

            // present
            SDL_RenderPresent(g_screen);
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
    }
    close();
    return 0;
}
