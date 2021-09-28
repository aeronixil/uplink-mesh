
#include "painlessMesh.h"

#define   MESH_PREFIX     "uplinkautomesh"
#define   MESH_PASSWORD   "alphabravocharliedelta"
#define   MESH_PORT       5555
#define   task_width 3000 // how many times a task occurs given in milli seconds
int counter = 0;

Scheduler uplink_schedule; // to control your personal task
painlessMesh  mesh;

// User stub
void sendMessage() ; // Prototype so PlatformIO doesn't complain
void uplink_execute();

Task uplink_task_default(task_width, TASK_FOREVER, &uplink_execute );

void uplink_execute()
{
  //write code to execute or business code or user code

  String str = String(counter);
  uplink_send(str);
  counter = counter+1;
  
}

// use the uplink_send command to send messages to the mesh
void uplink_send(String uplstr)
{
  uplstr += mesh.getNodeId();
  mesh.sendBroadcast(uplstr);
  
}

void sendMessage() {
  String msg = "Hi from node1";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg ) {
  Serial.println(msg);
  //Serial.printf("startHere: Received from %u msg=%s\n", from, msg.c_str());
}

void newConnectionCallback(uint32_t nodeId) {
    Serial.printf("--> startHere: New Connection, nodeId = %u\n", nodeId);
}

void changedConnectionCallback() {
  Serial.printf("Changed connections\n");
}

void nodeTimeAdjustedCallback(int32_t offset) {
    Serial.printf("Adjusted time %u. Offset = %d\n", mesh.getNodeTime(),offset);
}

void setup() {
  Serial.begin(115200);

//mesh.setDebugMsgTypes( ERROR | MESH_STATUS | CONNECTION | SYNC | COMMUNICATION | GENERAL | MSG_TYPES | REMOTE ); // all types on
  mesh.setDebugMsgTypes( ERROR | STARTUP );  // set before init() so that you can see startup messages

  mesh.init( MESH_PREFIX, MESH_PASSWORD, &uplink_schedule, MESH_PORT );
  mesh.onReceive(&receivedCallback);
  mesh.onNewConnection(&newConnectionCallback);
  mesh.onChangedConnections(&changedConnectionCallback);
  mesh.onNodeTimeAdjusted(&nodeTimeAdjustedCallback);

  uplink_schedule.addTask( uplink_task_default );
  uplink_task_default.enable();
}

void loop() {
  // it will run the user scheduler as well
  mesh.update();
} 
