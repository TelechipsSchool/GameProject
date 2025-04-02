#include "rank.h"

// 랭킹을 파일에서 읽어오는 함수
int load_score(Rank* ranks) {
    FILE* file = fopen(RANKING_FILE, "r");
    int rank_count = 0;

    if (file) {
        // 파일에서 username과 score를 읽어옴
        while (fscanf(file, "%s %d", ranks[rank_count].username, &ranks[rank_count].score) == 2) {
            rank_count++;
            if (rank_count >= MAX_RANKS) break;  // 최대 랭킹 개수 초과 방지
        }
        fclose(file);
    }
    else {
        DEBUG_MSG(rank file - 로드 실패);
    }

    return rank_count;  // 불러온 랭킹 수 반환
}

// 최고 점수를 반환하는 함수
int get_high_score() {
    Rank ranks[MAX_RANKS];
    int rank_count = load_score(ranks);

    if (rank_count == 0) return 0;  // 랭킹이 없을 경우

    int max_score = ranks[0].score;  // 첫 번째 점수로 초기화

    for (int i = 1; i < rank_count; i++) {
        if (ranks[i].score > max_score) {
            max_score = ranks[i].score;
        }
    }
    return max_score;
}

// 점수를 파일에 저장하는 함수
void save_score(const char* username, int score) {
    FILE* file = fopen(RANKING_FILE, "a");  // append 모드
    if (file) {
        fprintf(file, "%s %d\n", username, score);
        fclose(file);
    }
}

// 점수를 내림차순으로 비교하는 함수
int compare_ranks(const void* a, const void* b) {
    Rank* rank_a = (Rank*)a;
    Rank* rank_b = (Rank*)b;
    return rank_b->score - rank_a->score;  // 내림차순 정렬
}

// 랭킹을 출력하는 함수
void display_ranks() {
    if (rank_font == NULL) {
        DEBUG_MSG(rank_font - NULL);
        exit(1);
    }

    Rank ranks[MAX_RANKS];
    int rank_count = load_score(ranks);

    // 점수 내림차순 정렬
    qsort(ranks, rank_count, sizeof(Rank), compare_ranks);

    bool running = true;
    al_clear_to_color(al_map_rgb(20, 20, 20));

    // 이벤트 큐 생성
    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();
    al_register_event_source(event_queue, al_get_keyboard_event_source());

    while (running) {
        ALLEGRO_EVENT rank_event;
        al_wait_for_event(event_queue, &rank_event);

        if (rank_event.type == ALLEGRO_EVENT_KEY_DOWN &&
            rank_event.keyboard.keycode == ALLEGRO_KEY_ESCAPE) {
            running = false;  // ESC로 나가기
            play_music("audio/cancel.ogg");
            al_destroy_event_queue(event_queue);  // 이벤트 큐 해제
            menu();  // 메뉴로 돌아감
            return;
        }

        // 타이틀 출력
        al_draw_text(rank_font, al_map_rgb(255, 255, 0), SCREEN_W / 2, 100, ALLEGRO_ALIGN_CENTER, "RANKING");

        // 랭킹 출력
        int y = 200;
        for (int i = 0; i < rank_count && i < MAX_RANKS; ++i) {
            al_draw_textf(rank_font, al_map_rgb(255, 255, 255), SCREEN_W / 2, y, ALLEGRO_ALIGN_CENTER,
                "%d. %s - %d", i + 1, ranks[i].username, ranks[i].score);
            y += 50;  // 간격 조절
        }

        al_flip_display();  // 화면 갱신
    }
}