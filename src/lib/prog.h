#include "srcv/ArduinoJson.h"

bool sent(String topic, String me)
{
  char tops[40] = "";
  topic.toCharArray(tops, topic.length() + 1);
  delay(10);
  char sen[1000] = "";
  me.toCharArray(sen, me.length() + 1);
  
  client.publish(tops, sen);
}
bool onoff(int dataof, int idnut)
{
  
  bool stt = false;
  for (int i = 0; i < valdata.stbconec; i++)
  {
    if (valdata.ID[i].toInt() == idnut)
    {

      stt = deriver(dataof, i, idnut);
      String data;
      for (int j = 0; j < valdata.socaidat; j++)
      {
        data = valdata.setingdataanhxa[i][j];
        if (data.length() == 0)
          break;
        stt = anhxa(data, dataof);
      }
       if(valdata.sqlid[i]==true &&valdata.datasidsent[i]!= dataof|| valdata.Notif[i]==true&&valdata.datasidsent[i]!= dataof)
            {
               sent((variablemqtt.pussql+String(idnut)),String(dataof));
               valdata.datasidsent[i]= dataof;
            }
    }
  }
 
  return stt;
}
String senstr()
{
  String str = "[";
  for (int i = 0; i < valdata.stbconec; i++)
  {
    if (i > 0)
      str += ',';
    str += "{\"type\":" + String(valdata.TYPE[i]) + ",\"id\":" + valdata.ID[i] + ",\"mac\":\"12345\",\"data\":" + valdata.datastaus[i] +  "}";

  
  }
  str += "]";
   
  return str;
}

String datasen(String datain)
{

  DynamicJsonDocument doc(2000);
  DeserializationError error = deserializeJson(doc, datain);
  if (error)
  {
    return "0";
  }
  String DT = doc["action"];
  String DT2 = doc["dulieu"]["id"];

  switch (DT[0])
  {
  case 'o':
  {

    int DT3 = doc["dulieu"]["s"];
    
    onoff(DT3, DT2.toInt());
    

    break;
  }
  case 'd':
  {

    int DT3 = doc["dulieu"]["data"];
    
    onoff(DT3, DT2.toInt());
    //sent(variablemqtt.pussql+DT2,String(DT3));
    break;
  }
  case 't':
  {

    int DT3 = doc["dulieu"]["data"];
   
    onoff(DT3, DT2.toInt());
    //sent(variablemqtt.pussql+DT2,String(DT3));
    break;
  }
  case 's':
  {

    int datas = doc["dulieu"]["data"];
    int id = doc["dulieu"]["id"];

      for (int i = 0; i < valdata.stbconec; i++)
       {
         if(id  == valdata.ID[i].toInt())
         {
           valdata.datastaus[i] = datas ; 
           // String str = datasen(me);
            //sent(variablemqtt.pus1, ramsen);
         }
       }

    

    break;
  }
  }

  return senstr();
}
bool setingtimer(String data)
{

  DynamicJsonDocument doc(2000);
  DeserializationError error = deserializeJson(doc, data);
  if (error)
  {

    return false;
  }

  String DT1 = doc["id"];
  String DT2 = doc["data"];

  for (int i = 0; i < valdata.stbconec; i++)
  {
    int timerdem = 0;
    int anhxadem = 0;
    if (valdata.ID[i] == DT1)
    {

      if (DT2 == "[]")
      {

        if (valdata.TYPE[i] == 1 || valdata.TYPE[i] == 3 || valdata.TYPE[i] == 4)
        {
          for (int j = 0; j < valdata.socaidat; j++)
          {
            valdata.setingdata[i][j] = "";
            valdata.setingdataanhxa[i][j] = "";
          }
          return true;
        }
        else if (valdata.TYPE[i] == 2)
        {
          for (int j = 0; j < valdata.socaidat; j++)
          {
            valdata.setingdatacontro[i][j] = "";
          }

          return true;
        }
      }
      for (int j = 0; j < valdata.socaidat; j++)
      {
        DynamicJsonDocument doc2(2000);
        DeserializationError error2 = deserializeJson(doc2, DT2);
        if (error2)
        {
          return false;
        }
        String DT6 = doc2[j]["t"];
        if (DT6 == "null")
        {      
          if (DT6[0] == 't' || DT6[0] == 'd')
          { 
            valdata.setingdata[i][ timerdem] ="";
             timerdem++;
          } 
          else if(DT6[0] == 'r')
          {
         
            valdata.setingdataanhxa[i][anhxadem] = "";
            anhxadem++;
          
          }
           else if(DT6[0] == 'c')
          {
         
            valdata.setingdatacontro[i][j] = "";    
          
          }
        }
        String DT7 = doc2[j]["d"];
        if (valdata.TYPE[i] == 1 || valdata.TYPE[i] == 3 || valdata.TYPE[i] == 4)
        {

          if (DT6[0] == 't' || DT6[0] == 'd')
          {
            valdata.setingdata[i][timerdem] = DT7;
            
            timerdem++;
          }
          if (DT6[0] == 'r')
          {
            valdata.setingdataanhxa[i][anhxadem] = DT7;

            anhxadem++;
          }
        }
        else if (valdata.TYPE[i] == 2)
        {
          valdata.setingdatacontro[i][j] = DT7;
        }
      }
      return true;
    }
  }
}
int onoffstring(String datain)
{

  DynamicJsonDocument doc(2000);
  DeserializationError error = deserializeJson(doc, datain);
  if (error)
  {
    return 1;
  }
  for (int i = 0; i < valdata.socaidat; i++)
  {
    int timerdem = 0;
    int anhxadem = 0;
    String DT = doc[i];
    if (DT == "null")
      break;
    ++valdata.stbconec;
    String DT1 = doc[i]["ID"];
    int DT2 = doc[i]["Type"];
    String DT3 = doc[i]["VT"];
    String DT4 = doc[i]["Unit"];
    String DT5 = doc[i]["Setting"];

     valdata.sqlid[i] = doc[i]["SaveData"];
     valdata.Notif[i] = doc[i]["Notifications"];
    if (DT2 == 1 || DT2 == 3 || DT2 == 4)
    {

      for (int j = 0; j < valdata.socaidat; j++)
      {
        DynamicJsonDocument doc3(2000);
        DeserializationError error3 = deserializeJson(doc3, DT5);
        if (error3)
        {
        }
        String DT6 = doc3[j]["t"];
        if (DT6 == "null")
          break;
        String DT7 = doc3[j]["d"];
        if (DT6[0] == 't' || DT6[0] == 'd')
        {
          valdata.setingdata[i][timerdem] = DT7;
          timerdem++;
        }
        if (DT6[0] == 'r')
        {
          valdata.setingdataanhxa[i][anhxadem] = DT7;

          anhxadem++;
        }
      }
    }
    if (DT2 == 2)
    {

      for (int j = 0; j < valdata.socaidat; j++)
      {
        DynamicJsonDocument doc4(2000);
        DeserializationError error4 = deserializeJson(doc4, DT5);
        if (error4)
        {
        }
        String DT6 = doc4[j]["t"];
        if (DT6 == "null")
          break;
        String DT7 = doc4[j]["d"];
        valdata.setingdatacontro[i][j] = DT7;
      }
    }
    valdata.ID[i] = DT1;
    valdata.TYPE[i] = DT2;
    valdata.VT[i] = DT3;

    valdata.UNIT[i] = DT4;
    if (valdata.TYPE[i] == 2)
    {
      int DT5 = doc[i]["Timereding"];
      valdata.timereading[i] = DT5 * 1000;
    }
  }

  return 0;
}
