#include "rank.h"

// ��ŷ�� ���Ͽ��� �о���� �Լ�
int load_score(Rank* ranks) {
    FILE* file = fopen(RANKING_FILE, "r");
    int rank_count = 0;

    if (file) {
        // ���Ͽ��� username�� score�� �о��
        while (fscanf(file, "%s %d", ranks[rank_count].username, &ranks[rank_count].score) == 2) {
            rank_count++;
            if (rank_count >= MAX_RANKS) break;  // �ִ� ��ŷ ���� �ʰ� ����
        }
        fclose(file);
    }
    else {
        DEBUG_MSG(rank file - �ε� ����);
    }

    return rank_count;  // �ҷ��� ��ŷ �� ��ȯ
}

// �ְ� ������ ��ȯ�ϴ� �Լ�
int get_high_score() {
    Rank ranks[MAX_RANKS];
    int rank_count = load_score(ranks);

    if (rank_count == 0) return 0;  // ��ŷ�� ���� ���

    int max_score = ranks[0].score;  // ù ��° ������ �ʱ�ȭ

    for (int i = 1; i < rank_count; i++) {
        if (ranks[i].score > max_score) {
            max_score = ranks[i].score;
        }
    }
    return max_score;
}

// ������ ���Ͽ� �����ϴ� �Լ�
void save_score(const char* username, int score) {
    FILE* file = fopen(RANKING_FILE, "a");  // append ���
    if (file) {
        fprintf(file, "%s %d\n", username, score);
        fclose(file);
    }
}

// ������ ������������ ���ϴ� �Լ�
int compare_ranks(const void* a, const void* b) {
    Rank* rank_a = (Rank*)a;
    Rank* rank_b = (Rank*)b;
    return rank_b->score - rank_a->score;  // �������� ����
}

// ��ŷ�� ����ϴ� �Լ�
void display_ranks() {
    if (rank_font == NULL) {
        DEBUG_MSG(rank_font - NULL);
        exit(1);
    }

    Rank ranks[MAX_RANKS];
    int rank_count = load_score(ranks);

    // ���� �������� ����
    qsort(ranks, rank_count, sizeof(Rank), compare_ranks);

    bool running = true;
    al_clear_to_color(al_map_rgb(20, 20, 20));

    // �̺�Ʈ ť ����
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    while (running) {
        ALLEGRO_EVENT rank_event;
        al_wait_for_event(event_queue, &rank_event);

        if (rank_event.type == ALLEGRO_EVENT_KEY_DOWN &&
            rank_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = false;  // ESC�� ������
            play_music("audio/cancel.ogg");
            al_destroy_event_queue(event_queue);  // �̺�Ʈ ť ����
            menu();  // �޴��� ���ư�
            return;
        }

        // Ÿ��Ʋ ���
        al_draw_text(rank_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "RANKING");

        // ��ŷ ���
        int y = 200;
        for (int i = 0; i < rank_count && i < MAX_RANKS; ++i) {
            al_draw_textf(rank_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y, ALLEGRO_ALIGN_CENTER,
                "%d. %s - %d", i + 1, ranks[i].username, ranks[i].score);
            y += 50;  // ���� ����
        }

        al_flip_display();  // ȭ�� ����
    }
}