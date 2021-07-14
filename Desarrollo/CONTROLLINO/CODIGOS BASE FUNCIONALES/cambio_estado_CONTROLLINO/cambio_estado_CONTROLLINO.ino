/*------------------------------------------------------------------------
  Desarrollo de Driver de Control de Peso 
  Desarrollador: Ing. Wilfredo Jiménez
  Empresa: JM_Ingenieros
  FECHA: 14-03-2021
  Proyecto: FRONTRERA ANGUIATU
  ------------------------------------------------------------------------*/

#include <SPI.h>
#include <Controllino.h>
/*========================================================================
                         DECLARACION DE CONSTANTES
                      Las constantes no pueden cambiar
   ======================================================================== */

const int pinPulsador = CONTROLLINO_A6;         // pin al cual se conecta el pulsador
const int pinLed = CONTROLLINO_R0;              // pin al cual se conecta el LED
//Constantes para las monedas.
const int Peso1 = 5;
const int Peso2 = 7;
const int Peso3 = 10;

/* ========================================================================
                         DECLARACION DE VARIABLES
                       Las variables pueden cambiar
   ======================================================================== */

int contadorPulsaciones = 0;        // Contador: variable que acumula el numero de pulsaciones
int estadoPulsador = 0;             // Variable que almacena el estado actual del pulsador
int estadoAnteriorPulsador = 0;     // Variable que almacena el estado anterior del pulsador
unsigned long MillisUltPulso = 0;
int PulsosAcum = 0;
float pAcum = 0;
int MaxTimePulse = 200;
volatile int pulso = 0;

void setup() {
  pinMode(CONTROLLINO_A4, INPUT);
  pinMode(CONTROLLINO_A5, INPUT);
  //pinMode(CONTROLLINO_A6, INPUT);
  pinMode(pinPulsador, INPUT_PULLUP);       // Inicializa el pin del pulsador como entrada
  pinMode(pinLed, OUTPUT);           // Inicializa el pin del LED como salida
  Serial.begin(9600);                // Habilita la comunicación Serie
  Serial.println("PREPARADO"); 
}

void loop() 
{
  coinReader();  
  
  if (contadorPulsaciones % 4 == 0) {             // Enciende el LED una vez cada 4 pulsaciones 
    digitalWrite(pinLed, LOW);                   //// calculando el módulo del contador de pulsaciones.
  } else {                                        // La función módulo calcula el resto de una división 
   digitalWrite(pinLed, HIGH);                     //// entre dos números. Si el resto es cero, significa
  }                                               //// que el dividendo es múltiplo del divisor.

}


void coinReader()
{
unsigned long lastTime = millis() - MillisUltPulso;
  estadoPulsador = digitalRead(pinPulsador);      // Se lee y almacena el estado del pulsador

  if (estadoPulsador != estadoAnteriorPulsador) { // Se compara la variable estadoPulsador 
                                                  //// con su valor anterior
    if (estadoPulsador == LOW) {                 // Si el estado cambió, se incrementa el contador
      lastTime =0;                // Si el estado actual es HIGH entonces el pulsador
      pulso++;
      contadorPulsaciones++;                      //// pasó de un estado OFF a ON
      //Serial.println("Encendido");
      //Serial.print("numero de pulsaciones:  ");
      //Serial.println(contadorPulsaciones);
      MillisUltPulso = millis();
      //Serial.print("Pulses: ");
      //Serial.print(pulso);
      //Serial.print(" LastTime: ");
      //Serial.print(lastTime);
      //Serial.print(" LastPulse: ");
      //Serial.println(MillisUltPulso);
      
    } 
    else {                                        // Si el estado actual es LOW entonces el pulsador
      //Serial.println("Apagado");                  //// paso de un estado ON a OFF
    }
  }                                                  // Se guarda el estado actual como ultimo estado
  estadoAnteriorPulsador = estadoPulsador;        //// para la siguiente vuelta de la función loop     

if((pulso >= 5) && (lastTime >= MaxTimePulse))
{

    Serial.println("*********************************");
    //La cantidad de creditos es el contador y acumulador de pulsos, hasta que se cumpla alguna condicion.
    PulsosAcum = contadorPulsaciones;
    pulso = 0;
    Serial.print("Pulses: ");
    Serial.print(contadorPulsaciones);
    Serial.print(" LastTime: ");
    Serial.print(lastTime);
    Serial.print(" LastPulse: ");
    Serial.println(MillisUltPulso);  
    Serial.println("*********************************");

    //Validamos la moneda depositada.
switch (PulsosAcum){

  case Peso1:
    PulsosAcum = 0;
    contadorPulsaciones=0;
    Serial.println("Calculando secuencia 1");
    pAcum += Peso1;
    Serial.print("Total: $ ");
    Serial.print(pAcum);
    Serial.println("");
    break;

  case Peso2:
    PulsosAcum = 0;
    contadorPulsaciones=0;
    Serial.println("Calculando secuencia 2");
    pAcum += 0.10;
    Serial.print("Total: $ ");
    Serial.print(pAcum);
    Serial.println("");
    break;
    
  case Peso3:

    PulsosAcum = 0;
    contadorPulsaciones=0;
    Serial.println("Calculando secuencia 3");
    pAcum += 0.25;
    Serial.print("Total: $ ");
    Serial.print(pAcum);
    Serial.println("");
    break;  
  }
   
}
} 
