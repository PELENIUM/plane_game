#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
#include <array>
#include <string>
#include <fstream>

using namespace sf;

int death_counter = 0;
int counter = 0;
int maxcounter = 0;

#define weight 800
#define height 800

RenderWindow window(VideoMode(weight, height), "zxc");

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
Texture back, back2, t;
Sprite back_spr, back_spr2;
Font font;
Text score("", font, 50), maxscore("", font, 50);
float main_timer = 0, dt = 0;
bool allow = true;
std::vector <RectangleShape> bullets;
plane main_plane = plane("images/plane.png"), enemy = plane("images/plane2.png", -100, -100);

void draw_back() {
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

bool collision(int x, int y, int e_x, int e_y, plane *enemy, bool bullet, RectangleShape *ptr = nullptr) {
    if (bullet) {
        if (((x >= e_x and x <= e_x + 100) and (y >= e_y and y <= e_y + 100)) or ((x + 5 >= e_x and x + 5 <= e_x + 100) and (y >= e_y and y < e_y + 100))) {
            enemy->x = -100;
            enemy->y = -100;
            (*ptr).setFillColor(Color::Transparent);
            counter++;
            return true;
        }
    }
    if (!bullet) {
        if (((x >= e_x and x <= e_x + 100) and (y >= e_y and y <= e_y + 100)) or ((x >= e_x and x <= e_x + 100) and (y + 100 >= e_y and y + 100 < e_y + 100))
            or ((x + 100 >= e_x and x + 100 <= e_x + 100) and (y + 100 >= e_y and y + 100 <= e_y + 100)) or ((x + 100 >= e_x and x + 100 <= e_x + 100) and (y >= e_y and y < e_y + 100))) {
            enemy->x = -100;
            enemy->y = -100;
            death_counter++;
            counter--;
            return true;
        }
    }
}

int main() {
    Clock clock;
    font.loadFromFile("UF1.ttf");
    score.setStyle(Text::Bold);
    enemy.rotate(180);
    t.loadFromFile("images/bullet.png");
    while (window.isOpen()) {
        bool flag = true;
        std::ifstream maxtxt("max.txt");
        maxtxt >> maxcounter;
        float time = (float) clock.getElapsedTime().asMicroseconds() / 800;
        main_timer += time;
        if (main_timer > 2000) {
            if (allow) {
                enemy.x = rand() % 600 + 100;
                enemy.y = rand() % 100;
                allow = false;
            }
        }
        if (enemy.y < 900) {
            enemy.y += 8;
            collision(main_plane.x, main_plane.y, enemy.x - 100, enemy.y - 100, &enemy, false);
        }
        if (enemy.y >= 900) {
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
                if (event.key.code == Keyboard::D && main_plane.x + (time / 2) <= 670)
                    main_plane.x += time / 2;
                if (event.key.code == Keyboard::A && main_plane.x - (time / 2) >= 30)
                    main_plane.x -= time / 2;
                if (event.key.code == sf::Keyboard::Space and dt > 300) {
                        RectangleShape *bullet = new RectangleShape(Vector2f(5, 25));
                        (*bullet).setTexture(&t);
                        (*bullet).setTextureRect(IntRect(0, 0, 5, 25));
                        (*bullet).setPosition(main_plane.x + 45, main_plane.y);
                        bullets.push_back(*bullet);
                        dt = 0;
                }
                if (event.key.code == Keyboard::Escape)
                    window.close();
            }
        }
        window.clear( { 0, 0, 0 } );
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
        score.setString(std::to_string(counter));
        score.setPosition(700, 730);
        score.setFillColor(Color(254, 89, 194));
        window.draw(score);
        maxscore.setString(std::to_string(maxcounter));
        maxscore.setPosition(620, 730);
        maxscore.setFillColor(Color(254, 80, 190));
        window.draw(maxscore);
        window.display();
        if (death_counter > 2) {
            window.close();
        }
        std::ofstream maxtxt2;
        if (counter > maxcounter) {
            maxtxt2.open("max.txt");
            maxtxt2 << counter;
        }
        maxtxt.close();
        maxtxt2.close();
    }
    return 0;
}