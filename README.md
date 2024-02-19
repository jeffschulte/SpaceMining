# SpaceMining
WARNING!
Read the README.md your own computer after pulling. I'm having an issue with github not printing them properly!

Description
The SpaceMining simulation is a program desgined to simulate the operaton of a team of Lunar mining trucks mining for Helium-3 on the Moon!

Trucks are simulated to mine at one of infinite many mining sites on the moon (not a lot of variation of there!).
A truck will mine at a given site until it finishes a load.
After mining a load, they travel to a station, where they wait in a queue for other trucks to finish unloading before unloading their own load.
After unloading, they travel back to a site and start the process over again.
The simulation is tracked in minutes. Its takes them 5 minutes to unload, 30 minutes to travel to and from sites, and 72*60 minutes for the entire simulation.
At the end of the simulation, statistics of how things went are printed to the terminal, and an option log of every event that occured can be printed to a file.
The program allows for a configurable number of trucks and stations.

The code is based on the central concept of an eventList, which is a sorted list of things that happened over the entire operation, sorted based on when that event will occur. The core function driving the simulation is the HandleEvent() function, the class MiningManager. The core function will pick the
next event on the eventList (the first element on the list is the next event to occur), will process that event, and then insert a new event onto the list.
This method allows for a Discrete Event Simulation style. The eventList covers all events, all trucks, all stations, so the application is able to be single threaded.

Testing is done using CTest

Dependencies
Building and running the project requires CMake version 3.1 and C++14 or higher

Installing
git clone https://github.com/jeffschulte/SpaceMining.git
cd SpaceMining
cmake -S . -B build
cd build
make

If on windows, the cmake and make commands will be:
"<path>/cmake.exe" -S . -B build
"<path>\msbuild.exe" ALL_BUILD.vcxproj

Executing
cd src
./spaceMining -m <numberOfStations> -n <numberOfTrucks> -f <logFileName>
the -f argument is optional

If on windows:
cd src/Debug/
./spaceMining.exe -m <numberOfStations> -n <numberOfTrucks> -f <logFileName>
the -f argument is optional

Executing Tests
cd test/Debug/
./spaceMiningTets.exe <testName>

Contributing
Anyone can contribute!

Author
Jeff Schulte

Version History
1.0: Initial Release

License
This project is licensed under the MIT License - see the LICENSE.md file for details
