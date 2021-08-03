# setup env
## java 8
```
apt install openjdk-8-jdk
sudo update-alternatives --config java
```
## Buildozer install
 
 ```
git clone https://github.com/kivy/buildozer.git
cd buildozer
sudo python setup.py install
```

# Build
## Build APK

```
buildozer -v android debug deploy run logcat
```


# from 0
#  Create buildozer project file
```
buildozer init
```