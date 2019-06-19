# Notes
Notes taken when developing this repository

## Image Sizes
Images are not the sizes they claim to be. The following table shows describes
the intended and true image sizes. Extra pixels manifest as green lines at the
bottom of images.
- 4k:     (3840, 2160) --> (3840, 2176)
- 1080p:  (1920, 1080) --> (1920, 1088) 
- 720p:   (1280, 720)  --> (1280, 736) 
- VGA:    (640,  480)  --> (640, 480)

## Raw image data size
- A 4k raw image is ~12 MB.

## Disk throughput
The measured throughput performance of the disk is between 15-40MB/second,
depending on CPU usage. Given a raw image size of ~12 MB, only ~1-3 raw images
per second can be saved to disk. 

Measure throughput of ~12MB image with the following script:
```bash
dd if=/dev/zero of=/tmp/test1.img bs=12M count=100 oflag=dsync
```

## Snapdragon Camera Linux Interface Specification
http://support.intrinsyc.com/documents/143
