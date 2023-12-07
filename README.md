# CallHandlingCenter
CallHandlingCenter is a service that accepts HTTP requests, distributes them to operators, and emulates their response.

Done as coursework from Protei IT-school.

## Prerequisites
cmake, C++20 - compliant compiller, git, conan 2.x

Verified setup: cmake/3.22.1, g++/11.4.0, conan/2.0.14

## Building project
```bash
git clone https://github.com/Owizile/CallHandlingCenter-Protei-CW 
cd CallHandlingCenter-Protei-CW
mkdir build
cd build
cmake .. -DCMAKE_BUILD_TYPE=Debug
cmake --build ./src
cmake --build ./tests
```

To run the service:

```bash
cd src
./CallHandlingCenter
```

You can also use `./build.sh` script which will build the project, run the tests and the service

## Tests
To run the tests:
(from build-directory)
```bash
./tests/CDRTest
./tests/CallQueueTest
./tests/CallCenterConfigTest
```

## Usage information
The service is set up to run on the default address `localhost:8080`. It supports two content types: `plain/text` and `json`.

### Example Usage

#### plain/text

To make a request with `plain/text` content type, you can use a tool like `curl` or an HTTP client. 
Here is an example using `curl`:

```bash
curl -X POST -H "Content-Type: text/plain" -d "<your_call_number_here>" http://localhost:8080
```
Replace <your_call_number_here> with the actual call number you want to use.

#### json

json supported structure:

```
{
 "Call-Number": <your_call_number_here>
}
```