# GemStylus

![Image of Machine](https://github.com/sobodash/gemstylus/blob/master/machine.jpg?raw=true)

This is a simple Arduino project using a 7-segment LED backpack and MPU6050 sensor
to add digital angle reading to any faceting machine stylus.

Required components include:

  * Faceting stylus (duh)
  * Adafruit 7-segment LED and backpack
  * Adafruit MPU6050 accelerometer and gyro
  * Lots of high temperature hot glue

AliExpress vendors sell faceting arms that take 6mm brass dops and cost as little as
$50. With this, you can bring many of the bells and whistles of a $5,000 UltraTec V5
to any ancient faceting machine, or even one of those cheap Chinese machines that
cost $300.


## Before You Begin

I strongly recommend soldering all connections. In an earlier version of the project
I attempted to have a removable connector on the MPU6050. This is a terrible idea because
the stylus is always in motion and the connector is a weak point. During serious work, it
would wobble and break communications between the Arduino and the sensor. This froze the
sensor and required me to reboot and recalibrate.

Additionally, because of the length of the run between the sensor and the board and
proximity to spinning motors, a shielded cable is ideal. In my setup, I use a MIDI cable
cut in half and run to a DIN-5 connector on the project box. The connector is then
soldered directly to the Arduino. MIDI cables are designed for long runs and have just
enough wires to support the sensor.


## Installing the Sensor

After you solder your wire onto the MPU6050, you should cover any exposed wire and the
connection points in hot glue.

This is important. The stylus will constantly be moving up and down and side to side and
putting pressure on these wires. Even if you have the best soldering skills in the
world, the wires will eventually strain and snap off above the solder points. A casing
of hot glue will distribute the force while locking all the wires in place.

When the glue is cooled, clean the back of your sensor and the mount point of the stylus
with rubbing alcohol.

This program is designed for the MPU6050 to be mounted with the back side flush against
the side of the stylus nearest to the user. It should be mounted flush with the edge of
the stylus and as flat as possible.

To ensure a good bond, I let the stylus and sensor sit on a heater before applying
the glue. If you are a lapidarist, your dop wax pot works fine for this.


## Calibrating

In faceting, we don't care about having the sensor level with the earth: we need
it level with the spinning lap.

I am going to assume your faceting post is unbent and at a right angle to the lap.
If it is not, then you have a serious problem no Arduino is going to be able to solve.

To calibrate, compile and upload this sketch with `float offset = 0;`

Next, use a square dowel or other flat device alongside your stylus and move it all
the way down until it is square with the post.

Check what is displayed on the 7-segment display: this is your offset. Change the
value of offet to this number and upload the sketch again. Your display should now
show a number between 0.1 and -0.1.

To confirm calibration, rotate the stylus up to 90 degrees. Again, use your flat tool to
square it against post. You should see a reading between 89.8 and 90.1.

Do not be concerned if the digital 90 does not match the protractor's 90. A lot of
cheap faceting machines come with protractors that are not properly zeroed, and they
drift considerably. Even a $6 MPU6050 board is probably more accurate.

If you want to test further, because you don't believe me, grab anything soft like a
chunk of malachite or fluorite and quickly cut a 96-24-48-72 square girdle, then pick
any side and cut a 45 degree angle. Remove it from the dop and verify the results with
a protractor. You should realistically be able to achieve within 0.4 degrees of precision
if you are using a true lap.


