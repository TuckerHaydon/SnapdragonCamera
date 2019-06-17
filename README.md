# Snapdragon Camera
Save images captured from a Snapdragon Flight's camera and associated metadata to disk

## Install
### Pre-Requisites
Install ROS. Generally follow [this install guide](http://wiki.ros.org/indigo).
May have broken dependencies on the snapdragon flight.

Install opencv. May have broken dependencies on the snapdragon flight. See [this
thread](https://developer.qualcomm.com/forum/qdn-forums/hardware/snapdragon-flight/32904) for a potential fix.
```bash
sudo apt install libopencv-dev
```

### Clone and Build
```bash
git clone https://github.com/tuckerhaydon/SnapdragonCamera.git
cd SnapdragonCamera
mkdir build && cd build
cmake ..
make -j4
```

## Usage
Help menu
```bash
cd SnapdragonCamera/build
./src/exe/capture --help
```

Example program. Capture 4k raw images to disk. Must run as root to use the
camera.
```bash
cd SnapdragonCamera/build
sudo ./src/exe/capture --odom_topic /phoenix/ENU/local_odom
```

