
#include "painlessMesh.h"
#include <Arduino_JSON.h>

int device_id = 4567;
String device_name = "sample_name";
String target_building = "sample_building";
String target_floor = "one/two/three";
int target_device = 1234;
int target_payload = 8888;
int secretscode = 123456789;
String mess;
String message_json;

int prevon = 0;
int prevon2 =0;
int temp1 = 0;
int temp2 = 0;
int temp3 = 0;
int temp4 = 0;

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
String createjson();

String createjson()
{ 
    JSONVar jsonfile;
    jsonfile["device_id"] = device_id;
  //  jsonfile["device_name"] = device_name;
  //  jsonfile["target_building"] = target_building;
  //  jsonfile["target_floor"] = target_floor;
    jsonfile["target_device"] = target_device;
    jsonfile["target_payload"] = target_payload;
    jsonfile["secretscode"] = secretscode;

    message_json = JSON.stringify(jsonfile);
    return message_json;
}

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
  //String nodename = "nodenamehere";
  //uplstr += mesh.getNodeId();
  //uplstr += alpha;
  mesh.sendBroadcast(uplstr);
  
}

void sendMessage() {
  String msg = "Hi from node1";
  msg += mesh.getNodeId();
  mesh.sendBroadcast( msg );
  
}

// Needed for painless library
void receivedCallback( uint32_t from, String &msg )
{
    Serial.println("voidcallback is called");
    JSONVar obj  = JSON.parse(msg.c_str());

    int device_id = obj["device_id"];
   // String device_name = obj["device_name"];
   // String target_building = obj["target_building"];
  //  String target_floor = obj["target_floor"];
    int target_device = obj["target_device"];  
    int target_payload = obj["target_payload"];
    int secretscode = obj["secretscode"];
    if(target_device == 4567)
    {
        if(target_payload == 1)
        {
            digitalWrite(2, HIGH);
            Serial.println(device_name + "is ON");

        }
        if(target_payload == 0)
        {
            digitalWrite(2, LOW);
            Serial.println(device_name + "is OFF");
        }
    }
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
  pinMode(2, OUTPUT);
  pinMode(32, INPUT);
  pinMode(33, INPUT);
  pinMode(25, INPUT);
  pinMode(26, INPUT);


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
  temp1 = digitalRead(26);
  if(prevon != temp1)
  {
    prevon = temp1;
    if(temp1 == 1)
    {
      target_device = 5551;
      target_payload = 1;
      mess = createjson();
      uplink_send(mess);
    }
    if(temp1 == 0)
    {
      target_device = 5551;
      target_payload = 0;
      mess = createjson();
      uplink_send(mess);
    }
  }
  temp2 = digitalRead(25);
  if(prevon2 != temp2)
  {
    prevon2 = temp2;
    if(temp2 == 1)
    {
      target_device = 5552;
      target_payload = 1;
      mess = createjson();
      uplink_send(mess);
    }
    if(temp2 == 0)
    {
      target_device = 5552;
      target_payload = 0;
      mess = createjson();
      uplink_send(mess);
    }
  }
/*  temp3 = digitalRead(33);
  if(prevon != temp3)
  {
    prevon = temp3;
    if(temp3 == 1)
    {
      target_device = 5553;
      target_payload = 1;
      mess = createjson();
      uplink_send(mess);
    }
    if(temp3 == 0)
    {
      target_device = 5553;
      target_payload = 0;
      mess = createjson();
      uplink_send(mess);
    }
  }
  temp4 = digitalRead(32);
  if(prevon != temp4)
  {
    prevon = temp4;
    if(temp4 == 1)
    {
      target_device = 5554;
      target_payload = 1;
      mess = createjson();
      uplink_send(mess);
    }
    if(temp4 == 0)
    {
      target_device = 5554;
      target_payload = 0;
      mess = createjson();
      uplink_send(mess);
    }
  }*/
} 
