# nonogram

A program that solves any size nonogram puzzle using a constraint satisfaction algorithm of my design.

---

# Intro

Okay, let's talk about Nonograms. They look a little like this, an image I ripped off this [handy site](https://www.puzzle-nonograms.com/) you can generate and play them on:

![image](https://github.com/user-attachments/assets/8504aea0-34ac-4dc0-bda5-e8e88511412b)

If you're not familiar, the goal is to create some sort of pattern of black squares on the grid that satisfies the constraints they give (how quaint, foreshadowing). The constraints come in the form of numbers above and to the left of the grid. They specify the sizes of chunks of black squares. These chunks must be placed at least one square apart, and must fit completely in the grid. To better visualize this, here's the solved version of the above puzzle. The first row has a chunk of 3, the second row has a chunk of 1, the fifth row has a chunk of 2 and a chunk of 1, etc.

![image](https://github.com/user-attachments/assets/0395c741-c456-47ac-90b0-8d142fd9ff3c)

So how might you solve this algorithmically? To answer this, we must go back to 1987, the birth-year of Nonograms.

# Approach

Alright, I don't really care about the history of Nonograms, so let's just talk about the approach. The core of this algorithm is backtracking, but it has to be done a little differently from, say, Sudoku, because in Sudoku what varies between iterations is the value of a cell but here it's grid placement. Sudoku involves trying every number between 1-9 at every cell. Nonogram requires placing each chunk of black squares at every possible position in the row.

The main algorithm is to iterate through each row, place every chunk in the corresponding row constraint, and if a chunk is unable to be placed, backtrack and change the previous chunk's placement. Placing chunks follows certain guidelines, the most important of which being all chunks must be separated by at least one space. Otherwise, they would be part of the same chunk and constitute one number in the constraint.

# Chunk Placement

Say we're given a grid of size 10x10, and the first row has constraints 3, 2, and 1. This means three distinct chunks. Consider only 5. Which spaces can we place it on? For simplicity, let's define its placement based on its leftmost cell. This seems easy, it has (10 - 0) - 3 + 1 = 8 possible placements. (10 - 0) is written this way to highlight that the chunk's leftmost position is 0 from the left of the grid. This is important for later, as the second and third chunk start farther from the left.

But wait. If the 3 chunk is placed too far to the right, the 2 and 1 chunks can't be placed. Order matters here, so we can't place those two chunks before the 3. So how do we figure out the possible range for the 3 chunk? It ends up being (10 - 0) - 3 + 1 - (1 + 1) - (2 + 1) = 8 - 2 - 3 = 3 cells. (1 + 1) and (2 + 1) account for the 2 and 1 chunks at their rightmost positions, tacking on the extra spaces they need to the left. The 2 chunk will have a range of (10 - (3 + 1)) - 2 + 1 - (1 + 1) = 6 - 1 - 2 = 3 cells. I shall leave the 1 chunk as an exercise for the reader, though I really don't care if you do it or not.

This simple arithmetic defines the range for any chunk given its starting position and all chunk sizes that come after it. The algorithm will run through all possible chunk placements per row by testing each chunk at every position in its range. If any chunk reaches the end of its range, we backtrack to the previous chunk and move it up one space. If no placements are possible for the row, we backtrack to the previous row.

# Validity Checking

We've yet to go over how to check if a board is even valid. Consider this board state:

![image](https://github.com/user-attachments/assets/bd0e7ab9-0065-4bc8-a39c-7595f1d430a5)

Wait, no, wrong image. Consider this board state:

![image](https://github.com/user-attachments/assets/e50e6f6e-6c4e-473d-b213-0baf04f5cbbd)

We can easily tell this is valid, but the computer can't because the computer is stupid. What we need to understand is that in order to check a chunk placement, we only need to consider the columns it covers. Any given placement is guaranteed to satisfy row constraints because our algorithm places chunks with them in mind. Let's iterate through each column in the chunk that was just placed and search all cells above it (there won't be any below because our algorithm places chunks top down). The most recent chunk in this case was the 4-long one. Its first column features a 1 2 constraint, but currently we read a 1 1. Uh oh, is this bad?

No! It's actually fine for the bottommost number to be less than its corresponding constraint, because more chunks may be placed below it that complete the constraint. If this wasn't the bottommost chunk, we'd catch it and invalidate it, but it isn't. We can go ahead and move on; everything else ends up being fine. Now consider this board state:

![image](https://github.com/user-attachments/assets/854fc4f3-da03-4dc3-96a6-967684dff835)

The first column is 1 2 but we read 1 3. THIS is bad... current column chunks can be smaller than their constraint but never larger, because no future placements can ever reduce that number. We'll move the bottom 2-long chunk to the right by 1, and try again. No placement will end up being valid, and we'll backtrack to the previous row.

Once we call our recursive backtracking function on an index just past the end of the grid, we know we've found a solution. Any invalid board would have turned back by now.
