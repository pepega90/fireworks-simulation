#include "raylib.h"
#include <math.h>
#include <vector>
#include <iostream>
#include <algorithm>
#include <time.h>

enum Scene
{
    MENU = 1,
    PLAY,
    GAMEOVER
};

struct Particle
{
    Vector2 pos;
    Vector2 vel;
    int radius;
    bool explode;
    Color color = WHITE;
};

int main()
{
    const int WIDTH = 1280;
    const int HEIGHT = 700;
    InitWindow(WIDTH, HEIGHT, "Fireworks simulation");
    InitAudioDevice();
    SetTargetFPS(60);

    // load assets
    Sound shoot_sfx = LoadSound("assets/shoot_sfx.ogg");
    Sound explode_sfx = LoadSound("assets/explode.ogg");
    Sound sfx = LoadSound("assets/takbiran_sfx.wav");
    Texture2D sheep = LoadTexture("assets/sheep.png");

    std::vector<Particle> shootParticleList;

    std::vector<Particle> particleList;

    Color listColor[] = {YELLOW, GOLD, ORANGE, PINK, RED, MAROON, GREEN, LIME, BLUE, PURPLE, SKYBLUE, MAGENTA};

    int sizeFireWorks = 250;

    bool shoot = false;

    float dx, dy, angle, fx, fy, distance, lpx, lpy;

    srand((unsigned)time(NULL));

    while (!WindowShouldClose())
    {
        if (!IsSoundPlaying(sfx))
        {
            PlaySound(sfx);
        }

        Vector2 mousePos = GetMousePosition();

        ClearBackground(BLACK);
        BeginDrawing();

        if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && !shoot)
        {
            PlaySound(shoot_sfx);
            lpx = mousePos.x;
            lpy = mousePos.y;
            shoot = true;
            Particle newP;
            newP.pos = Vector2{WIDTH / 2, HEIGHT};
            newP.radius = 10;
            shootParticleList.push_back(newP);
        }

        for (auto &p : shootParticleList)
        {

            dx = lpx - p.pos.x;
            dy = lpy - p.pos.y;

            angle = atan2f(dy, dx);

            fx = cosf(angle) * 8;
            fy = sinf(angle) * 8;
            distance = sqrtf((dx * dx) + (dy * dy));

            if (distance > 30)
            {
                p.vel.x = fx;
                p.vel.y = fy;
            }
            else
            {
                if (!p.explode)
                {
                    float a = PI * 2 / sizeFireWorks;
                    for (int i = 0; i < sizeFireWorks; i++)
                    {
                        float px = cosf(a * i) * (float)rand() / static_cast<float>(RAND_MAX) * 8.;
                        float py = sinf(a * i) * (float)rand() / static_cast<float>(RAND_MAX) * 8.;
                        Particle newP;
                        newP.pos = shootParticleList.back().pos;
                        newP.vel.x = px;
                        newP.vel.y = py;
                        newP.radius = 5;
                        newP.color = listColor[GetRandomValue(0, 13)];
                        particleList.push_back(newP);
                    }
                    p.explode = true;
                    shoot = false;
                    PlaySound(explode_sfx);
                }
            }
        }

        // Selamat hari raya
        DrawText("Selamat hari raya idul adha", 360, 40, 40, WHITE);
        DrawText("1443 Hijiriyah", 500, 90, 40, WHITE);

        for (auto &x : particleList)
        {
            x.vel.x *= 0.99;
            x.vel.y *= 0.99;
            x.vel.y += 0.05;
            x.pos.x += x.vel.x;
            x.pos.y += x.vel.y;
            DrawCircleV(x.pos, x.radius, x.color);
        }

        auto particleRemove = std::remove_if(particleList.begin(), particleList.end(), [&](Particle t)
                                             { return t.pos.y > HEIGHT; });

        if (particleRemove != particleList.end())
        {
            particleList.erase(particleRemove);
        }

        for (auto &p : shootParticleList)
        {
            p.pos.x += p.vel.x;
            p.pos.y += p.vel.y;
            DrawCircleV(p.pos, p.radius, WHITE);
        }

        auto shootParticleRemove = std::remove_if(shootParticleList.begin(), shootParticleList.end(), [&](Particle t)
                                                  { return t.explode == true; });

        if (shootParticleRemove != shootParticleList.end())
        {
            shootParticleList.erase(shootParticleRemove);
        }

        // sheep
        DrawTexture(sheep, WIDTH / 2, HEIGHT - 210, WHITE);

        // credit
        DrawText("created by aji mustofa @peepga90", 20, HEIGHT - 30, 20, WHITE);
        EndDrawing();
    }
    UnloadSound(sfx);
    UnloadSound(shoot_sfx);
    UnloadSound(explode_sfx);
    UnloadTexture(sheep);
    CloseWindow();

    return 0;
}
