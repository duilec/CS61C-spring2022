#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static char get_board_at(game_state_t* state, int x, int y);
static void set_board_at(game_state_t* state, int x, int y, char ch);
static bool is_tail(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static int incr_x(char c);
static int incr_y(char c);
static void find_head(game_state_t* state, int snum);
static char next_square(game_state_t* state, int snum);
static void update_tail(game_state_t* state, int snum);
static void update_head(game_state_t* state, int snum);

/* Helper function to get a character from the board (already implemented for
 * you). */
static char get_board_at(game_state_t* state, int x, int y) {
  return state->board[y][x];
}

/* Helper function to set a character on the board (already implemented for
 * you). */
static void set_board_at(game_state_t* state, int x, int y, char ch) {
  state->board[y][x] = ch;
}

/* Task 1 */
game_state_t* create_default_state() {
  game_state_t* default_state = (game_state_t*)malloc(sizeof(game_state_t));
  default_state->x_size = DEFAULT_WIDTH;
  default_state->y_size = DEFAULT_HEIGHT;
  // allocate memory of board, then, set walls and space
  // note: state->board[y][x] = ch; in set_board_at()
  default_state->board = (char**)malloc(DEFAULT_HEIGHT * sizeof(char*));
  for (int y = 0; y < DEFAULT_HEIGHT; y++) {
    default_state->board[y] = (char*)malloc(DEFAULT_WIDTH * sizeof(char));
  }
  for (int x = 0; x < DEFAULT_WIDTH; x++) {
    for (int y = 0; y < DEFAULT_HEIGHT; y++) {
      if (x == 0 || x == DEFAULT_WIDTH - 1 || y == 0 || y == DEFAULT_HEIGHT - 1) {
        set_board_at(default_state, x, y, '#');
      } else {
        set_board_at(default_state, x, y, ' ');
      }
    }
  }
  // set head, tail and fruit in board
  set_board_at(default_state, 5, 4, '>');
  set_board_at(default_state, 4, 4, 'd');
  set_board_at(default_state, 9, 2, '*');
  // set position of head and tail in snake
  default_state->snakes = (snake_t*)malloc(sizeof(snake_t));
  default_state->snakes->head_x = 5;
  default_state->snakes->head_y = 4;
  default_state->snakes->tail_x = 4;
  default_state->snakes->tail_y = 4;
  // default state of snakes is living(i.e. live == true)
  default_state->snakes->live = true;
  // set the number of sanke
  default_state->num_snakes = 1;
  return default_state;
}

/* Task 2 */
void free_state(game_state_t* state) {
  free(state->snakes);
  for (int y = 0; y < state->y_size; y++) {
    free(state->board[y]);
  }
  free(state->board);
  free(state);
  return;
}

/* Task 3 */
void print_board(game_state_t* state, FILE* fp) {
  /* useless! Why??? */
  // for (int x = 0; x < WIDTH; x++) {
  //   for (int y = 0; y < HEIGHT; y++) {
  //     if (x == WIDTH - 1) {
  //       fprintf(fp, "%c\n", state->board[y][x]);
  //     } else {
  //       fprintf(fp, "%c", state->board[y][x]);
  //     }
  //   }
  // }

  /* useful! */
  for (int y = 0; y < state->y_size; y++) {
    fprintf(fp, "%s\n", state->board[y]);
  }
  return;
}

/* Saves the current state into filename (already implemented for you). */
void save_board(game_state_t* state, char* filename) {
  FILE* f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */
static bool is_tail(char c) {
  return (c == 'w' || c == 'a' || c == 's' || c == 'd');
}

static bool is_snake(char c) {
  return is_tail(c) || (c == '^') || (c == '<') || (c == '>') || (c == 'v');
}

static char body_to_tail(char c) {
  switch (c) {
    case '^':
      return 'w';
    case '<':
      return 'a';
    case '>':
      return 'd';
    case 'v':
      return 's';
  }
  return ' ';
}

static int incr_x(char c) {
  switch (c) {
    case '>':
      return 1;
    case 'd':
      return 1;
    case '<':
      return -1;
    case 'a':
      return -1;
    default:
      return 0;
  }
}

static int incr_y(char c) {
  switch (c) {
    case 'v':
      return 1;
    case 's':
      return 1;
    case '^':
      return -1;
    case 'w':
      return -1;
    default:
      return 0;
  }
}

/* Task 4.2 */
static char next_square(game_state_t* state, int snum) {
  snake_t snake = state->snakes[snum];
  int head_y = snake.head_y;
  int head_x = snake.head_x;
  char head_sq = state->board[head_y][head_x];
  char next_sq =
      state->board[head_y + incr_y(head_sq)][head_x + incr_x(head_sq)];
  return next_sq;
}

/* Task 4.3 */
static void update_head(game_state_t* state, int snum) {
  snake_t snake = state->snakes[snum];
  int head_y = snake.head_y;
  int head_x = snake.head_x;
  // get the updated head
  char update_head = state->board[head_y][head_x];
  int update_y = head_y + incr_y(update_head);
  int update_x = head_x + incr_x(update_head);

  // update new position of head in board
  set_board_at(state, update_x, update_y, update_head);
  // update new position of tail in snake
  state->snakes[snum].head_y = update_y;
  state->snakes[snum].head_x = update_x;
  return;
}

/* Task 4.4 */
static void update_tail(game_state_t* state, int snum) {
  snake_t snake = state->snakes[snum];
  int tail_y = snake.tail_y;
  int tail_x = snake.tail_x;
  // get the updated tail
  char old_tail = get_board_at(state, tail_x, tail_y);
  int update_y = tail_y + incr_y(old_tail);
  int update_x = tail_x + incr_x(old_tail);
  // get a body which is the next of tail
  char body_next_tail = get_board_at(state, update_x, update_y);
  char update_tail = body_to_tail(body_next_tail);

  // update new position of tail in board
  set_board_at(state, update_x, update_y, update_tail);
  // set old position to ' '
  set_board_at(state, tail_x, tail_y, ' ');
  // update new position of tail in snake
  state->snakes[snum].tail_y = update_y;
  state->snakes[snum].tail_x = update_x;
  return;
}

/* Task 4.5 */
void update_state(game_state_t* state, int (*add_food)(game_state_t* state)) {
  for (int i = 0; i < state->num_snakes; i++) {
    char next_sq = next_square(state, i);
    switch (next_sq) {
      case ' ':
        update_head(state, i);
        update_tail(state, i);
        break;
      case '*':
        add_food(state);
        update_head(state, i);
        break;
      // note: use pointer to change snake but NOT use local varibble of snake
      default:
        set_board_at(state, state->snakes[i].head_x, state->snakes[i].head_y,
                     'x');
        state->snakes[i].live = false;
        break;
    }
  }
  return;
}

/* Task 5 */
game_state_t* load_board(char* filename) {
  FILE* f = fopen(filename, "r");
  game_state_t* state = (game_state_t*)malloc(sizeof(game_state_t));
  // seek position of ending('\0') in f
  fseek(f, 0, SEEK_END);
  // get the current position(the ending of position) 
  long f_len = ftell(f);
  // rewind to the beginning of position in f
  rewind(f);

  char* temp_board = malloc(sizeof(char) * f_len);
  fread(temp_board, f_len, 1, f);
  fclose(f);
  // get the width of board
  for (int i = 0; i < f_len; i++) {
    if (temp_board[i] == '\n') {
      state->x_size = i;
      break;
    }
  }
  // get the height of board
  state->y_size = 0;
  for (int i = 0; i < f_len; i++) {
    if (temp_board[i] == '\n') {
      state->y_size += 1;
    }
  }

  // get the board in state
  state->board = (char**)malloc(state->y_size * sizeof(char*));
  for (int y = 0; y < state->y_size; y++) {
    state->board[y] = (char*)malloc(state->x_size * sizeof(char));
  }
  int i = 0;
  for (int y = 0; y < state->y_size; y++) {
    for (int x = 0; x < state->x_size; x++) {
      // if meeting '\n', we should skip it
      if (temp_board[i] == '\n') {
        i += 1;
      }
      set_board_at(state, x, y, temp_board[i]);
      i += 1;
    }
  }

  free(temp_board);
  return state;
}

/* use recursion to get the head*/
static void get_head(game_state_t* state, snake_t* snake, int prev_x, int prev_y, int curr_x, int curr_y) {
  char board_sq = get_board_at(state, curr_x, curr_y);
  if (board_sq == ' ' || board_sq == '*' || board_sq == '#') {
    snake->head_x = prev_x;
    snake->head_y = prev_y;
    return;
  }
  return get_head(state, snake, curr_x, curr_y, curr_x + incr_x(board_sq), curr_y + incr_y(board_sq));
}

/* Task 6.1 */
static void find_head(game_state_t* state, int snum) {
  get_head(state, &state->snakes[snum], 0, 0, state->snakes[snum].tail_x, state->snakes[snum].tail_y);
  return;
}

/* Task 6.2 */
game_state_t* initialize_snakes(game_state_t* state) {
  // initialize tail in all snakes
  // we must initialize tail firstly, because calling find_head() depond on 'tail'
  int index_snakes = 0;
  state->snakes = (snake_t*)malloc(sizeof(snake_t));
  for (int x = 0; x < state->x_size; x++) {
    for (int y = 0; y < state->y_size; y++) {
      if (is_tail(get_board_at(state, x, y))) {
        state->snakes = (snake_t*)realloc(state->snakes, sizeof(snake_t) * (index_snakes + 1));
        state->snakes[index_snakes].tail_x = x;
        state->snakes[index_snakes].tail_y = y;
        index_snakes += 1;
      }
    }
  }
  // in the index_snakes NOT represents index but representing the number
  state->num_snakes = index_snakes;
  // initialize head and 'live' in all snakes
  for (int i = 0; i < state->num_snakes; i++) {
    find_head(state, i);
    if (get_board_at(state, state->snakes[i].head_x, state->snakes[i].head_y) == 'x') {
      state->snakes[i].live = false;
    } else {
      state->snakes[i].live = true;
    }
  }
  return state;
}
