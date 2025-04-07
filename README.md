# GameProject
## 메인 화면
![image](https://github.com/user-attachments/assets/43467f15-31b2-4262-abd5-f55c49576f09)
## Game1 - 행성 만들기
![제목 없는 디자인](https://github.com/user-attachments/assets/ba57eaca-09c4-4ee3-8d9c-0a2723b833f2)
## Game2 - 소행성 피하기
![제목 없는 디자인 (1)](https://github.com/user-attachments/assets/94234133-2cec-40be-8ad7-91617d819a6d)

# Cosmic Protector Game - Module & API Documentation

## 1. `main.c`

### `int main(void)`
**역할**: 프로그램 진입점으로, 초기화 및 메뉴 실행, 종료 처리 수행.

**설명**:
- `init()` 호출로 Allegro 시스템 및 관련 초기화.
- `init_all_fonts()`로 전역 폰트 로드.
- `menu()` 호출하여 메인 메뉴 UI 진입.
- `destroy_all_fonts()`로 리소스 정리 후 종료.

---

## 2. `init.c`

### `void init(void)`
**역할**: Allegro, 오디오, 이미지, 폰트 시스템 등 초기화.

**포함 라이브러리**:
```c
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <allegro5/allegro5.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_audio.h>
#include <allegro5/allegro_acodec.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_ttf.h>
```

---

## 3. `menu.c`

### `void menu(void)`
**역할**: 게임의 메인 메뉴 UI를 출력하고, 키보드 입력을 통해 항목 선택.

**설명**:
- 메뉴 버튼은 전역 배열 `menu_buttons[]`에 정의.
- `display_ranks()`, `help_menu()`, `story1()`, `show_setting_menu()` 등 호출.

**포함 파일**:
```c
#include "init.h"
#include "display.h"
#include "font.h"
#include "rank.h"
#include "setting.h"
#include "audio.h"
#include "help.h"
#include "image.h"
```

---

## 4. `story.c`

**스토리 진행 및 사용자 이름 입력 관련 함수 모음**

- `void wait_or_skip(double duration)`
- `void story1(void)`
- `void story2(int score)`
- `void story3(void)`
- `void story4(char* username)`
- `char* getUserName(void)`

**포함 파일**:
```c
#include "init.h"
#include "font.h"
#include "display.h"
#include "audio.h"
```

---

## 5. `setting.c`

### `void show_setting_menu(void)`
**역할**: 난이도 설정 메뉴 출력 및 `current_difficulty` 업데이트

**포함 파일**:
```c
#include "init.h"
#include "font.h"
#include "display.h"
#include "audio.h"
```

---

## 6. `rank.c`

### 주요 함수
- `int load_score(Rank* ranks)`
- `int get_high_score(void)`
- `void save_score(const char* username, int score)`
- `int compare_ranks(const void* a, const void* b)`
- `void display_ranks(void)`

**포함 파일**:
```c
#include "font.h"
#include "audio.h"
```

---

## 7. `help.c`

### `void help_menu(void)`
**역할**: 게임 설명 및 도움말 출력

**포함 파일**:
```c
#include "init.h"
#include "audio.h"
```

---

## 8. `game.c`
### `void game1(char* username)`
**역할**: 태양 만들기 게임 메인 루프

**설명**:
- 이벤트 처리, 물리 계산, 오브젝트 상태 업데이트, 사운드 등 처리
- 조건에 따라 `story2()` 등 호출

**포함 파일**:
```c
#include "init.h"
#include "menu.h"
#include "display.h"
#include "rank.h"
#include "setting.h"
#include "font.h"
#include "audio.h"
#include "story.h"
#include "image.h"
#include "resource.h"
#include "planet.h"
#include "physics.h"
```

---

## 9. `game2.c` (cosmic_protector.c)

### 주요 함수
- `void loadBitmap(void)`
- `void destroyBitmap(void)`
- `double getRadian(int num)`
- `void update_rocket(void)`
- `void fire_bullet(void)`
- `void update_bullets(void)`
- `void alien_bullets(void)`
- `void alien_update_bullets(void)`
- `void spawn_asteroids(void)`
- `void update_asteroids(void)`
- `void check_collisions(void)`
- `void check_die(void)`
- `void alien_create(void)`
- `void alien_update(void)`
- `void draw_scene(void)`

**역할**: 우주선, 외계인, 소행성 중심의 슈팅 게임 루프와 렌더링 처리

---

## 10. `resource.c`, `image.c`, `audio.c`, `font.c`

### `resource.c`
- 타이머, 이벤트 큐 초기화 및 해제
- 이미지 및 사운드 리소스 초기화

### `image.c`, `display.c`
- 이미지 로드, 크기 조절, 전역 포인터 관리

### `audio.c`
- BGM, 효과음 로드 및 재생 함수 정의

### `font.c`
- `init_all_fonts()` / `destroy_all_fonts()` 함수로 전역 폰트 관리

---

## 11. Physics / Planet 연산 모듈

### `vector.c`
- 벡터 연산 함수 모음 (`Add`, `Sub`, `Scale`, `Dot`, `Distance` 등)

### `planet.c`
- 행성 생성, 병합, 초기화, 난이도 기반 타입 결정 등 처리

### `physics.c`
- 충돌 감지 및 중력 계산 함수 제공

---
## 참고
- 개발 언어: C (Allegro 5)
- 주요 렌더링 방식: `al_draw_bitmap`, `al_draw_rotated_bitmap`
- 오디오 처리: `al_load_sample`, `al_play_sample`
- 물리 엔진: 사용자 정의 중력, 충돌 로직 적용
- 개발 언어: <img src="https://img.shields.io/badge/C-A8B9CC?style=flat&logo=C&logoColor=white">
- 협업 툴:
<a href = "https://github.com/TelechipsSchool/GameProject.git"><img alt="GitHub" src ="https://img.shields.io/badge/GitHub-181717.svg?&style=for-the-badge&logo=GitHub&logoColor=white"/>
</a> <a href = "">
<img alt="Notion" src ="https://img.shields.io/badge/Notion-000000.svg?&style=for-the-badge&logo=Notion&logoColor=white"/></a> 
---
## 팀원
   <table>
  <tbody>
    <tr>
      <td align="center"><a href="https://github.com/Hyykk"><img src="https://avatars.githubusercontent.com/Hyykk" width="100px;" alt=""/><br /><sub><b> 홍윤교 </b></sub></a><br /></td>
      <td align="center"><a href="https://github.com/argan719"><img src="https://avatars.githubusercontent.com/argan719" width="100px;" alt=""/><br /><sub><b> 전지현 </b></sub></a><br /></td>
<td align="center"><a href="https://github.com/inbangCert"><img src="https://avatars.githubusercontent.com/inbangCert" width="100px;" alt=""/><br /><sub><b> 이원준 </b></sub></a><br /></td>
<td align="center"><a href="https://github.com/Skylarvv"><img src="https://avatars.githubusercontent.com/Skylarvv" width="100px;" alt=""/><br /><sub><b> 홍예주 </b></sub></a><br /></td>
  </tbody>
</table>

