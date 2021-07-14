/*------------------------------------------------------------------------
  Desarrollo de Driver de Impresion 
  Desarrollador: Ing. Wilfredo Jiménez
  Empresa: JM_Ingenieros
  FECHA: 14-03-2021
  Proyecto: FRONTRERA ANGUIATU
  ------------------------------------------------------------------------*/
//LIBRERIA IMPRESORA
#include <thermalprinter.h>
#include <SoftwareSerial.h>
//LIBRERIA DESERIALIZADOR
#include <Separador.h>
//LIBRERIA RED ETHERNET
#include <SPI.h>
#include <Ethernet.h>
//#include <Controllino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <ArduinoJson.h>

#define MaxHeaderLength 200    //maximum length of http header required

#define ledPin 13
#define rxPin 5  //5
#define txPin 6 //6


Epson TM88 = Epson(rxPin, txPin); // init the Printer with Output-Pin

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEF };   //physical mac address
byte ip[] = { 192, 168, 0, 102 };   // static ip of Arduino
byte gateway[] = { 192, 168, 0, 1 };  // gateway address
byte subnet[] = { 255, 255, 255, 0 };  //subnet mask
IPAddress dpserver(192, 168, 0, 98);
EthernetServer server(80);   //web server port
String HttpHeader = String(MaxHeaderLength);
Separador s;
int stopbit = 0;

void setup()
{
 //enable serial monitor
  Serial.begin(9600);
  //start Ethernet
  Ethernet.begin(mac);
 //initialize variable
  HttpHeader="";
  delay(500);
  Serial.println("PRINTER TEST");
  Serial.println(Ethernet.localIP());
  TM88.start();
  TM88.justifyCenter();
  TM88.println("PRINTER TEST");
  TM88.print("IP Address: ");
  TM88.println(Ethernet.localIP());
  TM88.println();
  TM88.println("Serial COM: Virtual COM TTL");
  TM88.println("Baud Rates: 9600");
  TM88.reverseOn();
  TM88.println(" TAS EL SALVADOR ");  
  TM88.reverseOff();   
  TM88.feed(1);
  TM88.cut();
}

