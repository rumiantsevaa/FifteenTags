# FifteenTags
This code implements a sliding puzzle game, commonly known as the "15 puzzle" or "N-puzzle." The user can choose the board size (3x3 or 4x4) and select between two game modes: "Human" or "Computer."
Create a priority queue to store nodes (configurations of the puzzle).
Create a set to keep track of visited nodes.
Initialize the start node with the initial board configuration, empty space position, g (cost) and h (heuristic) values.
Push the start node into the priority queue.
While the priority queue is not empty:
a. Pop the node with the lowest f value (g + h) from the priority queue.
b. If the node's board configuration matches the target board, the puzzle is solved. Update the board and exit.
c. Add the current board configuration to the set of visited nodes.
d. Generate all possible moves from the current empty space position.
e. For each possible move:
i. Create a new node with the moved tag and calculate its g and h values.
j. Swap the tags in the board configuration.
k. If the new configuration is not visited, push it into the priority queue.
If the puzzle is solved, display the number of moves and the time taken.
