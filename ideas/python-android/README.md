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
pip install git+https://github.com/kivy/python-for-android.git
```

## gradle

 
ouvrir buildozer/android/platform/build-armeabi-v7a/dists/KvTest__armeabi-v7a/gradle/wrapper

chamge la ligne en distributionUrl=https\://services.gradle.org/distributions/gradle-6.6.1-all.zip
need gradle 6.6.1 ?
 

## dans fichier gradle (.buildozer/android/platform/build-armeabi-v7a/dists/KvTest__armeabi-v7a/templates/build.tmpl.gradle)

```
repositories {
    mavenCentral()
    maven {
        url 'https://alphacephei.com/maven/'
    }
}

compileOptions {
    sourceCompatibility JavaVersion.VERSION_1_8
    targetCompatibility JavaVersion.VERSION_1_8
}

dependencies {
    implementation group: 'net.java.dev.jna', name: 'jna', version: '5.7.0'
    implementation group: 'com.alphacephei', name: 'vosk', version: '0.3.30'
}
```

```
TODO make a python4android repo clone
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