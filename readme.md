Tools / Dependencies needed to be installed for development environment:

- `g++`
- `gcov`
- `lcov`
- `cppcheck`
- `opencv2`

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