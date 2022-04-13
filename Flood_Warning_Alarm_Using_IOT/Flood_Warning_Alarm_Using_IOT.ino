//variabel ultra sonik
const int trigPin = 4;// D2
const int echoPin = 5;// D1
const int buzz = D5;
int stat = 1;
long duration;//waktu terbang
int distance;//Jarak yang terukur

// Inisialisasi ESP & Telegram Bot
#include <ESP8266WiFi.h>
#include <CTBot.h> 

CTBot bot;

//Konstanda Wifi & telegram bot
#define STASSID "*"//Replace with name of wifi
#define STAPSK  "*"//Replace with password of wifi
const char BotToken[] = "*";//Replace with Bot Token
const char* ssid     = STASSID;
const char* password = STAPSK;
int CHAT_ID = -579303151;
CTBotReplyKeyboard myKbd;   // reply keyboard object helper
bool isKeyboardActive;      // store if the reply keyboard is shown

//Variabel perhitungan sensor
int tinggisensor;
int batasnormal;

void setup() {
pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
pinMode(echoPin, INPUT); // Sets the echoPin as an Input
pinMode(buzz, OUTPUT);
digitalWrite(buzz, LOW);
Serial.begin(9600); // Starts the serial communication

 WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
     delay(500);
     Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println(WiFi.localIP());

  bot.wifiConnect(STASSID,STAPSK);
  bot.setTelegramToken (BotToken);

  tinggisensor =21;//22cm dari tanah
  batasnormal = 14;//14cm dari sensor atau 8cm dari tanah

    // reply keyboard customization
  // add a button that send a message with "Simple button" text
  myKbd.addButton("Cek Parameter");
  // add another button that send the user contact
  myKbd.addButton("Ubah Tinggi Sensor");
  // add another button that send the user position (location)
  myKbd.addButton("Ubah Batas Aman");
  // add a new empty button row
  myKbd.addRow();
  // add a button that send a message with "Hide replyKeyboard" text
  // (it will be used to hide the reply keyboard)
  myKbd.addButton("Cek tinggi air");
  // add a new empty button row
  myKbd.addRow();
  // add a button that send a message with "Hide replyKeyboard" text
  // (it will be used to hide the reply keyboard)
  myKbd.addButton("Sembunyikan Keyboard");
  // resize the keyboard to fit only the needed space
  myKbd.addRow();
  myKbd.addButton("Matikan Alarm");
  myKbd.addButton("Nyalakan Alarm");
  myKbd.addRow();
  myKbd.addButton("Status Alarm ");
  myKbd.enableResize();
  isKeyboardActive = false;    
}

