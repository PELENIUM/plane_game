#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <string>

using namespace sf;

int death_counter = 0;
int counter = 0;

const int weight = 800;
const int height = 800;

RenderWindow window(VideoMode(weight, height), "zxc");

int back_y = 0;

Texture back, back2;
Sprite back_spr, back_spr2;

void draw_back() {
    back.loadFromFile("images/back.png");
    back_spr.setTexture(back);
    back2.loadFromFile("images/back.png");
    back_spr2.setTexture(back2);
    if (back_y >= 800)
        back_y = 0;
    back_spr.setPosition(0, back_y);
    back_spr2.setPosition(0, back_y - 800);
    window.draw(back_spr);
    window.draw(back_spr2);
    back_y += 6;
}

class plane {
    public:
        Texture plane_text;
        Sprite plane_spr;
        int x = 350, y = 680;
        void draw_plane() {
            plane_spr.setPosition(x, y);
            window.draw(plane_spr);
        }
        void rotate(int x) {
            plane_spr.setRotation(x);
        }
};

void collision(int x, int y, int e_x, int e_y, plane *enemy, bool bullet, RectangleShape *ptr = NULL) {
    if (bullet) {
        if (((x >= e_x and x <= e_x + 100) and (y >= e_y and y <= e_y + 100)) or ((x + 5 >= e_x and x + 5 <= e_x + 100) and (y >= e_y and y < e_y + 100))) {
            enemy->x = -100;
            enemy->y = -100;
            (*ptr).setFillColor(Color::Transparent);
        }
    }
    if (!bullet) {
        if (((x >= e_x and x <= e_x + 100) and (y >= e_y and y <= e_y + 100)) or ((x + 100 >= e_x and x + 100 <= e_x + 100) and (y >= e_y and y < e_y + 100))) {
            enemy->x = -100;
            enemy->y = -100;
            death_counter++;
        }
    }
}

int main() {
    plane main_plane;
    main_plane.plane_text.loadFromFile("images/plane.png");
    main_plane.plane_spr.setTexture(main_plane.plane_text);
    plane enemy;
    enemy.plane_text.loadFromFile("images/plane.png");
    enemy.plane_spr.setTexture(enemy.plane_text);
    enemy.rotate(180);
    enemy.x = -100;
    enemy.y = -100;
    Clock clock;
    float main_timer = 0;
    bool allow = true;
    std::vector <RectangleShape> bullets;
    float dt = 0;
    while (window.isOpen()) {
        float time = (float) clock.getElapsedTime().asMicroseconds() / 800;
        main_timer += time;
        if (main_timer > 2000) {
            if (allow) {
                enemy.x = rand() % 600 + 100;
                enemy.y = rand() % 100;
                allow = false;
            }
        }
        if (enemy.y <= 900) {
            enemy.y += 8;
        }
        if (enemy.y > 900) {
            enemy.y = -100;
            enemy.x = -100;
            main_timer = 0;
            allow = true;
        }
        dt += time;
        clock.restart();
        Event event;
        while (window.pollEvent(event)) {            
            if ((event.type == Event::Closed) or (event.key.code == Keyboard::Escape))
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::D && main_plane.plane_spr.getPosition().x <= 670)
                    main_plane.x += time / 4;
                if (event.key.code == Keyboard::A && main_plane.plane_spr.getPosition().x >= 30)
                    main_plane.x -= time / 4;
                if (event.key.code == sf::Keyboard::Space and dt > 300) {
                        RectangleShape *bullet = new RectangleShape(Vector2f(5, 25));
                        (*bullet).setPosition(main_plane.x + 45, main_plane.y);
                        (*bullet).setFillColor(Color(237, 32, 25));
                        bullets.push_back(*bullet);
                        dt = 0;
                }
                if (event.key.code == Keyboard::Escape)
                    window.close();
            }
        }
        window.clear({255,255,255});
        draw_back();
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].getPosition().y < -60)
                bullets.erase(bullets.begin() + i);
            else {
                collision(bullets[i].getPosition().x, bullets[i].getPosition().y, enemy.x - 100, enemy.y - 100, &enemy, true, &bullets[i]);
                window.draw(bullets[i]);
                bullets[i].move(0, -10);
            }
        }
        collision(main_plane.x, main_plane.y, enemy.x - 100, enemy.y - 100, &enemy, false);
        enemy.draw_plane();
        main_plane.draw_plane();
        window.display();
        if (death_counter > 2) {
            window.close();
        }
    }
    return 0;
}