            .__                       __         .__                  
       _____|__| ____   _____ _____ _/  |_  ____ |  |__   ___________ 
      /  ___/  |/ ___\ /     \\__  \\   __\/ ___\|  |  \_/ __ \_  __ \
      \___ \|  / /_/  >  Y Y  \/ __ \|  | \  \___|   Y  \  ___/|  | \/
     /____  >__\___  /|__|_|  (____  /__|  \___  >___|  /\___  >__|   
          \/  /_____/       \/     \/          \/     \/     \/       

Various signature matching algorithms.

## Usage
You can look in the `test` directory to check out a few examples of how to use sigmatcher.
In general, the following exmpale depict a common usage pattern:
```c++
#include <iostream>
#include "signature_matcher_factory.h"

int main()
{
    // Create a signature matcher instance. See SignatureMatcherType for a list
    // of supported matchers.
    auto sig_matcher =
        sigmatcher::SignatureMatcherFactory::Create(sigmatcher::SignatureMatcherType::SMT_CRC32);

    // Add a file
    sig_matcher->Add("/path/to/some/file");

    // Check if a file matches a known one
    if (sig_matcher->Check("/path/to/another/file"))
    {
        std::cout << "Match!";
    }

    // Store the matcher's internal DB into persistent sotrage
    if (!sig_matcher->Serialize("signatures.db"))
    {
        std::cout << "Failed to serialize file";
        return -1;
    }

    // Load a DB into memory
    if (!sig_matcher->Deserialize("signatures.db"))
    {
        std::cout << "Failed to deserialize file";
        return -1;
    }

    return 0;
}
```

## Pre-requisite:
- boost/crc
- boost/test
- openssl
- CMake
