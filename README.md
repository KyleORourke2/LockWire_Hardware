LockWire Hardware
Written by Kyle O'Rourke, 1/19/2019
CruzHacks UCSC 2019

LockWire
Inspiration
Have you ever had your bike stolen? Ask anyone on campus and they are sure to have a story about a stolen bike. Bike theft is the 2nd most common crime in Santa Cruz. A four-year college student has a 53% chance of having their bike taken. It's a problem.

Having your bike stolen can be devastating to a college student who may already be struggling financially.

What it does
LockWire is a smart IOT device that alerts you when your bike is being stolen. From an Android app, you can track the location of your bike in real time.

How we built it
LockWire is a series of connections between three different systems: a Snapdragon embedded computer (on the bike), a Google Cloud server, and a front-end Android application.

Using a Snapdragon 410c paired with an Arduino Uno, we send GPS, accelerometer, and gyroscope data to our Google server. On the server, a Node.js program processes the data and places it in a Firebase database. This database is then read by the end user Android app. The app uses the fetched data to then interact with the Google Maps API.

Challenges we ran into
The most difficult parts of this project lied in the connections between the various systems. On the hardware side, we ran into challenges interfacing two embedded systems, Snapdragon and Arduino, with each other. Also, the sheer number of different APIs, programming languages, processor architectures, and even operating systems created problems that none of us have encountered before. Firebase, in particular, gave us countless headaches not solvable even with the copious amounts of caffeine available.

Accomplishments we're proud of
We are very proud to have successfully juggled this set of complicated and diverse tools to create a working system.

What we learned
A lot. We learned how to move data from physical real-time hardware to the internet, and then back to someone's phone. Specifically, we have never worked with a snapdragon board, routing through a Google cloud VM, and interacting with a real-time database.

What's next for LockWire
Scaling the system to allow for multiple "LockWire" devices on the network. Cellular-based instead of WiFi. Automatic "unlock" based on phone location data. iPhone support. Physical alarm systems. A beefier notification system. Network security.
