// LIBRERIA SERVO
#include <Servo.h>

// VARIABLE: CADENA DE BITS PARA DISPLAY 7 SEGMENTOS CATODO COMUN
byte numeros[10] = { 
0b0111111, // 0 
0b0000110, // 1 
0b1011011, // 2 
0b1001111, // 3 
0b1100110, // 4 
0b1101101, // 5 
0b1111101, // 6 
0b0000111, // 7 
0b1111111, // 8 
0b1101111 // 9
};
byte LETRA_ERROR = 0b1111001;

// VARIABLES: CAPACIDAD DE LA COCHERA MINIMA Y MAXIMA DE LA COCHERA
const int CAP_MIN = 0;
const int CAP_MAX = 7;
int conteo = 0;       // Variable que representa la cantidad de vehiculos adentro de la cochera

// VARIABLES: Tipo Sensor
struct Sensor{
  int pin;
  bool estado;
};
Sensor SensorEntradaAfuera = {0, false};
Sensor SensorEntradaAdentro = {1, false};
Sensor SensorSalidaAfuera = {11, false};
Sensor SensorSalidaAdentro = {12, false};

// VARIABLES: Tipo Puerta
struct Puerta{
  Servo servo;
  bool abierta;
  int pinServo;
};
Puerta PuertaEntrada = {Servo(), false, 9};
Puerta PuertaSalida = {Servo(), false, 10};

// VARIABLES: POSICIONES PINES PARA LOS SEGMENTOS DISPLAY 7 SEGMENTOS
const int segmento_a = 2;
const int segmento_b = 3;
const int segmento_c = 4;
const int segmento_d = 5;
const int segmento_e = 6;
const int segmento_f = 7;
const int segmento_g = 8;

// AGRUPANDO LOS PINES EN ARREGLOS
const int segmentos[7] = {segmento_a, segmento_b, segmento_c, segmento_d, segmento_e, segmento_f, segmento_g};
const int sensores[4] = {SensorEntradaAfuera.pin, SensorEntradaAdentro.pin , SensorSalidaAfuera.pin, SensorSalidaAdentro.pin};

// VARIABLES PARA LOS ESTADOS DE SECUENCIA LOGICO SEGÚN LA LECTURA DE LOS SENSORES
enum SecuenciaVehiculo{
  SIN_PRESENCIA,
  CARRO_EN_LA_PUERTA,
  CARRO_PASANDO_PUERTA,
  CARRO_PASO_PUERTA
};
SecuenciaVehiculo VehiculoEntrando = SIN_PRESENCIA;
SecuenciaVehiculo VehiculoSaliendo = SIN_PRESENCIA;

// FUNCIONES
// FUNCION PARA MOSTRAR EN EL DISPLAY EN BASE A UN NUMERO ENTERO COMO PARAMETRO DE ENTRADA
void mostrarNumero(int numero) {
  byte patron;
  if (numero < 0 || numero > 9) {
    patron = LETRA_ERROR;
  }
  else{
    patron = numeros[numero]; // Obtiene el patrón del número
  }
  // Recorre cada segmento y establece el estado
  for (int i = 0; i < 7; i++) {
    int estado = (patron >> i) & 0x01; // Extrae el bit correspondiente
    digitalWrite(segmentos[i], estado); // Activa o desactiva el segmento
  }
}

// FUNCION ABRIR PUERTA
void abrirPuerta(Puerta puerta){
  puerta.servo.write(90);
  puerta.abierta = true;
}

// FUNCION CERRAR PUERTA
void cerrarPuerta(Puerta puerta){
  puerta.servo.write(0);
  puerta.abierta = false;
}

// CONFIGURACION DE PINES
void setup() {
  // CONFIGURACION DE PINES DISPLAY 7 SEGMENTOS
  for (int i = 0; i < 7; i++) {
    pinMode(segmentos[i], OUTPUT);
  }
  // CONFIGURACION DE PINES PARA LOS SENSORES A USAR
  for (int j = 0; j < 4; j++){
    pinMode(sensores[j], INPUT);
  }
  // CONFIGURACION DE LOS PINES PARA LOS SERVOS
  PuertaEntrada.servo.attach(PuertaEntrada.pinServo);
  PuertaSalida.servo.attach(PuertaSalida.pinServo);
}

