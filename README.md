# Q U I N T A L

Yet another application to turn RC Vehicule into internet control , semi-autonomus and sensor filled.

## First Project : Wifi Control RC Car with little latency as possible

* Using a streaming service for now , ustreamer seem to be a good option [ustreamer](https://github.com/pikvm/ustreamer)
* Control the arduino using firmata [FirmataC](https://github.com/grandsmarquis/FirmataC/tree/master)
* websocket to control [websocketpp](https://github.com/zaphoyd/websocketpp) with boost asio
* c++ 20 or c++ 17


### First iteration , small python script, ustreamer and vanilla HTML

Finally i wanted the remote control application to work as fast as possible. I deciced to pause my
c++ compilation effort and to write a small python script.

Receive gamepad or keyboard input from the browser and control the rover using firmata.
Receive the webcam stream via ustreamer and a html video element.

Pretty easy and the result was good ! Here is an exemple of me driving it 500 km. (Via parsec on my brother computer)


### Second iteration , writting my own firmata library with asio serial port

Well it's still to be finish but the hard part for some reason was to understand asio and to make the serial port
reading work with the arduino.

Parsing the message was not to hard from the continue byte flow.

Now just need to finish the implementation of all command read and write. I only implemented 2.

After i also need to think about my abstraction with gpio from the pi to utilize them all together blindly.

### Third iteration , integrating with WebRTC , Gstreamer and OpenCV


It took me some time and a lot of help from ChatGPT but I manage to accomplish my goal.

* Reading the webcam
* Intercept raw frame with OpenCV (try to detect faces for a start)
* Stream in h264 with udpsink

After I manage to link the part with WebRTC

* Take the udpsink output and send it as a media channel in a WebRTC connection with the client app
* Using the data channel to send data both ways

Now that all of this is working in seperate exemple it's time to put all of this together with boost::asio
in my main application. And to integrate with a signaling server.



 vcpkg install protobuf
cmake .. -DCMAKE_TOOLCHAIN_FILE=/opt/vcpkg/scripts/buildsystems/vcpkg.cmake