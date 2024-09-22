#include "state.h"

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "snake_utils.h"

/* Helper function definitions */
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch);
static bool is_tail(char c);
static bool is_head(char c);
static bool is_snake(char c);
static char body_to_tail(char c);
static char head_to_body(char c);
static unsigned int get_next_row(unsigned int cur_row, char c);
static unsigned int get_next_col(unsigned int cur_col, char c);
static void find_head(game_state_t *state, unsigned int snum);
static unsigned int next_row(game_state_t *state, snake_t *snake, char c);
static unsigned int next_col(game_state_t *state, snake_t *snake, char c);
static char next_square(game_state_t *state, unsigned int snum);
static void update_tail(game_state_t *state, unsigned int snum);
static void update_head(game_state_t *state, unsigned int snum);

/* Task 1 */
game_state_t *create_default_state() {
  // TODO: Implement this function.
  
    game_state_t* default_state = malloc(sizeof(game_state_t));
    default_state->num_rows = 18;
    default_state->num_snakes = 1;

    default_state->board = malloc(default_state->num_rows * sizeof(char*));
   
    char middle_rows[] = "#                  #\n";

    for (int i = 0; i < default_state->num_rows; ++i) {
        default_state->board[i] = malloc(20);
        strcpy(default_state->board[i], middle_rows);
    }

    strcpy(default_state->board[0], "####################\n");
    strcpy(default_state->board[17], "####################\n"); 
    
    strcpy(default_state->board[2],"# d>D    *         #\n"); 

    const char *default_board[18] = {
     "####################",
     "#                  #",
     "# d>D    *         #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "#                  #",
     "####################"
    };

//    for (int i = 0; i < default_state->num_rows; ++i) {
//        strcpy(default_state->board[i], default_board[i]);
//    }

    
    default_state->snakes = malloc(default_state->num_snakes * sizeof(snake_t));

    default_state->snakes[0].tail_row = 2;
    default_state->snakes[0].tail_col = 2;
    default_state->snakes[0].head_row = 2;
    default_state->snakes[0].head_col = 4;
    default_state->snakes[0].live = true;

    return default_state;
}

/* Task 2 */
void free_state(game_state_t *state) {
  // TODO: Implement this function.
    free(state->snakes);

    for (int i = 0; i < state->num_rows; ++i) {
        free(state->board[i]); 
    }

    free(state->board);

    free(state);
    return;
}

/* Task 3 */
void print_board(game_state_t *state, FILE *fp) {
  for (int i = 0; i < state->num_rows; ++i) {
    fprintf(fp,"%s", state->board[i]);
  }
  return;
}

/*
  Saves the current state into filename. Does not modify the state object.
  (already implemented for you).
*/
void save_board(game_state_t *state, char *filename) {
  FILE *f = fopen(filename, "w");
  print_board(state, f);
  fclose(f);
}

/* Task 4.1 */

/*
  Helper function to get a character from the board
  (already implemented for you).
*/
char get_board_at(game_state_t *state, unsigned int row, unsigned int col) { return state->board[row][col]; }

/*
  Helper function to set a character on the board
  (already implemented for you).
*/
static void set_board_at(game_state_t *state, unsigned int row, unsigned int col, char ch) {
  state->board[row][col] = ch;
}

/*
  Returns true if c is part of the snake's tail.
  The snake consists of these characters: "wasd"
  Returns false otherwise.
*/
static bool is_tail(char c) {
  // TODO: Implement this function.
   return (c == 'w' || c == 'a' || c == 's' || c == 'd');
}

/*
  Returns true if c is part of the snake's head.
  The snake consists of these characters: "WASDx"
  Returns false otherwise.
*/
static bool is_head(char c) {
  // TODO: Implement this function.
    return (c == 'W' || c == 'A' || c == 'S' || c == 'D' || c == 'x');
}

/*
  Returns true if c is part of the snake.
  The snake consists of these characters: "wasd^<v>WASDx"
*/
static bool is_snake(char c) {
  // TODO: Implement this function.
  return (is_tail(c) || is_head(c) || c == '^' || c == '<' || c == 'v' || c == '>');
}

