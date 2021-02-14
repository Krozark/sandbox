# Buildozer install
 
git clone https://github.com/kivy/buildozer.git
cd buildozer
sudo python setup.py install

#  Create buildozer project file
buildozer init


# Build APK
buildozer android debug deploy run