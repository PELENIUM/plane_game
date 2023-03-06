#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>

using namespace sf;

int death_counter = 0;
int counter = 0;
int maxcounter = 0;

int speed = 35;

#define weight 800
#define height 800

RenderWindow window(VideoMode(weight, height), "AIR KILLER");

class plane {
private:
    Texture texture;
    Sprite sprite;
public:
    int x = 350, y = 680;
    void set_texture(String text) {
        texture.loadFromFile(text);
    }
    void set_sprite() {
        sprite.setTexture(texture);
    }
    void draw_plane() {
        sprite.setPosition(x, y);
        window.draw(sprite);
    }
    void rotate(int x) {
        sprite.setRotation(x);
    }
    plane(String text, int x1 = 350, int y1 = 680) {
        texture.loadFromFile(text);
        sprite.setTexture(texture);
        x = x1;
        y = y1;
    }
};

int back_y = 0;
Texture back, back2, t, menu_text, how_text, end_text;
Sprite back_spr, back_spr2, menu_spr, how_spr, end_spr;
Font font;
Text score("", font, 50), maxscore("", font, 50);
float main_timer = 0, dt = 0, color_timer = 0;
bool allow = true, is_stop = false, end = false, change_color = false;
std::vector <RectangleShape> bullets;
plane main_plane = plane("images/plane.png"), enemy = plane("images/plane2.png", -100, -100);
float speed_timer = 0;
bool btn1 = false, btn2 = false, fexit = false;



void draw_back() {
    if (!is_stop and !end) {
        back.loadFromFile("images/back.png");
        back_spr.setTexture(back);
        back2.loadFromFile("images/back.png");
        back_spr2.setTexture(back2);
        if (back_y >= 800)
            back_y = 0;
        back_spr.setPosition(0, back_y);
        back_spr2.setPosition(0, back_y - height);
        window.draw(back_spr);
        window.draw(back_spr2);
        back_y += 6;
    }
    if (is_stop or end) {
        back.loadFromFile("images/back.png");
        back_spr.setTexture(back);
        back2.loadFromFile("images/back.png");
        back_spr2.setTexture(back2);
        back_spr.setPosition(0, back_y);
        back_spr2.setPosition(0, back_y - height);
        window.draw(back_spr);
        window.draw(back_spr2);
    }
}

bool collision(int x, int y, int e_x, int e_y, plane* enemy, bool bullet, Sound *snd, RectangleShape* ptr = nullptr) {
    if (bullet) {
        if (((x >= e_x and x <= e_x + 100) and (y >= e_y and y <= e_y + 100)) or ((x + 5 >= e_x and x + 5 <= e_x + 100) and (y >= e_y and y < e_y + 100))) {
            enemy->x = -100;
            enemy->y = -100;
            (*ptr).setFillColor(Color::Transparent);
            counter++;
            (*snd).play();
            return true;
        }
        else
            return false;
    }
    if (!bullet) {
        if (((x >= e_x and x <= e_x + 100) and (y >= e_y and y <= e_y + 100)) or ((x >= e_x and x <= e_x + 100) and (y + 100 >= e_y and y + 100 < e_y + 100))
            or ((x + 100 >= e_x and x + 100 <= e_x + 100) and (y + 100 >= e_y and y + 100 <= e_y + 100)) or ((x + 100 >= e_x and x + 100 <= e_x + 100) and (y >= e_y and y < e_y + 100))) {
            enemy->x = -100;
            enemy->y = -100;
            death_counter++;
            if (counter > 0)
                counter--;
            (*snd).play();
            return true;
        }
        else
            return false;
    }
}

bool btn_check(int btnx, int btny, int mx, int my) {
    if (((mx >= btnx and mx <= btnx + 200) and (my >= btny and my <= btny + 80)) or ((mx + 200 >= btnx and mx + 200 <= btnx + 200) and (my >= btny and my <= btny + 80)))
        return true;
}



