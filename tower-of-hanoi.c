/*
 *	to solve the tower of hanoi you move in a clockwise, instruction pointer system, where the current pointer will move the topmost disc to the left stick if the focused tower height is even,
 *	and to the right stick if the tower is odd,
 *	the clockwise rotation works as if the disc is moved right, the instruction pointer is moved right, and scans the topmost focused disc for any left/right movements, then moves to the next one
 *
 *	each solution should take 2^(n)-1 operations, O(2^n)
 *
 */

// started: 9/12/24
// completed: 10/12/24

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <math.h>

void 		solve(unsigned int, unsigned int**, unsigned int, unsigned int, unsigned int**);
void 		solve_tower(unsigned int);
unsigned int	get_left_tower(unsigned int);
unsigned int	get_right_tower(unsigned int);
unsigned int	tower_height(unsigned int, unsigned int **const, unsigned int);
bool		possible_move(unsigned int, unsigned int **const, unsigned int, unsigned int);
unsigned int	top_element(unsigned int** const, unsigned int, unsigned int);
unsigned int** 	left_move_top(unsigned int**, unsigned int, unsigned int height);
unsigned int**	right_move_top(unsigned int**, unsigned int, unsigned int height);

unsigned int** left_move_top(unsigned int **game, unsigned int tower, unsigned int height)
{	
	unsigned int left_tower = get_left_tower(tower);
	unsigned int curr_top_element = top_element(game, tower, height);
	
	game[tower][height - tower_height(height, game, tower)] = 0;

	if (tower_height(height, game, left_tower) == 0 && game[left_tower][height - 1] == 0)
	{
		game[left_tower][height - 1] = curr_top_element;
		return game;
	};

	if (tower_height(height, game, left_tower) > 0 && game[left_tower][height - 1] != 0)
	{
		game[left_tower][height - tower_height(height, game, left_tower) - 1] = curr_top_element;
		return game;
	};

	return game;
}

unsigned int** right_move_top(unsigned int **game, unsigned int tower, unsigned int height)
{
	unsigned int right_tower = get_right_tower(tower);
	unsigned int curr_top_element = top_element(game, tower, height);

	game[tower][height - tower_height(height, game, tower)] = 0;	
	
	if (tower_height(height, game, right_tower) == 0 && game[right_tower][height - 1] == 0)
	{
		game[right_tower][height - 1] = curr_top_element;
		return game;
	};

	if (tower_height(height, game, right_tower) > 0 && game[right_tower][height - 1] != 0)
	{
		game[right_tower][height - tower_height(height, game, right_tower) - 1] = curr_top_element;
		return game;
	};

	return game;
};

unsigned int get_left_tower(unsigned int tower)
{// maybe handle error cases?
	switch (tower)
	{
		case 0:
			return 1;
		case 1:
			return 0;
		case 2:
			return 0;
	};

	return -1; // error
} // get_left_tower

unsigned int get_right_tower(unsigned int tower)
{
	switch (tower)
	{
		case 0:
			return 2;
		case 1:
			return 2;
		case 2:
			return 1;
	};

	return -1; // error
} // get_right_tower

// brief: Returns the topmost element of the specified tower
unsigned int top_element(unsigned int **const game, unsigned int tower, unsigned int height)
{
	unsigned int val = 0;
	int index = height - 1;

	while (index != -1 && game[tower][index] != 0)
	{
		val = game[tower][index];
		index -= 1;
	};
	
	return val;
} // top_element

// brief: Returns the height of the specified tower
unsigned int tower_height(unsigned int height, unsigned int **const game, unsigned int tower)
{
	unsigned  int tower_height = 0;

	for (unsigned int i = 0; i < height; i++)
	{
		if (game[tower][i] != 0)
			++tower_height;
	};

	return tower_height;
} // tower_height

