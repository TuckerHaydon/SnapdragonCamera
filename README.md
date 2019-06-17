## 

## Snapdragon Camera Linux Interface Specification
http://support.intrinsyc.com/documents/143

## Notes
- Each 4K image is ~12 MB raw. The measured throughput performance of the disk
  is ~20MB/second. This means that no matter how fast the camera can retrieve
  images, the program can really only write ~1.5 raw 4K images/second to disk

## Evaluate disk throughput
```bash
dd if=/dev/zero of=/tmp/test1.img bs=1G count=1 oflag=dsync
```
