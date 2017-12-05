#include <Adafruit_CircuitPlayground.h>
String notes[]={"G","A","B","G","G","A","B","G","B","C2","D2","B","C2","D2"};
  int duration[] = {200,200,200,200,200,200,200,200,200,200,400,200,200,400};
  int noteValue = 0;
void setup() {
  CircuitPlayground.begin();
  Serial.begin(9600);
  pinMode(6,INPUT);
  
}

void loop() {
  int songLength=sizeof(notes);
    for(int i=0;i<14&&digitalRead(6);i++)
    {
      if(notes[i]=="A0")
      {
        noteValue = 440;
      }
      else if(notes[i]=="A0#")
      {
        noteValue =466;
      }
      else if(notes[i]=="B0")
      {
        noteValue =494;
      }
      else if(notes[i]=="C")
      {
        noteValue =523;
      }
      else if(notes[i]=="C#")
      {
        noteValue =554;
      }
      else if(notes[i]=="D")
      {
        noteValue =587;
      }else if(notes[i]=="D#")
      {
        noteValue =622;
      }else if(notes[i]=="E")
      {
        noteValue =659;
      }else if(notes[i]=="F")
      {
        noteValue =698;
      }else if(notes[i]=="F#")
      {
        noteValue =740;
      }else if(notes[i]=="G")
      {
        noteValue =784;
      }else if(notes[i]=="G#")
      {
        noteValue =831;
      }else if(notes[i]=="A")
      {
        noteValue =880;
      }else if(notes[i]=="A#")
      {
        noteValue =932;
      }else if(notes[i]=="B")
      {
        noteValue =988;
      }else if(notes[i]=="C2")
      {
        noteValue =1047;
      }else if(notes[i]=="C2#")
      {
        noteValue =1109;
      }else if(notes[i]=="D2")
      {
        noteValue =1176;
      }


      
      if(noteValue!=0)
    {
      CircuitPlayground.playTone(noteValue,100);
      delay(duration[i]);
    }
    }
  

}
