# Robot Arm
This was a graphics project which aimed to create a robot arm which can move in certain ways using keyboard shortcuts and to also integrate limitations to the movement on the arm. These limitations are to disable the arm from rotating a full 360 degree angle. This project is created in `SFML` so to run it, you must first download the library. In terminal run this command `sudo apt-get install libsfml-dev`

# Compiling
1. First compile the code: `g++ -c main.cpp`
2. Next link the compiled file with SFML libraries: `g++ main.o -o sfml-app -lsfml-graphics -lsfml-window -lsfml-system`
3. Finally run the code: `./sfml-app`

# Keyboard Shortcuts
* `a` and `s` to rotate the entire arm
* `z` and `x` to rotate the fore-arm
* `w` to open the claws
* `q` to close the claws
