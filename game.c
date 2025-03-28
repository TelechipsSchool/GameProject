#include "game.h"
#include <math.h>

void start_game() {

    ALLEGRO_TIMER* timer = al_create_timer(1.0 / FPS);
    ALLEGRO_EVENT_QUEUE* game_event_queue = al_create_event_queue();
    al_register_event_source(game_event_queue, al_get_mouse_event_source());
    al_register_event_source(game_event_queue, al_get_timer_event_source(timer));
    al_start_timer(timer);

    ALLEGRO_BITMAP* planet_img = load_bitmap_resized("images/planet_1.png", 30, 30);
    Vector2 shootStart = { init_x, init_y };
    Rigidbody2D planet = { shootStart, {0, 0}, 0 };

    Vector2 gravityCenter = { center_x, center_y };
    ALLEGRO_BITMAP* center = load_bitmap_resized("images/center.png", 30, 30);
    ALLEGRO_BITMAP* startpoint = load_bitmap_resized("images/ShootStartingPoint.png", 150, 150);
    ALLEGRO_BITMAP* gravityfield = load_bitmap_resized("images/GravityField.png", 500, 500);

    Vector2 dragStart, dragEnd;
    float maxForce = 70.0f;
    float dragThreshold = 30.0f;
    float gravityScale = 1.0f;
    float centerCoefficient = 0.03f;
    float deltaTime = 1.0f / FPS;

    float angle = 0.0;
    bool isFired = false, redraw = true, playing = true, isDragging = false;

    while (playing) {
        ALLEGRO_EVENT event;
        al_wait_for_event(game_event_queue, &event);

        if (event.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
            playing = 0;
        else if (event.type == ALLEGRO_EVENT_TIMER)
            redraw = 1;
        
        ALLEGRO_MOUSE_STATE mouse;
        al_get_mouse_state(&mouse);

        if (al_mouse_button_down(&mouse, 1)) {  
            if (!isDragging) {
                isDragging = true;
                dragStart.x = mouse.x;
                dragStart.y = mouse.y;
            }
            dragEnd.x = mouse.x;
            dragEnd.y = mouse.y;
        }
        else if (isDragging) {
            isDragging = false;
            Vector2 dragVec = Vector2_Sub(dragEnd, dragStart);
            printf("debug 1: ");
            printf("%.lf\n", Vector2_Length(dragVec));

            if (Vector2_Length(dragVec) > dragThreshold) {
                printf("debug 2\n");
                Vector2 force = Vector2_Clamp(Vector2_Scale(dragVec, -0.2f), maxForce);
                planet.velocity = force;
                planet.position = shootStart;
                planet.isFlying = true;
            }
        }

        if (planet.isFlying) {
            CalcGravity(&planet, gravityCenter, gravityScale, centerCoefficient, deltaTime);
            planet.position = Vector2_Add(planet.position, Vector2_Scale(planet.velocity, deltaTime));
        }

        if (redraw) {
            al_clear_to_color(al_map_rgb(20, 20, 20));
            al_draw_bitmap(planet_img, planet.position.x - 15, planet.position.y - 15, 0);
            al_draw_bitmap(center, center_x - 15, center_y - 15, 0);
            al_draw_bitmap(gravityfield, center_x - 250, center_y - 250, 0);
            al_draw_bitmap(startpoint, init_x - 75, init_y - 75, 0);

            if (isDragging) {
                Vector2 forceVec = Vector2_Clamp(Vector2_Scale(Vector2_Sub(dragEnd, dragStart), -0.2f), maxForce);
                Vector2 endPoint = Vector2_Add(shootStart, forceVec);
                al_draw_line(shootStart.x, shootStart.y, endPoint.x, endPoint.y, al_map_rgb(255, 255, 0), 2);
            }

            al_flip_display();
        }
        //printf("isDragging: %d\n", isDragging);
        //printf("isFlying: %d\n", planet.isFlying);
    }    

    al_destroy_bitmap(planet_img);
    al_destroy_bitmap(center);
    al_destroy_bitmap(gravityfield);
    al_destroy_bitmap(startpoint);
    al_destroy_event_queue(game_event_queue);
}


Planet create_planet(float x, float y, float speed, float angle, int width, int height, const char* image_path) {
    Planet planet;
    planet.x = x;
    planet.y = y;
    planet.speed = speed;
    planet.angle = angle;
    planet.width = width;
    planet.height = height;
    planet.image = load_bitmap_resized(image_path, width, height);

    return planet;
}

bool collision_check(int x1, int y1, int size1, int x2, int y2, int size2) {
    // 두 원의 중심 간 거리 계산
    int dx = x2 - x1;
    int dy = y2 - y1;
    float distance = sqrt(dx * dx + dy * dy);

    // 두 원의 반지름 합이 두 원의 중심 간 거리보다 크거나 같으면 충돌
    if (distance < (size1 + size2) / 2) {
        return true; // 충돌
    }
    else {
        return false; // 충돌하지 않음
    }
}

// 핵심 중력 + 회전 보정 함수
void CalcGravity(Rigidbody2D* rb, Vector2 center, float gravityScale, float centerCoefficient, float deltaTime) {
    Vector2 gravityDir = Vector2_Normalize(Vector2_Sub(center, rb->position));
    Vector2 gravity = Vector2_Scale(gravityDir, gravityScale * 9.8f);

    Vector2 vel = rb->velocity;
    Vector2 nextPos = Vector2_Add(rb->position, vel);

    Vector2 parallelVel = Vector2_Project(vel, gravityDir);
    Vector2 perpendicularVel = Vector2_Sub(vel, parallelVel);

    Vector2 correctionDir = Vector2_Project((Vector2) { -perpendicularVel.x, -perpendicularVel.y }, Vector2_Sub(nextPos, center));
    Vector2 newPerpendicularVel = Vector2_Add(perpendicularVel, correctionDir);

    perpendicularVel.x = (1 - centerCoefficient) * perpendicularVel.x + centerCoefficient * newPerpendicularVel.x;
    perpendicularVel.y = (1 - centerCoefficient) * perpendicularVel.y + centerCoefficient * newPerpendicularVel.y;

    rb->velocity = Vector2_Add(Vector2_Add(parallelVel, perpendicularVel), Vector2_Scale(gravity, deltaTime));
}