void loop(){
  // Create a client connection
  EthernetClient client = server.available();
  if (client) {
    while (client.connected()) {
      if (client.available()) {
         char c = client.read();
         //read MaxHeaderLength number of characters in the HTTP header
         //discard the rest until \n
         if (HttpHeader.length() < MaxHeaderLength)
         {
           //store characters to string
           HttpHeader = HttpHeader + c;
         }
         //if HTTP request has ended
         if (c == '\n') {
           // show the string on the monitor
           Serial.println(HttpHeader);
           httpEvent();
         // start of web page
          client.println("<!DOCTYPE html>");
          client.println("<html lang='en'>");
          client.println("<head>");
            client.println("<title>COMPASS - TAS EL SALVADOR</title>");
            client.println("<meta charset='utf-8'>");
            client.println("<meta name='viewport' content='width=device-width, initial-scale=1'>");
            client.println("<link rel='stylesheet' href='https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/css/bootstrap.min.css'>");
            client.println("<script src='https://ajax.googleapis.com/ajax/libs/jquery/3.4.1/jquery.min.js'></script>");
            client.println("<script src='https://maxcdn.bootstrapcdn.com/bootstrap/3.4.0/js/bootstrap.min.js'></script>");
          client.println("</head>");
          client.println("<body>");
          
          client.println("<div class='container'>");
            client.println("<h2>IMPRESOR COMPASS</h2>");
            client.println("<a href='/0,07/11/19,07:30,07/11/19,08:30,60,1001,1111-5555,0.50,test,0' class='btn btn-danger' role='button'>Test 1</a>");
            client.println("<a href='/0,06/11/19,17:45,06/11/19,18:30,45,0998,2222-5555,0.50,test,0' class='btn btn-danger' role='button'>Test 2</a>");
            client.println("<a href='/0,06/11/20,17:45,06/11/20,18:30,45,2001,2222-5555,0.50,print,0' class='btn btn-danger' role='button'>Print</a>");
            client.println("<a href='/0,99,99,99,99,99,99,99,99,setup,0' class='btn btn-danger' role='button'>Setup Parameters</a>");
         client.println("</div>");

         client.println("<div class='container'>");
            client.println("<div class='row'>");
              client.println("<div class='col-sm-4'>");
                client.println("<h3>SISTEMA DE IMPRESION DE FACTURAS</h3>");
                client.println("<p align='justify'>Permite la impresión de facturas del sistema COMPASS, diseñado como un complemento al sistema de control de acceso y permitiendo que se emitan facturas con número correlativo según resolución del MINISTERIO DE HACIENDA.</p>");
                client.println("<p align='justify'>Este servicio de impresión está diseñado para funcionar con peticiones HTTP enviadas desde el SERVIDOR (Servidor de control de acceso), después de realiza la lectura del TAG por la antena, se realizara verificación en las listas del sistema y se procederá a realizar el cálculo del monto a cobrarse por la estadía, la información recolectada y calculada por el servidor deberá ser serializada en una petición HTTP que el módulo de impresión pueda entender logrando así imprimir la factura en tiempo real.</p>");
                client.println("<p></p>");                
              client.println("</div>");
              client.println("<div class='col-sm-4'>");
                client.println("<h3>DETALLES DE USO</h3>");
                client.println("<p align='justify'>El módulo de impresión está configurado para recibir una dirección IP dinámica dentro de la red, de manera que el administrador de red deberá reservar a través de la MAC Address la dirección IP con la que será reconocido dentro del rango de los equipos de estacionamientos.</p>");
                client.println("<p align='justify'>El módulo recibe una petición HTTP con un formato especifico desarrollado con una codificación propietaria; dicha petición se ve en el ejemplo siguiente:</p>");
                client.println("<p align='justify'></p>");
                client.println("<p align='justify'>http://X.X.X.X/0,06/11/19,17:45,06/11/19,18:30,45,0998,</p>");
                client.println("2222-5555,0.50,print,0 </p>");
                client.println("<p></p>");                
              client.println("</div>");
              client.println("<div class='col-sm-4'>");
                client.println("<h3>ENVIO DE PETICION HTTP</h3>"); 
                client.println("<p align='justify'>SINTAXIS DE PETICION</p>");
                client.println("<p align='justify'></p>");
                client.println("<p align='justify'>Direccion Módulo = http://X.X.X.X</p>");
                client.println("<p align='justify'>Inicio de Petición = /0,</p>");
                client.println("<p align='justify'>Fecha de Entrada = 06/11/19,</p>");
                client.println("<p align='justify'>Hora de Entrada = 17:45,</p>");
                client.println("<p align='justify'>Fecha de Factura = 06/11/19,</p>");
                client.println("<p align='justify'>Hora de Factura = 18:30,</p>");
                client.println("<p align='justify'>Estadia(min.) = 45,</p>");
                client.println("<p align='justify'># Transacción = 0998,</p>");
                client.println("<p align='justify'># TAG = 2222-5555,</p>");
                client.println("<p align='justify'>Monto a cobrar($) = 0.50,</p>");
                client.println("<p align='justify'>Comando Impresión = print,</p>");
                client.println("<p align='justify'>Cierre de Petición = 0</p>");
                client.println("<p></p>");
                client.println("<p></p>");
                client.println("<p></p>");
                client.println("<p>TAS EL SALVADOR</p>");
              client.println("</div>");
            client.println("</div>");
          client.println("</div>");
          client.println("</body>");                    
          client.println("</html>");
           //clearing string for next read
           HttpHeader="";
           //stopping client
            client.stop();
         }
       }
     }
   }
}

