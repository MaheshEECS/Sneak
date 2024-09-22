#include <stdio.h>
#include <string.h>

#include "snake_utils.h"
#include "state.h"

int main(int argc, char *argv[]) {
  bool io_stdin = false;
  char *in_filename = NULL;
  char *out_filename = NULL;
  game_state_t *state = NULL;

  // Parse arguments
  for (int i = 1; i < argc; i++) {
    if (strcmp(argv[i], "-i") == 0 && i < argc - 1) {
      if (io_stdin) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      in_filename = argv[i + 1];
      i++;
      continue;
    } else if (strcmp(argv[i], "--stdin") == 0) {
      if (in_filename != NULL) {
        fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
        return 1;
      }
      io_stdin = true;
      continue;
    }
    if (strcmp(argv[i], "-o") == 0 && i < argc - 1) {
      out_filename = argv[i + 1];
      i++;
      continue;
    }
    fprintf(stderr, "Usage: %s [-i filename | --stdin] [-o filename]\n", argv[0]);
    return 1;
  }

  // Do not modify anything above this line.

  /* Task 7 */

  // Read board from file, or create default board
  FILE *in_file = NULL;
  if (in_filename != NULL) {
    // TODO: Load the board from in_filename
    // TODO: If the file doesn't exist, return -1
    // TODO: Then call initialize_snakes on the state you made
    in_file = fopen(in_filename, "r");
    if (!in_file) {
        return -1;
    }

    state = load_board(in_file);
    fclose(in_file);
    state = initialize_snakes(state);
  } else if (io_stdin) {
    // TODO: Load the board from stdin
    // TODO: Then call initialize_snakes on the state you made
    state = load_board(stdin);
    state = initialize_snakes(state);
  } else {
    // TODO: Create default state
    state = create_default_state();
  }

  // TODO: Update state. Use the deterministic_food function
  // (already implemented in snake_utils.h) to add food.
  update_state(state, deterministic_food);
  // Write updated board to file or stdout
  FILE *out_file = NULL;
  if (out_filename != NULL) {
    // TODO: Save the board to out_filename
    out_file = fopen(out_filename, "w");
    if (out_file) {
        print_board(state, out_file);
        fclose(out_file);
    }
    
  } else {
    // TODO: Print the board to stdout
    print_board(state, stdout);
  }

  // TODO: Free the state
  free_state(state);

  return 0;
}
