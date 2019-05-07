//V1 ya funciona latitud y longitud pero arroja cosaas feas aun
//v2 fue muy facir arreglar esas cosas feas, solo cambie el nombre de unas variables y elimine algunos print serial
//V3 intentando hacer los mismo con GNGGA (altitud,....)

#include <SoftwareSerial.h>
SoftwareSerial gps(4, 3); //(tx,rx) (confirmado)

int updates;
int failedUpdates;
int pos;
int stringplace = 0;

String timeUp;
String nmea[15];
String labels[14]
{ "Time UTC: "   ,//0
  "Latitud: "   ,
  "N or S:"     ,
  "Longitud: "  ,
  "E or W: "      ,//4
  "Indic calidad: " ,
  "N° de satélites: "   ,
  "Horizontal dilution of position: " ,
  "Elevación: ", //8
  "9: ",
  "Geoidal separation: ",
  "11 : ",
  "12: ",
  "Checksum: " //13
}  ;

void setup() {
  Serial.begin(9600);
  gps.begin(9600);
  Serial.println("setup 1");
  delay(1000);
  Serial.println("setup 2");
}

void loop() {

  Serial.flush();
  gps.flush();

  while (gps.available() > 0) {
    gps.read();
  }

  if (gps.find("$GNGGA,")) {
    String tempMsg = gps.readStringUntil('\n');
    for (int i = 0; i < tempMsg.length(); i++) {
      if (tempMsg.substring(i, i + 1) == ",") {
        nmea[pos] = tempMsg.substring(stringplace, i);
        stringplace = i + 1;
        pos++;
      }
      if (i == tempMsg.length() - 1) {
        nmea[pos] = tempMsg.substring(stringplace, i);
      }
    }
    updates++;

    Serial.print(labels[0]); Serial.println(nmea[0]);
    Serial.print(labels[1]); Serial.print(nmea[1]); Serial.println(nmea[2]);
    Serial.print(labels[3]); Serial.print(nmea[3]); Serial.println(nmea[4]);
    Serial.print("Latitud, Longitud:  "); Serial.print(ConvertLat()); Serial.print(", "); Serial.println(ConvertLng());
    Serial.print(labels[6]); Serial.println(nmea[6]);
    Serial.print(labels[8]); Serial.print(nmea[8]); Serial.println(nmea[9]);
    Serial.println("");
  }

  else {
    failedUpdates++;
  }

  stringplace = 0;
  pos = 0;
}

//----------------------------------------------------------------funciones:--------------------------------------------------------------------------
String ConvertLat() {
  String posneg = "";
  if (nmea[2] == "S") {
    posneg = "-";
  }
  String latfirst;
  float latsecond;
  for (int i = 0; i < nmea[1].length(); i++) {
    if (nmea[1].substring(i, i + 1) == ".") {
      latfirst = nmea[1].substring(0, i - 2);
      latsecond = nmea[1].substring(i - 2).toFloat();
    }
  }
  latsecond = latsecond / 60;
  String CalcLat = "";

  char charVal[9];
  dtostrf(latsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLat += charVal[i];
  }
  latfirst += CalcLat.substring(1);
  latfirst = posneg += latfirst;
  return latfirst;
}
//--------------------------------------------------------------
String ConvertLng() {
  String posneg = "";
  if (nmea[4] == "W") {
    posneg = "-";
  }

  String lngfirst;
  float lngsecond;
  for (int i = 0; i < nmea[3].length(); i++) {
    if (nmea[3].substring(i, i + 1) == ".") {
      lngfirst = nmea[3].substring(0, i - 2);
      //Serial.println(lngfirst);
      lngsecond = nmea[3].substring(i - 2).toFloat();
      //Serial.println(lngsecond);
    }
  }
  lngsecond = lngsecond / 60;
  String CalcLng = "";
  char charVal[9];
  dtostrf(lngsecond, 4, 6, charVal);
  for (int i = 0; i < sizeof(charVal); i++)
  {
    CalcLng += charVal[i];
  }
  lngfirst += CalcLng.substring(1);
  lngfirst = posneg += lngfirst;
  return lngfirst;
}
