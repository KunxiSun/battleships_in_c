#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BUF_LEN (100)


void print_board(char board[8][8]) {
  puts("\n  1 2 3 4 5 6 7 8");  // print x coordinate
  for (int i = 0; i < 8; i++) {
    printf("%c|", i + 'A');
    for (int j = 0; j < 8; j++) {
      printf("%c|", board[i][j]);  // print y coordinate
    }
    puts("");  // print the next line
  }
  puts("");
}

struct ship {
  char name[20];
  int cells;
  char short_name;
};

struct ship ships[5] = {{"Carrier", 5, 'C'},
                        {"Battleship", 4, 'B'},
                        {"Destroyer", 3, 'D'},
                        {"Submarine", 2, 'S'},
                        {"Patrol Boat", 2, 'P'}};

int findShipByShortName(char short_name) {
  for (int i = 0; i < 5; i++) {
    if (ships[i].short_name == short_name) {
      return i;
    }
  }
  return -1;
}

int check_coordinate(char x, int y) {
  return x >= 'A' && x <= 'H' && y >= 1 && y <= 8;
}

int check_overlap(char board_player[8][8], char x, int y, char o, int len){
    if (o == 'V') {
      for (int j = 0; j < len; j++) {
        if(board_player[x - 'A' + j][y - 1]!=' '){
            return 0;
        }
      }
    } else if (o == 'H') {
      for (int j = 0; j < len; j++) {
        if(board_player[x - 'A'][y + j - 1]!=' '){
            return 0;
        }
      }
    }
    return 1;
}

void configure_ships(char board_player[8][8]) {
  char input[10];
  char x, o;
  int y;
  for (int i = 0; i < 5; i++) {
    while (1) {
      printf("%s: ", ships[i].name);
      fgets(input, 10, stdin);
      sscanf(input, "%c %d %c", &x, &y, &o);
      
      if (check_coordinate(x, y) && (o == 'V' || o == 'H')&&check_overlap(board_player,x,y,o,ships[i].cells)) {
        break;
      } else {
        puts("Invalid ship configuration");
      }
    }

    if (o == 'V') {
      // check availble
      for (int j = 0; j < ships[i].cells; j++) {
        board_player[x - 'A' + j][y - 1] = ships[i].name[0];
      }
      // print_board(board_player);
    } else if (o == 'H') {
      // check availble
      for (int j = 0; j < ships[i].cells; j++) {
        board_player[x - 'A'][y + j - 1] = ships[i].name[0];
      }
      // print_board(board_player);
    }
  }
}

void configuration(char board_player1[8][8], char board_player2[8][8]) {
  puts("Player 1, please set up your ships (y, x, orientation)");
  configure_ships(board_player1);
  puts("\n");
  puts("Player 2, please set up your ships (y, x, orientation)");
  configure_ships(board_player2);
  puts("\nAll boats set up, Player 1 can now strike.\n");
}

void fire(char board[8][8], char player[8][8], char x, int y) {
  char c = player[x - 'A'][y - 1];
  if (c == ' ') {
    board[x - 'A'][y - 1] = '0';
    puts("\nYou have missed!\n");
  } else {
    player[x - 'A'][y - 1] = 'X';
    board[x - 'A'][y - 1] = 'X';

    int sunk = 1;
    for (int i = 0; i < 8; i++) {
      if (player[i][y - 1] == c) {
        sunk = 0;
        break;
      }
    }
    for (int i = 0; i < 8; i++) {
      if (player[x - 'A'][i] == c) {
        sunk = 0;
        break;
      }
    }
    if (sunk == 0) {
      puts("\nWe have hit the target!\n");
    } else if (sunk == 1) {
      printf("\nWe have sunk your %s!\n\n", ships[findShipByShortName(c)].name);
    }
  }
}

int check_fired(int x, int y, char board1[8][8], char board2[8][8], int turn) {
  if (turn == 0) {
    if (board1[x - 'A'][y - 1] == ' ') {
      return 0; //not fired
    } else {
      return 1; //fired
    }
  } else {
    if (board2[x - 'A'][y - 1] == ' ') {
      return 0;
    } else {
      return 1;
    }
  }
}

int has_won(char player1[8][8], char player2[8][8], int turn) {
  if (turn == 0) {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        char c = player2[i][j];
        if (c == 'C' || c == 'B' || c == 'D' || c == 'S' || c == 'P') {
          return 0;
        }
      }
    }
    return 1;
  } else {
    for (int i = 0; i < 8; i++) {
      for (int j = 0; j < 8; j++) {
        char c = player1[i][j];
        if (c == 'C' || c == 'B' || c == 'D' || c == 'S' || c == 'P') {
          return 0;
        }
      }
    }
    return 1;
  }
}

int main() {
  int turn = 0;
  char board1[8][8];
  char board2[8][8];
  char board_player1[8][8];
  char board_player2[8][8];
  char buf[BUF_LEN];
  char x;
  int y;
  char command[10];

  memset(board1, ' ', 64);  // fill our cell with one space
  memset(board2, ' ', 64);
  memset(board_player1, ' ', 64);
  memset(board_player2, ' ', 64);

  configuration(board_player1, board_player2);

  while (fgets(buf, BUF_LEN, stdin)) {
    sscanf(buf, "%s %c %d", command, &x, &y);
    // printf("%s %c %d\n", command, x, y);

    if (strcmp(command, "exit") == 0) {
      return 0;
    } else if (strcmp(command, "shots") == 0) {
      if (turn == 0) {
        print_board(board1);
      } else {
        print_board(board2);
      }
    } else if (strcmp(command, "fire") == 0) {
      if (check_coordinate(x, y)) {
        if (check_fired(x, y, board1, board2, turn) == 0) {
          if (turn == 0) {
            fire(board1, board_player2, x, y);
          } else {
            fire(board2, board_player1, x, y);
          }

          if (has_won(board_player1, board_player2, turn) == 0) {
            turn = (turn + 1) % 2;
            if (turn == 0) {
              puts("Player 1's turn\n");
            } else {
              puts("Player 2's turn\n");
            }
          } else {
            printf("Player %d wins!\n", turn + 1);
            return 0;
          }
        } else {
          puts("You have already fired at this location");
        }
      } else {
        puts("Invalid coordinates");
      }
    } else {
      puts("Unrecognised command");
    }
  }
}