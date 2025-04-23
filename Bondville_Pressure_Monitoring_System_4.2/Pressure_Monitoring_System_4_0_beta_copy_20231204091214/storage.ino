
#include "nvs_flash.h"
#include "nvs.h"
char* result;

#include "StringSplitter.h"
String BPMSID;
String area;
int uploadDelay;
int emailDelay;
float cutoff;
String systemStatus;
String saveParam;
String adminMail;
String indicator1;
String indicator2;

String newBPMSID;
String newArea;
String newUploadDelay;
String newEmailDelay;
String newCutoff;
String newSystemStatus;
String newSaveParam;
String newAdminMail;
String restartRecord;
String newRecord;

float pressureA;
float pressureB;
int sensorA;
int sensorB;

int BPMSIDIndex=0;
int areaIndex=0;
int uploadDelayIndex=0;
int emailDelayIndex=0;
float cutoffIndex=0;
int systemStatusIndex=0;
int saveParamIndex=0;
int adminMailIndex=0;
int indicator1Index=0;
int indicator2Index=0;

char read_ssid[32];
size_t length = sizeof(read_ssid);


void setupNVS() {
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
  loading=loading+10;
  loadView();
}

String saveStringToFlash(String getkey,String getvalue){
    const char* key = getkey.c_str();
    const char* value = getvalue.c_str();

    saveToFlash(key, value);
    Serial.printf("Saving ");
    Serial.printf(key);
    Serial.printf(" to NVS:\n");
    return("stored successfully");
}



void saveToFlash(const char* key, const char* value) {
      nvs_handle_t nvs_handle;
      esp_err_t err;

      // Open the NVS namespace "storage" with read-write permissions
      err = nvs_open("storage", NVS_READWRITE, &nvs_handle);
      if (err != ESP_OK) {
          Serial.printf("Error (%s) opening NVS handle for saving!\n", esp_err_to_name(err));
          return;
      }

      // Write the string value to the specified key
      err = nvs_set_str(nvs_handle, key, value);
      if (err != ESP_OK) {
          Serial.printf("Error (%s) saving value to NVS!\n", esp_err_to_name(err));
          return;
      }

      // Commit and close the NVS handle
      nvs_commit(nvs_handle);
      nvs_close(nvs_handle);
}


String readFlash(const char* key) {
    // Read the string back from flash (optional, just for verification)
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err == ESP_OK) {
        char read_value[100]; // Assuming your string is less than 32 characters
        size_t length = sizeof(read_value);
        
        err = nvs_get_str(nvs_handle, key, read_value, &length);
        if (err == ESP_OK) {
            Serial.printf(key);
            Serial.printf(" is: %s\n", read_value);

            // Convert read_value to a char*
            result = new char[length];
            strcpy(result, read_value);

            // Print and verify the updated SSID
            //Serial.printf("Updated SSID is: %s\n", ssid);
            return(result);
        } else {
            Serial.printf("Error reading from NVS (%s)\n", esp_err_to_name(err));
            saveStringToFlash(key,""); //reset key
            return("Error");
        }

        nvs_close(nvs_handle);
    } else {
        Serial.printf("Error opening NVS handle for reading(%s)\n", esp_err_to_name(err));
        return("Error");
    }
}


String readRestartRecord(const char* key) {
    // Read the string back from flash (optional, just for verification)
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err == ESP_OK) {
        char read_value[2048]; // Assuming your string is less than 32 characters
        size_t length = sizeof(read_value);
        
        err = nvs_get_str(nvs_handle, key, read_value, &length);
        if (err == ESP_OK) {
            Serial.printf(key);
            Serial.printf(" is: %s\n", read_value);

            // Convert read_value to a char*
            result = new char[length];
            strcpy(result, read_value);

            // Print and verify the updated SSID
            //Serial.printf("Updated SSID is: %s\n", ssid);
            return(result);
        } else {
            Serial.printf("Error reading from NVS (%s)\n", esp_err_to_name(err));
            saveStringToFlash(key,""); //reset key
            return("Error");
        }

        nvs_close(nvs_handle);
    } else {
        Serial.printf("Error opening NVS handle for reading(%s)\n", esp_err_to_name(err));
        return("Error");
    }
}



String readSSID() {
    // Read the string back from flash (optional, just for verification)
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err == ESP_OK) {
        char read_value[100]; // Assuming your string is less than 32 characters
        size_t length = sizeof(read_value);
        
        err = nvs_get_str(nvs_handle, "ssid", read_value, &length);
        if (err == ESP_OK) {
            Serial.printf("ssid");
            Serial.printf(" is: %s\n", read_value);

            // Convert read_value to a char*
            ssid = new char[length];
            strcpy(ssid, read_value);

            // Print and verify the updated SSID
            //Serial.printf("Updated SSID is: %s\n", ssid);
            return("SSID successfully synced");
        } else {
            Serial.printf("Error reading from NVS (%s)\n", esp_err_to_name(err));
            saveStringToFlash("ssid",""); //reset key
            return("Error");
        }

        nvs_close(nvs_handle);
    } else {
        Serial.printf("Error opening NVS handle for reading(%s)\n", esp_err_to_name(err));
        return("Error");
    }
}

