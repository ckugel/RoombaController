# About
This is the high level controller application for the roomba for Computer Engineer 2880. The main repo for our code can be found [here](https://github.com/ckugel/Waste-Cleanup.git).

# Documentation
There is documentation on using the app in the Wiki page and there is documentation on the functions within the app hosted [here](https://ckugel.github.io/RoombaController/docs/html/index.html)

# How to use
The app is fairly straightforward to use. The blue icon on the field is the bot. The red icon's on the field are pillars that we can see on the field.
There are a few buttons that a user can use. They can choose whether to enter autonomous mode or to control the bot themselves. In autonomous mode the bot will SLAM and generate a path to the ore as well as to the "entrance" to the field which is where the bot started.

## How to build and run
Make sure that if you are connecting to the bot that you have BOT_CONNECT set to 1 in src/main.cpp. If you are want to just test with dummy data you can set BOT_CONNECT to 0 in src/main.cpp and run the simple-python....py file in the root directory of the project which starts a sample TCP server to connect to.

Ensure that you have ran ```git submodule init``` and ```git submodule update``` to make sure that you have access to Dear ImGui which this program uses for the grpahical aspect of the app.


If you are using nixos all you need to build the code is to:
1) Have flakes enabled
2) run ```nix develop```
3) ```cmake .```
4) ```cd build```
5) ```make```
6) ```./RoombaController```

If you are trying to run it without a cooresponding bot to connect to, you cna run the python mock server. To do so all you have to do is ensure that in main, Connect to Bot is false. Then run the python script in the root directory of the project. This will start a server that the controller can connect to.


## How to communicate with
Communication with the controller application uses a TCP websocket. On the roomba controller side this just looks like using uart with putty. We accept data primarily for the field, and primarily return instructutions for the bot to execute.

### Field Data (App receives)
Field data starts with an F and is followed by pillar objects. A pillar object is formatted as ```o X Y HEADING Radius```. 
So sending a field would look like: F {pillar} {pillar} ...

#### Example
```F o 27.65 -5.89 0.00 4.29 o 21.75 25.25 0.00 6.56 o 56 -11 0.00 3 F```\
this will send a field where there are 3 pillars (one pillar per o). As these are pillars on the field we can ignore their headings which are all 0 in this case. The first pillar has a radius of 4.29, the second of 6.56, and the last of 3 (all meters).

#### Code to accomplish this
[Graph.c](https://github.com/ckugel/Waste-Cleanup/blob/master/EmbeddedApp/Waste_Management/Graph.c) has a function called **send_pillars_through_putty(Pillar\* pillars, uint8_t size)** which esentially sends a field. I starts and ends the send with F to denote a field.

### Routines (App returns)
The application will send back instructions to the bot in the form of turn to angle and drive magnitude instructions.

### More communication
Can be found under the WIKI tab and How To Communicate with

# Screen shots of what the app looks like
![image](https://github.com/user-attachments/assets/1c1d741a-956f-4118-9f68-b6061a3ca88b)
