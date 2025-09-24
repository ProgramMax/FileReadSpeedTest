# FileReadSpeedTest

FileReadSpeedTest is a tool for measuring file read speeds so it can later be mimicked.

This tool finds the optimal buffer size for a given drive & file system, then reports the time taken for each buffer to be loaded.
This allows replaying the loading times, which enabled realistic performance testing for processing the loaded file.

[![Build and test](https://github.com/ProgramMax/FileReadSpeedTest/actions/workflows/build-and-test.yaml/badge.svg)](https://github.com/ProgramMax/FileReadSpeedTest/actions/workflows/build-and-test.yaml)

An example output using the provided test file:

Worker thread count: 6
Clock is steady
Buffer size: 4096
Context 0 - 39000ns
Context 1 - 13300ns
Context 2 - 6200ns
Context 3 - 11600ns
Context 4 - 11600ns
Context 5 - 11500ns
Context 6 - 11800ns
Context 7 - 10400ns
Context 8 - 11600ns
Context 9 - 9800ns
Context 10 - 15100ns
Context 11 - 10900ns
Context 12 - 15400ns
Context 13 - 12500ns
Context 14 - 27400ns

## Features

## Documentation

## Support

* Compilers:
    * Visual C++
* OSes:
    * Windows

If you would like max to support more contacts us and let us know.

## Engage

* **Community:** We have a welcoming community which follows the [Code of Conduct](code_of_conduct.md).
* **Contribute:** We accept pull requests. Take a look at some [good first tasks](https://github.com/ProgramMax/FileReadSpeedTest/issues?q=is%3Aissue+is%3Aopen+label%3A"good+first+issue").
* **Support:** You can [report bugs](https://github.com/ProgramMax/FileReadSpeedTest/issues/new?assignees=&labels=&template=bug_report.md&title=) and [request changes](https://github.com/ProgramMax/FileReadSpeedTest/issues/new?assignees=&labels=&template=feature_request.md&title=) using GitHub issues.