void loop() {

  // LECTURA DE SENSORES
  SensorEntradaAdentro.estado = digitalRead(SensorEntradaAdentro.pin);
  SensorEntradaAfuera.estado = digitalRead(SensorEntradaAfuera.pin);
  SensorSalidaAdentro.estado = digitalRead(SensorSalidaAdentro.pin);
  SensorSalidaAfuera.estado = digitalRead(SensorSalidaAfuera.pin);

  // SECUENCUA CUANDO EL VEHICULO ESTÁ ENTRANDO
  switch (VehiculoEntrando) {
    case SIN_PRESENCIA:
      cerrarPuerta(PuertaEntrada);
      if(SensorEntradaAfuera.estado && !SensorEntradaAdentro.estado && conteo < CAP_MAX){
        VehiculoEntrando = CARRO_EN_LA_PUERTA;
      }
      break;
    case CARRO_EN_LA_PUERTA:
      abrirPuerta(PuertaEntrada);
      if(SensorEntradaAfuera.estado && SensorEntradaAdentro.estado){
        VehiculoEntrando = CARRO_PASANDO_PUERTA;
      }
      if(!SensorEntradaAfuera.estado){
        VehiculoEntrando = SIN_PRESENCIA;
      }
      break;
    case CARRO_PASANDO_PUERTA:
      abrirPuerta(PuertaEntrada);
      if(!SensorEntradaAfuera.estado && SensorEntradaAdentro.estado){
        VehiculoEntrando = CARRO_PASO_PUERTA;
      }
      if(!SensorEntradaAdentro.estado){
        VehiculoEntrando = CARRO_EN_LA_PUERTA;
      }
      break;
    case CARRO_PASO_PUERTA:
      abrirPuerta(PuertaEntrada);
      if(SensorEntradaAfuera.estado){
        VehiculoEntrando = CARRO_PASANDO_PUERTA;
      }
      if(!SensorEntradaAfuera.estado && !SensorEntradaAdentro.estado){
        VehiculoEntrando = SIN_PRESENCIA;
        conteo = conteo + 1;
      }
      break;
  }

  // SECUENCIA CUANDO EL VEHICULO ESTÁ SALIENDO
  switch (VehiculoSaliendo) {
    case SIN_PRESENCIA:
      cerrarPuerta(PuertaSalida);
      // CUANDO EL VEHICULO SE ACERCA A LA PUERTA DE ENTRADA
      if(SensorSalidaAdentro.estado && !SensorSalidaAfuera.estado && conteo > CAP_MIN){
        // PASA AL SIGUIENTE ESTADO DE LA SECUENCIA EN DONDE SE ABRIRÁ LA PUERTA
        VehiculoSaliendo = CARRO_EN_LA_PUERTA;
      }
      break;
    case CARRO_EN_LA_PUERTA:
      // SE ABRE LA PUERTA
      abrirPuerta(PuertaSalida);
      // SI EL VEHICULO SIGUE INGRESANDO, SIENDO DETECTADO POR OTRO SENSOR
      if(SensorSalidaAdentro.estado && SensorSalidaAfuera.estado){
        // CAMBIO DE ESTADO
        VehiculoSaliendo = CARRO_PASANDO_PUERTA;
      }
      // SI EL VEHICULO RETROCEDE Y NO INGRESA
      if(!SensorSalidaAdentro.estado){
        // VUELVE AL ESTADO DE SECUENCIA ANTERIOR
        VehiculoSaliendo = SIN_PRESENCIA;
      }
      break;
    case CARRO_PASANDO_PUERTA:
      // LA PUERTA SIGUE ABIERTA
      abrirPuerta(PuertaSalida);
      if(!SensorSalidaAdentro.estado && SensorSalidaAfuera.estado){
        VehiculoSaliendo = CARRO_PASO_PUERTA;
      }
      if(!SensorSalidaAfuera.estado){
        VehiculoSaliendo = CARRO_EN_LA_PUERTA;
      }
      break;
    case CARRO_PASO_PUERTA:
      abrirPuerta(PuertaSalida);
      if(SensorSalidaAdentro.estado){
        VehiculoSaliendo = CARRO_PASANDO_PUERTA;
      }
      if(!SensorSalidaAdentro.estado && !SensorSalidaAfuera.estado){
        VehiculoSaliendo = SIN_PRESENCIA;
        conteo = conteo - 1;
      }
      break;
  }

  // MOSTRAR NUMERO EN DISPLAY
  mostrarNumero(CAP_MAX - conteo);
}
