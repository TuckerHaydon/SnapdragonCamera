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
## Helper Scripts
Helper scripts are available in the [scripts](scripts/) folder. Use these
scripts to crop and convert the raw images to jpeg on the snapdragon flight.
Examine their usage with the 'help' command. May require installation of avconc
and imagemagick.

```bash
./nv12-to-jpg-linaro.bash --help
Examine their usage with the 'help' command.
```bash
./nv12-to-jpg-linaro.bash --help
./remove-green-bar-linaro.bash --help
```

### Tips
- Saving raw images requires less CPU usages, but greater IO usage. Choose the
  strategy you want depending on your system needs.
- Adjusting the jpeg compression parameter adjusts affects both the frame save
  rate and the frame save size. Choose the compression ratio to fit your disk
  and framerate needs.
- If you are ssh'd into the snapdragon with X11 (ssh -Y linaro@), you can
  quickly view jpg images with ```eog frame00001.jpg```.
