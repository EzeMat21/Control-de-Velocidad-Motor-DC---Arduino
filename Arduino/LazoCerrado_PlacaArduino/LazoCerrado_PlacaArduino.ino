//
//    FILE: AS5600_angular_speed_RPM.ino
//  AUTHOR: Rob Tillaart
// PURPOSE: demo
//     URL: https://github.com/RobTillaart/AS5600
//
//  Programa de control a lazo cerrado de un motor DC 
//  utilizando un sensor AS5600, utilizando la libreria de Rob Tillaart.


#include "AS5600.h"
#include <Wire.h>

const uint8_t IN1 = 12;
const uint8_t IN2 = 13;

volatile uint8_t contador = 0;
volatile uint16_t contador2 = 0;

const float Kp = 0.00123;
//const float Kp = 0.0833; 
const float I = 200;
const float Ts = 0.004;

const uint16_t MAX_PWM = 600;
const uint16_t TOP = 639;

volatile float referencia = 200; //la velocidad en rad/s.
float error = 0;
float accion_integral = 0;
float u = 0;    //La accion de control va entre 0-255;
float u1 = 0;

//  Uncomment the line according to your sensor type
//AS5600L as5600;   //  use default Wire
AS5600 as5600;   //  use default Wire

volatile bool flag_timer_interrupt = false;
float vel_medido = 0;
float vel_medido_1;
float vel_filtrada = 0;
float vel_filtrada_1 = 0;
const uint8_t N = 3;
const float UMBRAL = 200;
float t1,t2 = 0;


void PWM_init();
void Timer_init();



void setup()
{

    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    Serial.begin(115200,SERIAL_8E1);
    Wire.begin(); 

  as5600.begin(4);  //  set direction pin.
  as5600.setDirection(AS5600_CLOCK_WISE);  //  default, just be explicit.

  Serial.println(as5600.getAddress());

  int b = as5600.isConnected();
  Serial.print("Connect: ");
  Serial.println(b);

  cli();
    Timer_init();
    PWM_init();
  sei();

  delay(1000);
  
  //digitalWrite(IN2, HIGH);
  //digitalWrite(IN1, LOW);
  
}


void loop()
{  
  
 if(isnan(vel_medido = as5600.getAngularSpeed(AS5600_MODE_RADIANS)) == false && (flag_timer_interrupt == true)){

    contador2++;
        if(abs(vel_medido - vel_filtrada) <= UMBRAL){
    
            vel_filtrada = 0.7757*vel_filtrada_1 + 0.1122*vel_medido + 0.1122*vel_medido_1;
            Serial.println(vel_medido);  //Velocidad filtrada

            vel_filtrada_1 = vel_filtrada;
            vel_medido_1 = vel_medido;
           
        }

        flag_timer_interrupt = false;
        
        //Se genera la accion de control:
        error = referencia - vel_filtrada;
        
        //u = Kp*error;
        u = Kp*error + accion_integral;
        accion_integral = accion_integral + Kp*I*Ts*error;  
        
        if(u>0){
            digitalWrite(IN1, HIGH);
            digitalWrite(IN2, LOW);
        }
        else{
            digitalWrite(IN1, LOW);
            digitalWrite(IN2, HIGH);
        }


        u1 = u;
        u = (abs(u)*TOP/17);  //Convertir la accion de control que esta en unidades de Volts a un valor digital entre 0-255 (se multiplica po 255/19.5)
        
        if(u > MAX_PWM){
          u = MAX_PWM;
          u1 = (u1/abs(u1))*16;
        }
               
        OCR1A = (uint16_t )u;
        Serial.print("v:");
        Serial.println(u1);
        //Serial.print("error:");
        //Serial.println(error);
    if(contador2 >= 500 ){
      referencia = 300;
      contador2 = 0;
    }
  }
}


ISR(TIMER2_COMPA_vect){

  flag_timer_interrupt = true;
}


void PWM_init(){
  
  DDRB |= (1<<DDB1);  //Salida el pin OC2A
  TCCR1A = 0;
  TCCR1B = 0;
  
  TCCR1A |= (1<<COM1A1);        //Clear OC2A on compare match, set OC2A at BOTTOM, (non-inverting mode)
  TCCR1A |= (1<<WGM11);         //Fast PWM
   
  TCCR1B |= (1<<CS10);        //No prescaling, N=1; fPWM = 62.5kHz
  TCCR1B |= (1<<WGM13) | (1<<WGM12);  //Fast PWM con ICR1 como TOP value

  ICR1 = TOP;    //TOP Value
  OCR1A = 0;   
  
}

void Timer_init(){      //Interrupcion por Timer
  TCCR2A = 0;// set entire TCCR0A register to 0
  TCCR2B = 0;
  TCNT2  = 0;//initialize counter value to 0
  // set compare match register for 250Hz increments
  OCR2A = 249;// = (16*10^6) / (250*256) - 1 (must be <256) //Cada 4ms
  // turn on CTC mode
  TCCR2A |= (1 << WGM21);
  // Set CS01 and CS00 bits for 256 prescaler
  TCCR2B |= (1 << CS22) | (1<< CS21);   
  // enable timer compare interrupt
  TIMSK2 |= (1 << OCIE2A);
}
