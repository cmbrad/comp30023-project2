/* Connect 4: a simple text based implementation */
/* Implementation by Michael */

#ifndef INCLUDE_CONNECT4_H
#define INCLUDE_CONNECT4_H

	/* number of columns in the game */
#define WIDTH		7

	/* number of slots in each column */
#define HEIGHT		6

	/* number in row required for victory */
#define STRAIGHT	4

	/* sign that a cell is still empty */
#define EMPTY		' '

	/* the two colours used in the game */
#define RED			'R'
#define YELLOW		'Y'

	/* horizontal size of each cell in the display grid */
#define WGRID	5

	/* vertical size of each cell in the display grid */
#define HGRID	3

#define RSEED	876545678

typedef char c4_t[HEIGHT][WIDTH];

void print_config(c4_t);
void init_empty(c4_t);
int do_move(c4_t, int, char);
void undo_move(c4_t, int);
int get_move(c4_t);
int move_possible(c4_t);
char winner_found(c4_t);
int rowformed(c4_t,  int r, int c);
int explore(c4_t, int r_fix, int c_fix, int r_off, int c_off);
int suggest_move(c4_t board, char colour);

#endif