void loop() {
//Mengukur Jarak
// Clears the trigPin
digitalWrite(trigPin, LOW);
delayMicroseconds(2);
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
// Calculating the distance
distance= duration*0.034/2;
// Prints the distance on the Serial Monitor
Serial.print("Distance: ");
Serial.println(distance);

// Perintah untuk mengirim pesan pada jarak tertentu
if (distance< batasnormal)
bot.sendMessage(CHAT_ID,"Bahaya!! Ketinggian banjir sudah "+ String (batasnormal-distance)+ "cm di luar batas aman dengan ketinggian banjir "+ String (tinggisensor-distance)+"cm dari tanah");
else if(distance >= batasnormal && distance<(tinggisensor-4) )
bot.sendMessage(CHAT_ID,"Hati-Hati!! genangan air telah berubah menjadi banjir dengan ketinggian "+ String (tinggisensor-distance)+ "cm dan "+ String (distance-batasnormal)+"cm lagi akan mencapai batas aman");
else if (distance>(tinggisensor-4) && distance<tinggisensor)
bot.sendMessage(CHAT_ID,"Siaga!! Sudah ada genangan air dengan ketinggian "+ String (tinggisensor-distance)+ "cm" );
else 
{}
delay (2000);
//alarm buzzer jika ketinggian banjir sudah di tahap bahaya
if (distance< batasnormal && stat == 1)
digitalWrite(buzz, HIGH);
else 
digitalWrite(buzz, LOW);

//tes
TBMessage msg;
TBMessage nmsg;

  // if there is an incoming message...
  if (bot.getNewMessage(msg)) {
    // check what kind of message I received
    if (msg.messageType == CTBotMessageText) {
      // received a text message
      if (msg.text.equalsIgnoreCase("tampilkan keyboard")) {
        // the user is asking to show the reply keyboard --> show it
        bot.sendMessage(msg.sender.id, "Keyboard ditampilkan. Anda bisa memeriksa parameter, mengubah tinggi sensor, dan mengubah batas jarak aman", myKbd);
        isKeyboardActive = true;
      }
      else if (msg.text.equalsIgnoreCase("Cek Parameter")) {
        // the user is asking to show the reply keyboard --> show it
        bot.sendMessage(msg.sender.id, "Tinggi sensor dari daratan: "+ String(tinggisensor)+"cm \nBatas normal sensor dengan air: "+ String(batasnormal)+"cm", myKbd);
        isKeyboardActive = true;}
          else if (msg.text.equalsIgnoreCase("Nyalakan Alarm")) {
        bot.sendMessage(msg.sender.id,"Alarm bahaya telah menyala" , myKbd);
        stat = 1;
        }
       else if (msg.text.equalsIgnoreCase("Matikan Alarm")) {
        bot.sendMessage(msg.sender.id,"Alarm bahaya telah mati" , myKbd);
        stat = 0;
        }
        else if (msg.text.equalsIgnoreCase("Status Alarm")) {
          if( stat == 0){
          bot.sendMessage(msg.sender.id,"Alarm bahaya sedang dalam keadaan mati" , myKbd);}
          else if (stat == 1){
           bot.sendMessage(msg.sender.id,"Alarm bahaya sedang dalam keadaan menyala" , myKbd);}
           else 
            delay(1);} 
        else if (msg.text.equalsIgnoreCase("Ubah tinggi sensor")) {
        bot.sendMessage(msg.sender.id,"Masukkan tinggi sensor dari daratan" , myKbd);
        for(int i; i<5000;i++){
           if (bot.getNewMessage(nmsg)) {
            tinggisensor = (nmsg.text.toInt());
            break;
            }
            else 
            delay(1);}
            bot.sendMessage(msg.sender.id, "Tinggi sensor dari daratan: "+ String(tinggisensor)+"cm \nBatas normal sensor dengan air: "+ String(batasnormal)+"cm", myKbd);
            }
            
        else if (msg.text.equalsIgnoreCase("Ubah batas aman")) {
        bot.sendMessage(msg.sender.id,"Masukkan batas jarak aman" , myKbd);
        for(int i; i<5000;i++){
           if (bot.getNewMessage(nmsg)) {
            batasnormal = (nmsg.text.toInt());
            break;
            }
            else 
            delay(1);}
            bot.sendMessage(msg.sender.id, "Tinggi sensor dari daratan: "+ String(tinggisensor)+"cm \nBatas normal sensor dengan air: "+ String(batasnormal)+"cm", myKbd);
            }
        else if (msg.text.equalsIgnoreCase("Cek tinggi air")) {
        // the user is asking to show the reply keyboard --> show it
        bot.sendMessage(msg.sender.id, "Ketinggian air: "+ String(distance) +"cm dari sensor", myKbd);
        isKeyboardActive = true;
      }
      // check if the reply keyboard is active 
      else if (isKeyboardActive) {
        // is active -> manage the text messages sent by pressing the reply keyboard buttons
        if (msg.text.equalsIgnoreCase("sembunyikan keyboard")) {
          // sent the "hide keyboard" message --> hide the reply keyboard
          bot.removeReplyKeyboard(msg.sender.id, "keyboard disembunyikan");
          isKeyboardActive = false;
        } else {
          // print every others messages received
          bot.sendMessage(msg.sender.id, msg.text);
        }
      } 
      
      else {
        // the user write anything else and the reply keyboard is not active --> show a hint message
        bot.sendMessage(msg.sender.id, "Coba 'tampilkan keyboard'");
      }
    }
    }
}
