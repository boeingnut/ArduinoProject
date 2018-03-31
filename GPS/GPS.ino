void setup () {
  Serial.begin (9600);
}

void loop () {
  static char s[256];
  static int pos = 0;
  if (Serial.available()) {
    s[pos] = Serial.read();
      if (s[pos] == '\n') {
        s[pos - 1] = '\0'; // Escape '\r'
        analyze_data(s);
        pos = 0;
      } else {
        pos++;
      }
   }
}

void analyze_data(char *s) {
  char *type;
  char *time;
  char *latitude, *longitude;
  char *tmp1;
  char *tmp2;
  char *tmp3;
  type = strtok(s, ",");
  if(strcmp(s, "$GPRMC") != 0) {
    return;
  }

  time = strtok(NULL, ",");
  tmp1  = strtok(NULL, ",");
  latitude = strtok(NULL, ",");
  tmp2 = strtok(NULL, ",");
  longitude = strtok(NULL, ",");
  tmp3 = strtok(NULL, ",");

  
  Serial.print("Time: ");
  Serial.println(time);
  Serial.print("Latitude: ");
  Serial.print(latitude);
  Serial.println(tmp2);
  Serial.print("Longitude: ");
  Serial.print(longitude);
  Serial.println(tmp3);
  Serial.println();
  
  Serial.print(stod(latitude), 7);
  Serial.print(", ");
  Serial.println(stod(longitude), 7);
  Serial.println();
  delay(5000);
}

float stod(char *s) {
  float f;
  float deg, min;
  
  f = atof(s);

  deg = (int)f / 100;
  min = f - deg * 100;
  
  return deg + min / 60;
}