void httpEvent()
{
  
  String datosrecibidos = HttpHeader;
  //separo datos recibidos
  //a la funcion separa debo pasarle el string que quiero serparar, el separador y el indice.
  String elemento0 = s.separa(datosrecibidos, ',', 0);// Arranque de secuencia
  String elemento1 = s.separa(datosrecibidos, ',', 1);// Fecha Entrada
  String elemento2 = s.separa(datosrecibidos, ',', 2);// Hora Entrada
  String elemento3 = s.separa(datosrecibidos, ',', 3);// Fecha Factura
  String elemento4 = s.separa(datosrecibidos, ',', 4);// Hora Factura
  String elemento5 = s.separa(datosrecibidos, ',', 5);// XX minutos
  String elemento6 = s.separa(datosrecibidos, ',', 6);// ## Transaccion
  String elemento7 = s.separa(datosrecibidos, ',', 7);// ## TAG
  String elemento8 = s.separa(datosrecibidos, ',', 8);// ## Monto a Cobrar
  String elemento9 = s.separa(datosrecibidos, ',', 9);// Comando
  String elemento10 = s.separa(datosrecibidos, ',', 10);// Cierre de secuencia


  if(elemento9=="test")
  {
  //digitalWrite(R01, HIGH); 
    
  Serial.println("COMANDO: "+ elemento9);
  Serial.println("todo funciona bien...!!!");
  Serial.println("");

  TM88.start();
  TM88.justifyCenter();
  TM88.reverseOn();
  TM88.println(" TAS EL SALVADOR ");  
  TM88.reverseOff();
  TM88.println();
  TM88.println();
  TM88.justifyLeft();
  TM88.println("--------------------------");  
  TM88.println("Fecha Entrada: "+ elemento1);
  TM88.println("Hora Entrada: "+ elemento2);
  TM88.println("Fecha Factura: "+ elemento3);
  TM88.println("Hora Factura: "+ elemento4);
  TM88.println("Estadia: "+ elemento5+ " mins.");
  TM88.println("Transaccion "+ elemento6);
  TM88.println("# TAG: "+ elemento7);
  TM88.println("Monto: $"+ elemento8);
  TM88.println("--------------------------");
  TM88.cut();
  delay(100);
  //digitalWrite(R01, LOW);
  elemento9 = "";
  HttpHeader="";
  
  }

 if(elemento9=="print")
  {
  //digitalWrite(R01, HIGH); 
  
  
  
  Serial.println("COMANDO: "+ elemento9);
  Serial.println("todo funciona bien...!!!");
  Serial.println("");

  TM88.start();
  TM88.justifyCenter();
  TM88.reverseOn();
  TM88.println(" TAS EL SALVADOR ");  
  TM88.reverseOff();
  TM88.println();
  TM88.println();
  TM88.justifyLeft();
  TM88.println("--------------------------");  
  TM88.println("Fecha Entrada: "+ elemento1);
  TM88.println("Hora Entrada: "+ elemento2);
  TM88.println("Fecha Factura: "+ elemento3);
  TM88.println("Hora Factura: "+ elemento4);
  TM88.println("Estadia: "+ elemento5+ " mins.");
  TM88.println("Transaccion "+ elemento6);
  TM88.println("# TAG: "+ elemento7);
  TM88.println("Monto: $"+ elemento8);
  TM88.println("--------------------------");
  TM88.cut();
  delay(100);
  //digitalWrite(R01, LOW);
  HttpHeader="";
  elemento0 = "";
  elemento1 = "";
  elemento2 = "";
  elemento3 = "";
  elemento4 = "";
  elemento5 = "";
  elemento6 = "";
  elemento7 = "";
  elemento8 = "";
  elemento9 = "";
  elemento10 = "";
  }

if(elemento9=="setup")
{
   
  //digitalWrite(R02, HIGH);
  Serial.println("COMANDO: "+ elemento9);
  Serial.println("todo funciona bien...!!!");
  Serial.println("");
  TM88.start();
  TM88.justifyCenter();
  TM88.println("PRINTER TEST");
  TM88.print("IP Address: ");
  TM88.println(Ethernet.localIP());
  TM88.print("Mask: ");
  TM88.println(Ethernet.subnetMask());
  TM88.print("Gateway: ");
  TM88.println(Ethernet.gatewayIP());
  TM88.print("DNS: ");
  TM88.println(Ethernet.dnsServerIP());
  TM88.println();
  TM88.println("Serial COM: Virtual COM TTL");
  TM88.println("Baud Rates: 9600");
  TM88.reverseOn();
  TM88.println(" TAS EL SALVADOR ");  
  TM88.reverseOff();   
  TM88.feed(1);
  TM88.cut();
  delay(100); 
  //digitalWrite(R02, LOW);
  HttpHeader="";
  elemento0 = "";
  elemento1 = "";
  elemento2 = "";
  elemento3 = "";
  elemento4 = "";
  elemento5 = "";
  elemento6 = "";
  elemento7 = "";
  elemento8 = "";
  elemento9 = "";
  elemento10 = "";
}
  
}
 



                  
                    

  
