Tools / Dependencies needed to be installed for development environment:

- `g++`
- `gcov`
- `lcov`
- `cppcheck`
- `opencv2`

# Programs

To make all the programs provided by this repository, run `make` on the root directory of the repository. The list of programs available are:

- `ip`: Image Processing Program
- `sp`: Puzzle Solving Program
- `pb`: Random Puzzle Generation Program
- `demo`: Demo Program used to show off the capabilities of this project.
- `test`: Unit testing program

The following additional commands are also available in the makefile:

- `coverage`: Run tests and generate unit testing coverage report
- `clean`: Clean the folder of all generated content.

# Testing

To run tests:

    $ make test
    $ bin/test

To run tests and generate code coverage report:

    $ make coverage

The code coverage reports can be found in `coverage/html` folder.

# IP - Image Processor

Set up:
    Ensure that bin/sp is available (see below), because ip needs to call sp to check if a solution to the puzzle is available.
    Ensure that the upload_directory also has an EMPTY folder named "solutions" at direct child level; sp will save the first found solution into the solutions folder.

To run ip:
    $ make ip
    $ bin/ip <image_filename> <upload_directory> (<path_to_sp_bin>?)

    Where:
    <image_filename>:       file name of the puzzle image file to be processed.
    <upload_directory>:     directory path where the output "pieces" file will be stored in.
    <path_to_sp_bin>:       optional param, path to the solver binary file if it is not provided, program will assume it is in bin/sp.

Example:
    $ bin/ip images/puzzle999.jpg  puzzledir  

# SP - Puzzle Solver

Set-up:
    Ensure that there is a "pieces" file (containing information on the puzzle pieces and the container, each represented with an integer array) stored in the puzzle/directory. The "pieces" file is usually created by either ip (image processor) or pb(random puzzle generator).

To run sp:
    $ make sp
    $ bin/sp <puzzle_directory> <state> <debug>3info

    Where:
    <puzzle_directory>:     directory path where the "pieces" file is stored.
    <state>:                string consisting of container width, container height, followed by a string representing the current state of the container.
    <debug>:                1 or 0, prints out debug information if 1 is entered.

    Example: (3 X 4 board with shape piece identified as "2" added)

    0   0   0   0
    2   2   0   0
    2   0   0   0

    $ bin/sp ./puzzledir "4 3 0 0 0 0 2 2 0 0 2 0 0 0" 1

#PB - Random Puzzle Generator

Set Up
    Ensure that IF there is a folder named "solutions" within the current directory (where pb is called) at immediate child level, the "solutions" folder should be empty.

To run pb:
    $ make pb
    $ bin/pb <size_of_puzzle>

Example:
    $ bin/pb 10
    this creates a 10 X 10 random puzzle, with relevant container and pieces information stored in a current directory file named "pieces"  

#Demo - Quick Demonstrative Program  

To run demo:
    $ make demo
    $ bin/demo <image_filename>

Example:
    $ bin/demo images/puzzle999,jpg



# Git Hooks

To install and use the git hooks available for this repository, run the following:

    $ scripts/install-git-hooks.sh

The following hooks are available:

  - `pre-push`: Before pushing to remote `master`, tests will run. In the event a test fails, the push will be rejected.