/*
  Converts a character in the snake's body ("^<v>")
  to the matching character representing the snake's
  tail ("wasd").
*/
static char body_to_tail(char c) {
  // TODO: Implement this function
  if (c == '^') return 'w';
  if (c == '<') return 'a';
  if (c == 'v') return 's';
  if (c == '>') return 'd';
  return '?';
}

/*
  Converts a character in the snake's head ("WASD")
  to the matching character representing the snake's
  body ("^<v>").
*/
static char head_to_body(char c) {
  // TODO: Implement this function.
  //
  if (c == 'W') return '^';
  if (c == 'A') return '<';
  if (c == 'S') return 'v';
  if (c == 'D') return '>';

  return '?';
}

/*
  Returns cur_row + 1 if c is 'v' or 's' or 'S'.
  Returns cur_row - 1 if c is '^' or 'w' or 'W'.
  Returns cur_row otherwise.
*/
static unsigned int get_next_row(unsigned int cur_row, char c) {
  // TODO: Implement this function.
  if (c == 'v' || c == 's' || c == 'S') return cur_row + 1;
  if (c == '^' || c == 'w' || c == 'W') return cur_row - 1;
  return cur_row;
}

/*
  Returns cur_col + 1 if c is '>' or 'd' or 'D'.
  Returns cur_col - 1 if c is '<' or 'a' or 'A'.
  Returns cur_col otherwise.
*/
static unsigned int get_next_col(unsigned int cur_col, char c) {
  // TODO: Implement this function.
  if (c == '>' || c == 'd' || c == 'D') return cur_col + 1;
  if (c == '<' || c == 'a' || c == 'A') return cur_col - 1;

  return cur_col;
}

/*
  Task 4.2

  Helper function for update_state. Return the character in the cell the snake is moving into.

  This function should not modify anything.
*/

static unsigned int next_row(game_state_t *state, snake_t *snake, char c) {
    if (c == 'h') {
      return get_next_row(snake->head_row, get_board_at(state, snake->head_row, snake->head_col)); 
    } 
    return get_next_row(snake->tail_row, get_board_at(state, snake->tail_row, snake->tail_col));
}

static unsigned int next_col(game_state_t *state, snake_t *snake, char c) {
    if (c == 'h') {
      return get_next_col(snake->head_col, get_board_at(state, snake->head_row, snake->head_col)); 
    } 
    return get_next_col(snake->tail_col, get_board_at(state, snake->tail_row, snake->tail_col));
}
static char next_square(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t *snake = &state->snakes[snum];
  unsigned int next_r = next_row(state, snake, 'h');
  unsigned int next_c = next_col(state, snake, 'h');
  return get_board_at(state, next_r, next_c);
}

/*
  Task 4.3
  Helper function for update_state. Update the head...

  ...on the board: add a character where the snake is moving

  ...in the snake struct: update the row and col of the head

  Note that this function ignores food, walls, and snake bodies when moving the head.
*/
static void update_head(game_state_t *state, unsigned int snum) {
  snake_t *snake = &state->snakes[snum];
  unsigned int next_r = next_row(state, snake, 'h');
  unsigned int next_c = next_col(state, snake, 'h');
 
  set_board_at(state, next_r, next_c, get_board_at(state, snake->head_row, snake->head_col));
  set_board_at(state, snake->head_row, snake->head_col, head_to_body(get_board_at(state, snake->head_row, snake->head_col)));
 
  snake->head_row = next_r;
  snake->head_col = next_c;
}

/*
  Task 4.4

  Helper function for update_state. Update the tail...

  ...on the board: blank out the current tail, and change the new
  tail from a body character (^<v>) into a tail character (wasd)

  ...in the snake struct: update the row and col of the tail
*/
static void update_tail(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t *snake = &state->snakes[snum];
  unsigned int next_r = next_row(state, snake, 't');
  unsigned int next_c = next_col(state, snake, 't');
 
  set_board_at(state, next_r, next_c, body_to_tail(get_board_at(state, next_r, next_c)));
  set_board_at(state, snake->tail_row, snake->tail_col, ' ');
 
  snake->tail_row = next_r;
  snake->tail_col = next_c; 
}