// brief: Checks if there exists a possible move for the current tower
bool possible_move(unsigned int height, unsigned int **const game, unsigned int tower, unsigned int next_tower)
{
	if (game[tower][height - 1] == 0) return false;

	// FIX POSSIBLE MOVE CHECK FOR SIZE(4)

	unsigned int left_tower = get_left_tower(tower);
	unsigned int right_tower = get_right_tower(tower);

	unsigned int left_final = top_element(game, left_tower, height);
	unsigned int right_final = top_element(game, right_tower, height);
	unsigned int curr_final = top_element(game, tower, height);

	if (game[left_tower][height - 1] == 0) return true;
	if (game[right_tower][height - 1] == 0) return true;

	if (next_tower == 1 && right_final > curr_final) return true;
	if (next_tower == 0 && left_final > curr_final) return true;

	return false;
} // possible_move

// brief: Solving algorithm for tower of hanoi
void solve(unsigned int height, 
		unsigned int **game, 
		unsigned int number_of_operations, 
		unsigned int instruction_index,
		unsigned int **solution
		)
{
	printf("\nNew Iteration\nNumber Of Operations: %d\nInstruction Index: %d\n", number_of_operations, instruction_index);	
	
	for (unsigned int i = 0; i < height; i++)
	{
		for (unsigned int j = 0; j < 3; j++)
		{
			printf("%d", game[j][i]);
		};
		printf("    Index: %d", i);
		printf("\n");
	};

	

	// base case
	bool solved = false;
	for (unsigned int i = 0; i < height; i++)
	{
		if (game[0][i] == solution[0][i] &&
				game[1][i] == solution[1][i] &&
				game[2][i] == solution[2][i])
		{
			solved = true; 
		} else solved = false;
	};

	if (solved == true)
	{
		printf("Solved!\n");
		return;
	};

	if (number_of_operations >= pow(2, height) - 1)
	{
		printf("Error with solving algorithm");
		return;
	};

	if (possible_move(height, game, instruction_index, tower_height(height, game, instruction_index) % 2))
	{
		if (tower_height(height, game, instruction_index) % 2 == 0)
		{
			printf("moving left\n");
			game = left_move_top(game, instruction_index, height);

			unsigned int old_ins_index = get_left_tower(instruction_index);
			unsigned int new_ins_index = get_left_tower(instruction_index);
			++new_ins_index;
			if (++old_ins_index == 3) new_ins_index = 0;

			solve(height, game, ++number_of_operations, new_ins_index, solution);
		} else if (tower_height(height, game, instruction_index) % 2 == 1) {
			printf("moving right\n");
			game = right_move_top(game, instruction_index, height);

			unsigned int old_ins_index = get_right_tower(instruction_index);
			unsigned int new_ins_index = get_right_tower(instruction_index);
			
			++new_ins_index;
			if (++old_ins_index == 3) new_ins_index = 0;
			
			solve(height, game, ++number_of_operations, new_ins_index, solution);
		};
	} else {
		printf("No Possible Move\n");
		if (instruction_index == 2)
		{
			solve(height, game, number_of_operations, 0, solution);
		} else {
			solve(height, game, number_of_operations, ++instruction_index, solution);
		};	
	};
} // solve

void solve_tower(unsigned int height)
{
	if (height == 1 || height == 0)
	{
		printf("bruh\n");
		return;
	};

	// initialize an array of arrays
	unsigned int **game = (unsigned int**)malloc(sizeof(unsigned int*) * 3);
	unsigned int **const solution = (unsigned int**)malloc(sizeof(unsigned int*) * 3);
	for (unsigned int i = 0; i < 3; i++)
	{
		game[i] = calloc(height, sizeof(unsigned int*));
		solution[i] = calloc(height, sizeof(unsigned int*));
	};

	for (unsigned int i = 0; i < height; i++)
	{
		game[0][i] = i + 1;
		solution[2][i] = i + 1;
	};	

	solve(height, game, 0, 0, solution);


} // solve_tower

int main(int argc, char **argv) 
{
	solve_tower(9);
}
