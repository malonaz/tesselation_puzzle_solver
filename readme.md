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

# Git Hooks

To install and use the git hooks available for this repository, run the following:

    $ scripts/install-git-hooks.sh
    
The following hooks are available:

  - `pre-push`: Before pushing to remote `master`, tests will run. In the event a test fails, the push will be rejected.