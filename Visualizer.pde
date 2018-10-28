import processing.serial.*;
Serial myPort;
import processing.serial.*;
import processing.opengl.*;
import toxi.geom.*;
import toxi.processing.*;
ToxiclibsSupport gfx;

float yaw = 0.0;
float pitch = 0.0;
float roll = 0.0;

void setup()
{
  size(600, 500, P3D);

  // if you have only ONE serial port active
  myPort = new Serial(this, "COM7", 115200); // if you have only ONE serial port active

  // if you know the serial port name
  //myPort = new Serial(this, "COM6:", 9600);                    // Windows
  //myPort = new Serial(this, "/dev/ttyACM0", 9600);             // Linux
  //myPort = new Serial(this, "/dev/cu.usbmodem1217321", 9600);  // Mac

  textSize(16); // set text size
  textMode(SHAPE); // set text mode to shape
}

void draw()
{
  serialEvent();  // read and parse incoming serial message
  background(0); // set background to white
  lights();

  translate(width/2, height/2); // set position to centre

  pushMatrix(); // begin object

  float c1 = cos(radians(roll));
  float s1 = sin(radians(roll));
  float c2 = cos(radians(pitch));
  float s2 = sin(radians(pitch));
  float c3 = cos(radians(yaw));
  float s3 = sin(radians(yaw));
  applyMatrix( c2*c3, s1*s3+c1*c3*s2, c3*s1*s2-c1*s3, 0,
               -s2, c1*c2, c2*s1, 0,
               c2*s3, c1*s2*s3-c3*s1, c1*c3+s1*s2*s3, 0,
               0, 0, 0, 1);

  drawing();

  popMatrix(); // end of object

  // Print values to console
  print(roll);
  print("\t");
  print(pitch);
  print("\t");
  print(yaw);
  println();
}

void serialEvent()
{
  int newLine = 13; // new line character in ASCII
  String message;
  do {
    message = myPort.readStringUntil(newLine); // read from port until new line
    if (message != null) {
      String[] list = split(trim(message), " ");
      if (list.length >= 4 && list[0].equals("Orientation:")) {
        yaw = float(list[1])/10; // convert to float yaw
        pitch = float(list[3])/10; // convert to float pitch
        roll = -1*float(list[2])/10; // convert to float roll
      }
    }
  } while (message != null);
}

void drawing()
{
  /* function contains shape(s) that are rotated with the IMU 
  stroke(10, 10, 10); // set outline colour to darker teal
  fill(0, 50, 150); // set fill colour to lighter teal
  box(280, 10, 280); // draw Arduino board base shape


  fill(150, 0, 50); // set fill colour to lighter teal
  box(15, 300, 15); // draw Arduino board base shape


  fill(0, 150, 50); // set fill colour to lighter teal
  box(15, 15, 300); // draw Arduino board base shape*/
  
    // draw main body in red
    fill(255, 0, 0, 200);
    box(20, 20, 215);
    fill(0, 0, 255, 200);
    pushMatrix();
    translate(0, 0, -120);
    rotateX(PI/2);
    drawCylinder(0, 30, 30, 30);
    popMatrix();
    
    // draw wings and tail fin in green
    fill(0, 255, 0, 200);
    beginShape(TRIANGLES);
    vertex(-250,  2, 30); vertex(0,  2, -80); vertex(250,  2, 30);  // wing top layer
    vertex(-250, -2, 30); vertex(0, -2, -80); vertex(250, -2, 30);  // wing bottom layer
    vertex(-2, 0, 98); vertex(-2, -30, 98); vertex(-2, 0, 70);  // tail left layer
    vertex( 2, 0, 98); vertex( 2, -30, 98); vertex( 2, 0, 70);  // tail right layer
    endShape();
    beginShape(QUADS);
    vertex(-200, 2, 30); vertex(-200, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
    vertex( 200, 2, 30); vertex( 200, -2, 30); vertex(  0, -2, -80); vertex(  0, 2, -80);
    vertex(-200, 2, 30); vertex(-200, -2, 30); vertex(200, -2,  30); vertex(200, 2,  30);
   
    //vertex(-2,  10, 140); vertex(2,   0, 140); vertex(2, -30, 140); vertex(-2, -30, 140);
    vertex(-80,   0, 98); vertex(80,   0, 98); vertex(80,   0, 133); vertex(-80,   0, 133);
  
    
    vertex(-2, -30, 100); vertex(2, -30, 100); vertex(2,   0, 70); vertex(-2,   0, 70);
    endShape();
}


void drawCylinder(float topRadius, float bottomRadius, float tall, int sides) {
    float angle = 0;
    float angleIncrement = TWO_PI / sides;
    beginShape(QUAD_STRIP);
    for (int i = 0; i < sides + 1; ++i) {
        vertex(topRadius*cos(angle), 0, topRadius*sin(angle));
        vertex(bottomRadius*cos(angle), tall, bottomRadius*sin(angle));
        angle += angleIncrement;
    }
    endShape();
    
    // If it is not a cone, draw the circular top cap
    if (topRadius != 0) {
        angle = 0;
        beginShape(TRIANGLE_FAN);
        
        // Center point
        vertex(0, 0, 0);
        for (int i = 0; i < sides + 1; i++) {
            vertex(topRadius * cos(angle), 0, topRadius * sin(angle));
            angle += angleIncrement;
        }
        endShape();
    }
  
    // If it is not a cone, draw the circular bottom cap
    if (bottomRadius != 0) {
        angle = 0;
        beginShape(TRIANGLE_FAN);
    
        // Center point
        vertex(0, tall, 0);
        for (int i = 0; i < sides + 1; i++) {
            vertex(bottomRadius * cos(angle), tall, bottomRadius * sin(angle));
            angle += angleIncrement;
        }
        endShape();
    }
}