String readPassword() {
    // Read the string back from flash (optional, just for verification)
    nvs_handle_t nvs_handle;
    esp_err_t err;

    err = nvs_open("storage", NVS_READONLY, &nvs_handle);
    if (err == ESP_OK) {
        char read_value[100]; // Assuming your string is less than 32 characters
        size_t length = sizeof(read_value);
        
        err = nvs_get_str(nvs_handle, "password", read_value, &length);
        if (err == ESP_OK) {
            Serial.printf("password");
            Serial.printf(" is: %s\n", read_value);

            // Convert read_value to a char*
            password = new char[length];
            strcpy(password, read_value);

            // Print and verify the updated SSID
            //Serial.printf("Updated password is: %s\n", password);
            return("password successfully synced");
        } else {
            Serial.printf("Error reading from NVS (%s)\n", esp_err_to_name(err));
            saveStringToFlash("password",""); //reset key
            return("Error");
        }

        nvs_close(nvs_handle);
    } else {
        Serial.printf("Error opening NVS handle for reading(%s)\n", esp_err_to_name(err));
        return("Error");
    }
}

String split(String strPayload){
  Serial.println("Payload Received");
  //Serial.println("Test String: " + strPayload);
  StringSplitter *splitter = new StringSplitter(strPayload, '&', 25);  // new StringSplitter(string_to_split, delimiter, limit)
  int itemCount = splitter->getItemCount();
  //Serial.println("Item count: " + String(itemCount));

  for(int i = 0; i < itemCount; i++){
    String item = splitter->getItemAtIndex(i);
    //Serial.println("Item @ index " + String(i) + ": " + String(item));

    //Serial.println(item);
    String strItem=String(item);
    if(strItem.equals("BPMS ID")){
      BPMSIDIndex=i+1;
    }else if(strItem.equals("Deployed Area")){
      areaIndex=i+1;
    }else if(strItem.equals("Upload Delay")){
      uploadDelayIndex=i+1;
    }else if(strItem.equals("Email Delay")){
      emailDelayIndex=i+1;
    }else if(strItem.equals("Cutoff Value")){
      cutoffIndex=i+1;
    }else if(strItem.equals("Admin E-mail")){
      adminMailIndex=i+1;
    }else if(strItem.equals("System Status")){
      systemStatusIndex=i+1;
    }else if(strItem.equals("Save Parameters")){
      saveParamIndex=i+1;
    }else if(strItem.equals("indicator1")){
      indicator1Index=i+1;
    }else if(strItem.equals("indicator2")){
      indicator2Index=i+1;
    }

    if(i==BPMSIDIndex){
      newBPMSID=item;
    }else if(i==areaIndex){
      newArea=item;
    }else if(i==uploadDelayIndex){
      newUploadDelay=item;
    }else if(i==emailDelayIndex){
      newEmailDelay=item;
    }else if(i==cutoffIndex){
      newCutoff=item;
    }else if(i==adminMailIndex){
      newAdminMail=item;
    }else if(i==systemStatusIndex){
      newSystemStatus=item;
    }else if(i==saveParamIndex){
      newSaveParam=item;
    }else if(i==indicator1Index){
      indicator1=item;
    }else if(i==indicator2Index){
      indicator2=item;
    }
  }
  Serial.println("Payload Splitted");
  return(newBPMSID+"\n"+newArea+"\n"+newUploadDelay+"\n"+newEmailDelay+"\n"+newCutoff+"\n"+newAdminMail+"\n"+newSystemStatus+"\n"+newSaveParam+"\n"+indicator1+"\n"+indicator2);
}



String SaveParamToNVS(){
  if(newSaveParam=="Update Parameters"){
    BPMSID=readFlash("BPMSID");
    area=readFlash("area");
    uploadDelay=readFlash("uploadDelay").toInt();
    emailDelay=readFlash("emailDelay").toInt();
    cutoff=readFlash("cutoff").toFloat();
    recipient0=readFlash("recipient0");
    recipient1=readFlash("recipient1");
    recipient2=readFlash("recipient2");
    recipient3=readFlash("recipient3");
    recipient4=readFlash("recipient4");
    systemStatus=readFlash("systemStatus");
    saveParam=readFlash("saveParam");
    Serial.println("Parameters updating");
    Serial.println("Old Values: "+BPMSID+" "+area+" "+uploadDelay+" "+emailDelay+" "+cutoff+
    " "+recipient0+","+recipient1+","+recipient2+","+recipient3+","+recipient4+","+systemStatus+" "+saveParam+" "+indicator1+" "+indicator2);
    //Serial.println("Old Setup Values: "+pressureA+" "+pressureB+" "+sensorA+" "+sensorB);
    Serial.println(emailSplitter(newAdminMail));
  
    saveStringToFlash("BPMSID",newBPMSID);
    saveStringToFlash("area",newArea);
    saveStringToFlash("uploadDelay",newUploadDelay);
    saveStringToFlash("emailDelay",newEmailDelay);
    saveStringToFlash("cutoff",newCutoff);
    saveStringToFlash("recipient0",recipient0);
    saveStringToFlash("recipient1",recipient1);
    saveStringToFlash("recipient2",recipient2);
    saveStringToFlash("recipient3",recipient3);
    saveStringToFlash("recipient4",recipient4);
    saveStringToFlash("systemStatus",newSystemStatus);
    // saveStringToFlash("Indicator1",newIndicator1);
    // saveStringToFlash("Indicator2",newIndicator2);
    newSaveParam="Parameters saved";
    Serial.println("Parameters saved to NVS");
    loadParameters();
    return("Parameters saved to NVS");
  }else{
    return("Permission not granted for saving parameters to NVS");
  }
}

