# udpfwd
UDP port forwarder

Usage:
```
python3 udpfwd.py -S 127.0.0.1:4000 -D 127.0.0.1:3000 -D 127.0.0.1:3001
```
![image](https://github.com/defencore/udpfwd/assets/56395503/ec07dfc3-2d2d-4221-8163-6f22f00c4b8a)
Testing:
```
ffmpeg -stream_loop -1 -re -i video.mp4  -c:v copy -c:a copy -c:d copy -f mpegts udp://127.0.0.1:4000
```
```
ffplay -y 400 -fflags nobuffer -flags low_delay -probesize 20000 -analyzeduration 1 -strict experimental -framedrop udp://127.0.0.1:3000
```
```
ffplay -y 400 -fflags nobuffer -flags low_delay -probesize 20000 -analyzeduration 1 -strict experimental -framedrop udp://127.0.0.1:3001
```
