# CS210 PA2 – Story Tree Reflection

## Question 1: Functions and Time Complexity

Here, n represents the total number of scenes in the story tree.

### Scene.h

**TODO 1 – Constructor**

The constructor allocates an array with exactly the required number of choices. Every pointer is initialized to nullptr. An ending does not require an array.

Time Complexity: O(1), because a scene has at most three choices.

**TODO 2 – Destructor**

The destructor deletes all child scenes recursively, deletes the choices array, and finally deletes the scene's data.

Time Complexity: O(n).

**TODO 3 – isEnding**

This function checks whether the current scene has zero choices.

Time Complexity: O(1).

**TODO 4 – getChoice**

This function returns the child scene at the requested index. It returns nullptr if the index is invalid.

Time Complexity: O(1).

**TODO 5 – setChoice**

This function stores a child scene at the specified index after checking that the index is valid.

Time Complexity: O(1).

### StoryTree.h

**TODO 6 – countScenes**

This recursive function counts the current scene and all its descendant scenes.

Time Complexity: O(n).

**TODO 7 – countEndings**

This recursive function counts all scenes that have no choices.

Time Complexity: O(n).

**TODO 8 – height**

This recursive function calculates the longest path from the current scene to an ending. An empty tree has a height of -1.

Time Complexity: O(n).

**TODO 9 – clear**

This function deletes the root scene, which recursively deletes the entire tree. It then sets the root pointer to nullptr.

Time Complexity: O(n).

### Adventure.cpp

**TODO 10 – load**

This function opens the story file, reads the starting HP, and calls readScene to construct the story tree. It returns false if loading fails.

Time Complexity: O(n).

**TODO 11 – readScene**

This recursive function reads the scene description, HP, loot, item, and choices. It constructs the child scenes and rejects invalid choice counts.

Time Complexity: O(n).

**TODO 12 – askChoice**

This function reads the player's input and checks whether it is a valid choice. Invalid input causes the function to ask again.

Time Complexity: O(1) per attempt with bounded input length.

**TODO 13 – play**

This function allows the player to move through the story. It updates HP, loot, and inventory. The game ends when the player dies or reaches an ending.

Time Complexity: O(n²) in the worst case because inventory searches may scan an increasingly large linked list.

**TODO 14 – printStats**

This function displays the total number of scenes, endings, and the longest playthrough.

Time Complexity: O(n).

**TODO 15 – printSpoilers**

This function starts the recursive process of displaying every ending and its path.

Time Complexity: O(n) for scene traversal.

**TODO 16 – printEndings**

This recursive function visits the story tree and prints every ending with its corresponding choice path.

Time Complexity: O(n) for scene traversal.

**TODO 17 – greedyRun**

This function selects the next scene with the highest immediate loot. It does not consider future rewards or health before choosing.

Time Complexity: O(n) for scene traversal.

**TODO 18 – bestLoot**

This recursive function explores possible routes and finds the maximum loot that can be collected while keeping the player alive. It returns -1 when no surviving route exists.

Time Complexity: O(n) for scene traversal.

**TODO 19 – optimalRun**

This function calls bestLoot and displays the best surviving path and its total loot.

Time Complexity: O(n) for scene traversal.

Complexity note: The traversal-based complexities count scene visits. Copying and extending path strings can increase the actual worst-case running time to O(n²) in a deeply nested tree. File-loading complexities assume bounded field lengths.

## Question 2: Dynamic Choice Allocation

Each scene allocates exactly the number of choices it requires instead of using a fixed array of three pointers.

This avoids allocating unnecessary choice slots. For example, a scene with one choice only needs one pointer.

An ending has zero choices, so its choices_ pointer remains nullptr.

## Question 3: Scene Destructor

The destructor releases memory in the following order:

1. Delete every child scene recursively.
2. Delete the dynamically allocated choices array.
3. Delete the scene's data.

The child scenes must be deleted before the choices array because the array contains the pointers needed to access them.

Deleting the array first would make those pointers inaccessible and could cause memory leaks or invalid memory access.

## Question 4: Greedy vs Optimal

My story is about a hero searching for legendary treasure in a haunted forest.

The story contains 15 scenes, 8 endings, and a maximum height of 5.

### Greedy Result

- Path: 3-1
- Total Loot: 19
- Result: Died

The greedy advisor chooses the mountain because it provides the highest immediate loot. It then fights the dragon for another reward. However, the dragon causes enough damage to kill the hero.

### Optimal Result

- Path: 2-1-1-1-1
- Total Loot: 53
- Result: Reached an ending
- Final HP: 18

The optimal advisor follows the magical river, restores health, crosses the wooden bridge, enters the treasure room, opens the secret chest, and discovers the legendary treasure.

Greedy loses because it only considers immediate rewards. The optimal advisor considers future rewards and survival.

## Question 5: Why bestLoot Is Not Exponential

Although bestLoot explores every possible route, the story is a tree rather than a graph with shared or repeated nodes.

Every scene has exactly one path from the root. The recursive function visits each reachable scene at most once and checks at most three children.

Therefore, the number of scene visits is proportional to the total number of scenes, which is O(n).

Path-string operations may add additional copying costs, but the tree traversal itself is linear.

## Question 6: Inventory List Methods

My game uses the following List methods:

- search(): Checks whether an item already exists in the inventory.
- addFront(): Adds a new item to the inventory.
- size(): Returns the number of collected items.
- print(): Displays all collected items.

The search function checks the linked list one item at a time.

If the inventory contains m items, one search takes O(m) time in the worst case.

Using search prevents duplicate items from being added to the player's inventory.