void game() {
    SoundBuffer shoot_buf;
    shoot_buf.loadFromFile("sounds/shoot.ogg");
    Sound shoot(shoot_buf);
    shoot.setVolume(30);
    SoundBuffer colis_buf;
    colis_buf.loadFromFile("sounds/collision.ogg");
    Sound colis(colis_buf);
    colis.setVolume(100);
    SoundBuffer end_buf;
    end_buf.loadFromFile("sounds/end.ogg");
    Sound ends(end_buf);
    ends.setVolume(100);
    Music back_mus;
    back_mus.openFromFile("sounds/back.ogg");
    back_mus.setLoop(true);
    back_mus.setVolume(50);
    back_mus.play();
    window.setVerticalSyncEnabled(true);
    menu_text.loadFromFile("images/menu.png");
    menu_spr.setTexture(menu_text);
    how_text.loadFromFile("images/how_to.png");
    how_spr.setTexture(how_text);
    end_text.loadFromFile("images/end.png");
    end_spr.setTexture(end_text);
    Clock clock;
    font.loadFromFile("UF1.ttf");
    score.setStyle(Text::Bold);
    enemy.rotate(180);
    t.loadFromFile("images/bullet.png");
    while (window.isOpen()) {
        window.setFramerateLimit(speed);
        bool flag = true;
        std::ifstream maxtxt("max.txt");
        maxtxt >> maxcounter;
        float time = (float)clock.getElapsedTime().asMicroseconds() / 800;
        main_timer += time;
        speed_timer += time;
        if (speed_timer >= 3000) {
            speed += 8;
            speed_timer = 0;
        }
        if (main_timer > 2000 and !is_stop and !end) {
            if (allow) {
                enemy.x = rand() % 600 + 100;
                enemy.y = rand() % 100;
                allow = false;
            }
        }
        if (enemy.y < 900 and !is_stop and !end) {
            enemy.y += 8;
            collision(main_plane.x, main_plane.y, enemy.x - 100, enemy.y - 100, &enemy, false, &colis);
        }
        if (enemy.y >= 900 and !is_stop and !end) {
            enemy.y = -100;
            enemy.x = -100;
            main_timer = 0;
            allow = true;
        }
        dt += time;
        clock.restart();
        Event event;
        while (window.pollEvent(event)) {
            if (event.type == Event::Closed)
                window.close();
            if (event.type == Event::KeyPressed) {
                if (event.key.code == Keyboard::D && main_plane.x + (time / 2) <= 670 && !is_stop && !end)
                    main_plane.x += time / 2;
                if (event.key.code == Keyboard::A && main_plane.x - (time / 2) >= 30 && !is_stop && !end)
                    main_plane.x -= time / 2;
                if (event.key.code == Keyboard::R) {
                    /*is_stop = false;*/
                    main_plane.x = 350;
                    main_plane.y = 680;
                    end = false;
                    counter = 0;
                    death_counter = 0;
                }
                if (event.key.code == sf::Keyboard::Space and dt > 300 && !is_stop) {
                    shoot.play();
                    RectangleShape* bullet = new RectangleShape(Vector2f(5, 25));
                    (*bullet).setTexture(&t);
                    (*bullet).setTextureRect(IntRect(0, 0, 5, 25));
                    (*bullet).setPosition(main_plane.x + 45, main_plane.y);
                    bullets.push_back(*bullet);
                    dt = 0;
                }
                if (event.key.code == Keyboard::Escape) {
                    is_stop = !is_stop;
                    if (is_stop == false) {
                        btn1 = false;
                        btn2 = false;
                    }
                }
            }
            if (event.type == sf::Event::MouseButtonPressed) {
                if (event.mouseButton.button == Mouse::Left) {
                    Vector2i mouse_pos = Mouse::getPosition(window);
                    Vector2f pos = window.mapPixelToCoords(mouse_pos);
                    btn1 = btn_check(300, 260, pos.x, pos.y);
                    btn2 = btn_check(300, 460, pos.x, pos.y);
                }
            }
        }
        window.clear({ 0, 0, 0 });
        draw_back();
        for (int i = 0; i < bullets.size(); i++) {
            if (bullets[i].getPosition().y < -60)
                bullets.erase(bullets.begin() + i);
            else {
                if (!is_stop) {
                    collision(bullets[i].getPosition().x, bullets[i].getPosition().y, enemy.x - 100, enemy.y - 100, &enemy, true, &colis, &bullets[i]);
                    bullets[i].move(0, -10);
                }
                window.draw(bullets[i]);
            }
        }
        if (death_counter > 2) {
            end = true;
            ends.play();
        }
        collision(main_plane.x, main_plane.y, enemy.x - 100, enemy.y - 100, &enemy, false, &colis);
        enemy.draw_plane();
        main_plane.draw_plane();
        score.setString(std::to_string(counter));
        score.setPosition(700, 730);
        score.setFillColor(Color(254, 89, 194));
        window.draw(score);
        maxscore.setString(std::to_string(maxcounter));
        maxscore.setPosition(620, 730);
        maxscore.setFillColor(Color(254, 80, 190));
        window.draw(maxscore);
        if (is_stop == true and btn1 != true and !end) {
            window.draw(menu_spr);
        }
        if (is_stop == true and btn1 == true and !end) {
            window.draw(how_spr);
        }
        if (end) {
            window.draw(end_spr);
            ends.play();
        }
        window.display();
        if (btn2 == true) {
            window.close();
            fexit = true;
        }
        std::ofstream maxtxt2;
        if (counter > maxcounter) {
            maxtxt2.open("max.txt");
            maxtxt2 << counter;
        }
        maxtxt.close();
        maxtxt2.close();
    }
}

int main() {
    game();
    return 0;
}