void loadParameters(){
    BPMSID=readFlash("BPMSID");
    area=readFlash("area");
    uploadDelay=readFlash("uploadDelay").toInt();
    emailDelay=readFlash("emailDelay").toInt();
    cutoff=readFlash("cutoff").toFloat();
    recipient0=readFlash("recipient0");
    recipient1=readFlash("recipient1");
    recipient2=readFlash("recipient2");
    recipient3=readFlash("recipient3");
    recipient4=readFlash("recipient4");
    systemStatus=readFlash("systemStatus");
    saveParam=readFlash("saveParam");
    readSSID();
    readPassword();
    pressureA=readFlash("pressureA").toFloat();
    pressureB=readFlash("pressureB").toFloat();
    sensorA=readFlash("sensorA").toInt();
    sensorB=readFlash("sensorB").toInt();
    Serial.println(autoMode());
    Serial.println("New Values: "+BPMSID+" "+area+" "+uploadDelay+" "+emailDelay+" "+cutoff+" "+recipient0+","+recipient1+","+recipient2+","+recipient3+","+recipient4+","+systemStatus+" "+saveParam+" "+indicator1+" "+indicator2);
    Serial.println("Parameters updated");
    loading=loading+10;
    loadView();
}


void saveCredentials(){
  Serial.println("Old credentials");
  readSSID();
  readPassword();
  Serial.print("Save new SSID : ");
  Serial.println(newSSID);
  Serial.print("Save new Password : ");
  Serial.println(newPassword);

  Serial.println("Credentials updating"); 
  saveStringToFlash("ssid",newSSID);
  saveStringToFlash("password",newPassword);
  readSSID();
  readPassword();
  Serial.println("Credentials updated");   

  Serial.println("New Setup data : ");
  Serial.print("ssid : ");
  Serial.println(ssid);
  Serial.print("password : ");
  Serial.println(password);
}

void saveCalibration(){
  Serial.println("Old calibration");
  pressureA=readFlash("pressureA").toFloat();
  pressureB=readFlash("pressureB").toFloat();
  sensorA=readFlash("sensorA").toInt();
  sensorB=readFlash("sensorB").toInt();

  Serial.println("pressureA : ");
  Serial.println(pressureA);
  Serial.println("pressureB : ");
  Serial.println(pressureB);
  Serial.println("sensorA : ");
  Serial.println(sensorA);
  Serial.println("sensorB : ");
  Serial.println(sensorB);

  saveStringToFlash("pressureA",newPressureA);
  saveStringToFlash("pressureB",newPressureB);
  saveStringToFlash("sensorA",newSensorA);
  saveStringToFlash("sensorB",newSensorB);
  Serial.println("Calibrated");

  Serial.println("New calibration");
  pressureA=readFlash("pressureA").toFloat();
  pressureB=readFlash("pressureB").toFloat();
  sensorA=readFlash("sensorA").toInt();
  sensorB=readFlash("sensorB").toInt();

}

void loadCredentials(){
  readSSID();
  readPassword();
}

void restarted(){
  newRecord=getTime();
  Serial.println("restart Record : ");
  Serial.print("Time:");
  Serial.println(newRecord);
  restartRecord=readRestartRecord("restarted");
  restartRecord=restartRecord+"\n"+newRecord;
  saveStringToFlash("restarted",restartRecord);
  Serial.println(restartRecord);
}

void restarting(){
  newRecord="Restarting due to lack of Internet";
  Serial.print("restart Record : ");
  Serial.println(newRecord);
  restartRecord=readRestartRecord("restarted");
  restartRecord=restartRecord+"\n"+newRecord;
  saveStringToFlash("restarted",restartRecord);
  Serial.println(restartRecord);
}
void restartingReason(String reason){
  newRecord="Restarting due to "+reason;
  Serial.print("restart Record : ");
  Serial.println(newRecord);
  restartRecord=readRestartRecord("restarted");
  restartRecord=restartRecord+"\n"+newRecord;
  saveStringToFlash("restarted",restartRecord);
  Serial.println(restartRecord);
}

String getTime(){
  static bool flag = false;
  struct tm timeinfoGet;
  if (!getLocalTime(&timeinfoGet)) {
    Serial.println("Failed to obtain time");
    return("Failed to obtain time");
  }
  char timeStringBuff1[50]; //50 chars should be enough
  strftime(timeStringBuff1, sizeof(timeStringBuff1), "%H:%M %B %d %Y", &timeinfoGet);
  String newTime(timeStringBuff1);
  return(newTime);
}
