# FileReadSpeedTest

FileReadSpeedTest is a tool for capturing file read speeds so the behavior can be analyzed and later replayed for performance testing against real-world scenarios.

This tool finds the optimal buffer size for a given drive & file system, then reports the time taken for each buffer to be loaded.
This allows replaying the loading times, which enabled realistic performance testing for processing the loaded file.

[![Build and test](https://github.com/ProgramMax/FileReadSpeedTest/actions/workflows/build-and-test.yaml/badge.svg)](https://github.com/ProgramMax/FileReadSpeedTest/actions/workflows/build-and-test.yaml)

An example output using the provided test file:
```
Worker thread count: 6
Clock is steady
Buffer size: 4096
Buffer 0 - Read issue delay: 100ns - Issue to completion delay: 51600ns
Buffer 1 - Read issue delay: 45200ns - Issue to completion delay: 19000ns
Buffer 2 - Read issue delay: 54600ns - Issue to completion delay: 18500ns
Buffer 3 - Read issue delay: 63900ns - Issue to completion delay: 14700ns
Buffer 4 - Read issue delay: 72700ns - Issue to completion delay: 18600ns
Buffer 5 - Read issue delay: 82300ns - Issue to completion delay: 17800ns
Buffer 6 - Read issue delay: 95800ns - Issue to completion delay: 11500ns
Buffer 7 - Read issue delay: 102900ns - Issue to completion delay: 11600ns
Buffer 8 - Read issue delay: 110200ns - Issue to completion delay: 15600ns
Buffer 9 - Read issue delay: 118600ns - Issue to completion delay: 16700ns
Buffer 10 - Read issue delay: 128300ns - Issue to completion delay: 14700ns
Buffer 11 - Read issue delay: 137000ns - Issue to completion delay: 77900ns
Buffer 12 - Read issue delay: 145900ns - Issue to completion delay: 16000ns
Buffer 13 - Read issue delay: 154900ns - Issue to completion delay: 60000ns
Buffer 14 - Read issue delay: 163500ns - Issue to completion delay: 95400ns
```

## Features

## Documentation

## Support

* Compilers:
    * Visual C++
* OSes:
    * Windows

If you would like FileReadSpeedTest to support more contacts us and let us know.

## Engage

* **Community:** We have a welcoming community which follows the [Code of Conduct](code_of_conduct.md).
* **Contribute:** We accept pull requests. Take a look at some [good first tasks](https://github.com/ProgramMax/FileReadSpeedTest/issues?q=is%3Aissue+is%3Aopen+label%3A"good+first+issue").
* **Support:** You can [report bugs](https://github.com/ProgramMax/FileReadSpeedTest/issues/new?assignees=&labels=&template=bug_report.md&title=) and [request changes](https://github.com/ProgramMax/FileReadSpeedTest/issues/new?assignees=&labels=&template=feature_request.md&title=) using GitHub issues.
