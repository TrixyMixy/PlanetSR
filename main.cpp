#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <chrono>
#include <vector>
#include <cmath>
#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdlib.h>
#include <stdio.h>

using namespace std;

// Measurements are in m, kg, atm, etc.
const double gconst=0.00000000006674;

double elT;
double eT=chrono::duration<double>(chrono::high_resolution_clock::now().time_since_epoch()).count(), sT;
double tstep;
double dt=1/120.0;

class vect {
  public:
    double x, y, z;
    vect(){
      x=0;
      y=0;
      z=0;
    };
    vect(double ix, double iy, double iz){
      x=ix;
      y=iy;
      z=iz;
    };
    vect add(vect a){
      return vect(x+a.x,y+a.y,z+a.z);
    }
    vect sub(vect a){
      return vect(x-a.x,y-a.y,z-a.z);
    }
    vect mult(double a){
      return vect(x*a,y*a,z*a);
    }
    vect div(double a){
      return vect(x/a,y/a,z/a);
    }
    double getDist(vect a){
      return sqrt(abs(pow(x-a.x,2)+pow(y-a.y,2)+pow(z-a.z,2)));
    }
    string toString(){
      return to_string(x)+" "+to_string(y)+" "+to_string(z);
    }
};

class entity {
  vect pos;
  vect vel;
  double radius, mass;
  public:
    entity(vect ipos,double ir,double im){
      pos=ipos;
      radius=ir;
      mass=im;
    };
    vect addV(vect a){
      vel=vel.add(a);
      return vel;
    }
    vect setV(vect a){
      vel=a;
      return vel;
    }
    vect getV(){
      return vel;
    }
    vect getPos(){
      return pos;
    }
    vect addPos(vect a){
      pos=pos.add(a);
      return pos;
    }
    double calcForce(entity a){
      return (gconst*mass*a.mass)/pow(pos.getDist(a.pos),2);
    }
    double getMass(){
      return mass;
    }
    vect step(double t){
      return vect();
    } 
};

vector<entity> entities;

void universe(double t){
  for(int i=0;i<entities.size();++i){ // Get Force
    vect tVec=entities[i].getV();
    for(int j=0;j<entities.size();++j){
      if(&entities[j]==&entities[i])continue;
      tVec=tVec.add(entities[i].getPos().sub(entities[j].getPos()).div(entities[i].getPos().getDist(entities[j].getPos())).mult(entities[i].calcForce(entities[j])).div(entities[i].getMass()));
    } 
    entities[i].setV(tVec);
    entities[i].addPos(entities[i].getV().mult(-1*dt)); // May move to somewhere else
    //cout<<i<<": "<<entities[i].getPos().toString()<<"\n";
    //cout<<tVec.div(entities[i].getMass()).toString()<<"\n\n"; // Newtons
  }
  //cout<<"\n";
}

void simulate(){
  sT=chrono::duration<double>(chrono::high_resolution_clock::now().time_since_epoch()).count();
  elT=sT-eT;
  while(tstep>=dt){
    //cout<<tstep<<"\n";
    universe(elT);
    tstep-=dt;
  }
  if(tstep<dt)tstep+=elT;
  eT=sT;
}
int main() {
  if (!glfwInit()){
    throw "ERROR: GLFW Failed.";
  } else {
    GLFWwindow* simview = glfwCreateWindow(640, 480, "PlanetSR PRE-ALPHA", NULL, NULL);
    if (!simview){
      throw "ERROR: Window failed to load."; // Window or OpenGL context creation failed
    }
    glfwMakeContextCurrent(simview);
    entities.push_back(entity(vect(3,0,0),10,1e10));
    entities.push_back(entity(vect(0,0,0),10,1e8));
    entities[1].addV(vect(0,0,-3));  
    while (!glfwWindowShouldClose(simview)){
      int width, height;
      float ratio = width / (float) height;
      glfwGetFramebufferSize(simview, &width, &height);
      glViewport(0, 0, width, height);
      glMatrixMode(GL_PROJECTION);
      glLoadIdentity();
      glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
      glMatrixMode(GL_MODELVIEW);
      glLoadIdentity();
      glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
      glBegin(GL_TRIANGLES);
      glColor3f(1.f, 0.f, 0.f);
      glVertex3f(-0.6f, -0.4f, 0.f);
      glColor3f(0.f, 1.f, 0.f);
      glVertex3f(0.6f, -0.4f, 0.f);
      glColor3f(0.f, 0.f, 1.f);
      glVertex3f(0.f, 0.6f, 0.f);
      glEnd();
      glfwSwapBuffers(simview);
      simulate();
      glfwPollEvents();
    }
  }
}