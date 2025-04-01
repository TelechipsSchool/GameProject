#include "rank.h"

// ��ŷ�� ���Ͽ��� �о���� �Լ�
int load_score(Rank* ranks) {
    FILE* file = fopen(RANKING_FILE, "r");

    int rank_count = 0;

    if (file) {
        while (fscanf(file, "%s %d", ranks[rank_count].username, &ranks[rank_count].score) == 2) {
            rank_count++;
            if (rank_count >= MAX_RANKS) break;  // �ִ� ��ŷ ���� ����
        }
        fclose(file);
    }
    else {
        printf("fail to load ranks\n");
    }
    return rank_count;  // �ҷ��� ��ŷ�� ���� ��ȯ
}

int get_high_score() {
    Rank ranks[MAX_RANKS];  // ��ŷ ���� �迭
    int rank_count = load_score(ranks);  // ��ŷ �ε�

    if (rank_count == 0) {
        return 0;  // ��ŷ�� ������ 0 ��ȯ
    }

    int max_score = 0;  // ù ��° ������ �ʱ� �ִ밪���� ����

    for (int i = 1; i < rank_count; i++) {
        if (ranks[i].score > max_score) {
            max_score = ranks[i].score;
        }
    }

    return max_score;
}


// ���ο� ������ ���Ͽ� �����ϴ� �Լ�
void save_score(const char* username, int score) {
    FILE* file = fopen(RANKING_FILE, "a");
    if (file) {
        fprintf(file, "%s %d\n", username, score);
        fclose(file);
    }
}

void display_ranks() {
    ALLEGRO_FONT* rank_font = get_rank_font();

    Rank ranks[MAX_RANKS];

    int rank_count = load_score(ranks);

    bool running = true;

    al_clear_to_color(al_map_rgb(20, 20, 20));

    while (running) {
        int y = 200;

        // �̺�Ʈ ť ����
        ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
        al_register_event_source(event_queue, al_get_keyboard_event_source());
        ALLEGRO_EVENT rank_event;
        al_wait_for_event(event_queue, &rank_event);

        if (rank_event.type == ALLEGRO_EVENT_KEY_DOWN &&
            rank_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = false; // ESC ������ ���� ����
            menu();
        }

        al_draw_text(rank_font, al_map_rgb(255, 255, 0),
            SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "RANKING");

        // ȭ�鿡 �ؽ�Ʈ�� ���
        for (int i = 0; i < rank_count && i < MAX_RANKS; ++i) {
            // �̸� - ���� ǥ��
            al_draw_textf(rank_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y, ALLEGRO_ALIGN_CENTER, "%d. %s - %d", i + 1, ranks[i].username, ranks[i].score);
            y += 50;  // �� ��ŷ ���� ������ �α� ���� y ��ǥ ����
        }

        // ȭ�� ������Ʈ
        al_flip_display();
    }
}