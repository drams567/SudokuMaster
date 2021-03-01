# SudokuMaster

**Set of classes used to solve, generate, and analyze sudoku puzzles efficiently.**
<br>
<br>
Driver used for testing and demonstration purposes. Main components consist of the following three classes:
<ul>
  <li>SMaster - Used for solving puzzles. Capable of solving regardless of number of 'hints'. Will accept and solve and empty board.</li>
  <li>SGen (in development) - Child class of SMaster used for randomly generating puzzles.</li>
  <li>SChecker (in development) - Utility class for checking the state of a puzzle; in a winning state, contains contradications, unfinished, contains rule violations, etc.</li>
</ul>
<br>
Notes: All implementation has been generalized to work for any sized 9 region (i.e. divisible by 3) puzzle, simply adjust N.