/* Task 4.5 */
void update_state(game_state_t *state, int (*add_food)(game_state_t *state)) {
  // TODO: Implement this function.
  for (int i = 0; i < state->num_snakes; ++i) {
    if (state->snakes[i].live) {
        char next_char = next_square(state, i);
        if (next_char == '#' || is_snake(next_char)) {
            state->snakes[i].live = false;
            set_board_at(state, state->snakes[i].head_row, state->snakes[i].head_col, 'x');
        }
        else {
            if (next_char != '*') {
                update_tail(state,i);    
            }
            else {
                add_food(state);
            }
            update_head(state, i);
            
        }
    }
  }
}

/* Task 5.1 */
char *read_line(FILE *fp) {
  // TODO: Implement this function.

    if (fp == NULL) {
        return NULL;
    }

    size_t size = 128;
    size_t len = 0;
    char *stream = (char*)malloc(size);


    while (1) {

       if (fgets(stream + len, size - len, fp) == NULL) {
           if (len == 0) {
                free(stream);
                return NULL;
           }
        break;
       }
len+= strlen(stream + len);

        if (stream[len -1] == '\n' || stream[len -1] == '\0') 
        {
            break;
        }

        
        
        size *= 2;
        char* new_stream = (char*)realloc(stream, size);
        stream = new_stream;

    }
//  len = strlen(stream);


    return stream;
}

/* Task 5.2 */
game_state_t *load_board(FILE *fp) {


  game_state_t *state = (game_state_t*) malloc(sizeof(game_state_t));

//    unsigned int rows = 0;
    size_t rows_alloc = 8;

    state->num_rows = 0;
    state->snakes = NULL;
    state->num_snakes = 0;


    state->board = (char**)malloc(rows_alloc * sizeof(char*));
    char *line;

while ((line = read_line(fp)) != NULL) {
   if (state->num_rows >= rows_alloc) {
        rows_alloc *=2;
        char** new_board = (char**)realloc(state->board, rows_alloc * sizeof(char *));
        state->board = new_board;
   }

   state->board[state->num_rows] = line;
   state->num_rows++;
//   size_t row_size = strlen(row);
//    state->board[rows] = malloc((row_size +1) * sizeof(char));

}

return state;

}

/*
  Task 6.1

  Helper function for initialize_snakes.
  Given a snake struct with the tail row and col filled in,
  trace through the board to find the head row and col, and
  fill in the head row and col in the struct.
*/
static void find_head(game_state_t *state, unsigned int snum) {
  // TODO: Implement this function.
  snake_t *snake = &state->snakes[snum];
  unsigned int row = snake->tail_row;
  unsigned int col = snake->tail_col;

  char c = get_board_at(state, row, col);

  while (is_snake(c) && !is_head(c)) {
    row = get_next_row(row, c);
    col = get_next_col(col, c);
    c = get_board_at(state, row, col);
  }

  snake->head_row = row;
  snake->head_col = col;
}

/* Task 6.2 */
game_state_t *initialize_snakes(game_state_t *state) {
  
  unsigned int num_of_snakes = 0;

  for (unsigned int i = 0; i < state->num_rows; ++i) {
    for (unsigned int j = 0; j < strlen(state->board[i]); ++j) {
        if (is_head(state->board[i][j])) {
            num_of_snakes+=1;
        }
    }
  }

  state->snakes = malloc(num_of_snakes * sizeof(snake_t));
  state->num_snakes = num_of_snakes;

  unsigned int snum = 0;

  for (unsigned int i = 0; i < state->num_rows; ++i) {
    for (unsigned int j = 0; j < strlen(state->board[i]); ++j) {
        if (is_tail(state->board[i][j])) {
            state->snakes[snum].tail_row = i;
            state->snakes[snum].tail_col = j;
            find_head(state, snum);
            state->snakes[snum].live = true;
            snum+=1;
        }
    }
  }
  return state;
}
