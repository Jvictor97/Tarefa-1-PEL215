/*
 * File: pionner_controller.c
 * Date: 16/06/2020
 * Description: Controller para o robô Pioneer
 * Author: João Souza
 */

/*
 * You may need to add include files like <webots/distance_sensor.h> or
 * <webots/motor.h>, etc.
 */
#include <webots/robot.h>
#include <webots/distance_sensor.h>
#include <webots/motor.h>
#include <math.h>
#include <stdio.h>

/*
 * You may want to add macros here.
 */
#define TIME_STEP 64

/*
 * This is the main program.
 * The arguments of the main function can be specified by the
 * "controllerArgs" field of the Robot node
 */
 
void delay (int timeMiliseconds) {
  double currentTime, initTime, timeLeft;
  double timeValue = (double) timeMiliseconds / 1000;
  initTime = wb_robot_get_time();
  timeLeft = 0.0;
  while (timeLeft < timeValue) {
    currentTime = wb_robot_get_time();
    timeLeft = currentTime - initTime;
    wb_robot_step(TIME_STEP);
  }
}

int main(int argc, char **argv) {
  /* necessary to initialize webots stuff */
  wb_robot_init();
 
  // Criando tags para os motores do robô
  WbDeviceTag frontLeftMotor = wb_robot_get_device("front left wheel");   // motor dianteiro esquerdo
  WbDeviceTag frontRightMotor = wb_robot_get_device("front right wheel"); // motor dianteiro direito
  WbDeviceTag backLeftMotor = wb_robot_get_device("back left wheel");    // motor traseiro esquerdo
  WbDeviceTag backRightMotor = wb_robot_get_device("back right wheel");  // motor traseiro direito
  
  // Criando tags para os sensores dianteiro (SO3) e traseiro (SO11)
  WbDeviceTag so0 = wb_robot_get_device("so0");
  WbDeviceTag so1 = wb_robot_get_device("so1");
  WbDeviceTag so2 = wb_robot_get_device("so2");  
  WbDeviceTag so3 = wb_robot_get_device("so3");
  WbDeviceTag so4 = wb_robot_get_device("so4");
  WbDeviceTag so5 = wb_robot_get_device("so5");
  WbDeviceTag so6 = wb_robot_get_device("so6");
  WbDeviceTag so7 = wb_robot_get_device("so7");
  
  /*
  WbDeviceTag frontSensor = wb_robot_get_device("so3");
  WbDeviceTag frontLeftSensor = wb_robot_get_device("so1");
  WbDeviceTag frontRightSensor = wb_robot_get_device("so6");
  WbDeviceTag backSensor = wb_robot_get_device("so11");
  WbDeviceTag backLeftSensor = wb_robot_get_device("so14");
  WbDeviceTag backRightSensor = wb_robot_get_device("so9");
  */

  // Habilitando os sensores
  /*
  wb_distance_sensor_enable(frontSensor, TIME_STEP);
  wb_distance_sensor_enable(backSensor, TIME_STEP);
  wb_distance_sensor_enable(frontLeftSensor, TIME_STEP);
  wb_distance_sensor_enable(frontRightSensor, TIME_STEP);
  wb_distance_sensor_enable(backLeftSensor, TIME_STEP);
  wb_distance_sensor_enable(backRightSensor, TIME_STEP);
  */
  
  wb_distance_sensor_enable(so0, TIME_STEP);
  wb_distance_sensor_enable(so1, TIME_STEP);
  wb_distance_sensor_enable(so2, TIME_STEP);
  wb_distance_sensor_enable(so3, TIME_STEP);
  wb_distance_sensor_enable(so4, TIME_STEP);
  wb_distance_sensor_enable(so5, TIME_STEP);
  wb_distance_sensor_enable(so6, TIME_STEP);
  wb_distance_sensor_enable(so7, TIME_STEP);
  
  // Configurando os motores
  wb_motor_set_position(frontLeftMotor, INFINITY);
  wb_motor_set_position(frontRightMotor, INFINITY);
  wb_motor_set_position(backLeftMotor, INFINITY);
  wb_motor_set_position(backRightMotor, INFINITY);   
  
  // Velocidade Máxima
  double MAX_SPEED = 2;
  // Limite de proximidade para os sensores
  double threshold = 900;

  // Setando a velocidade inicial dos motores
  double leftSpeed = MAX_SPEED;
  double rightSpeed = MAX_SPEED;
  
  // Variáveis para o valor dos sensores
  /*
  double frontValue;
  double backValue;
  double frontLeftValue;
  double frontRightValue;
  double backLeftValue;
  double backRightValue;
  */
  
  double so0Value, so1Value, so2Value, so3Value, so4Value, so5Value, so6Value, so7Value; 
  
  // Enum para mapear as ações
  enum actions { FORWARD, BACKWARD, LEFT_TURN, RIGHT_TURN, LEFT_SPIN, RIGHT_SPIN };
  
  // Variável para guardar a ação atual
  enum actions currentAction = FORWARD;

  /* main loop
   * Perform simulation steps of TIME_STEP milliseconds
   * and leave the loop when the simulation is over
   */
  while (wb_robot_step(TIME_STEP) != -1) {
    // Leitura dos sensores
    /*
    frontValue = wb_distance_sensor_get_value(frontSensor);
    backValue = wb_distance_sensor_get_value(backSensor);
    frontLeftValue = wb_distance_sensor_get_value(frontLeftSensor);
    frontRightValue = wb_distance_sensor_get_value(frontRightSensor);
    backLeftValue = wb_distance_sensor_get_value(backLeftSensor);
    backRightValue = wb_distance_sensor_get_value(backRightSensor);
    */
    
    so0Value = wb_distance_sensor_get_value(so0);
    so1Value = wb_distance_sensor_get_value(so1);
    so2Value = wb_distance_sensor_get_value(so2);
    so3Value = wb_distance_sensor_get_value(so3);
    so4Value = wb_distance_sensor_get_value(so4);
    so5Value = wb_distance_sensor_get_value(so5);
    so6Value = wb_distance_sensor_get_value(so6);
    so7Value = wb_distance_sensor_get_value(so7);
    
    // printf("F: %f FL: %f FR: %f\n", frontValue, frontLeftValue, frontRightValue);
    printf("s0: %f s1: %f s2: %f s3: %f s4: %f s5: %f s6: %f s7: %f\n", 
           so0Value, so1Value, so2Value, so3Value, so4Value, so5Value, so6Value, so7Value);
    fflush(stdout);
    
    switch(currentAction) {
      case FORWARD:
         if(so4Value > threshold || so3Value > threshold){
           // LEFT_SPIN
           leftSpeed = -MAX_SPEED;
           rightSpeed = MAX_SPEED;
           currentAction = LEFT_SPIN;
         }
         else if (so2Value > threshold || so1Value > threshold || so0Value > threshold) {
           // TURN RIGHT
           leftSpeed = MAX_SPEED * 3;
           rightSpeed = MAX_SPEED / 3;
           currentAction = RIGHT_TURN;
         }
         else if (so7Value > threshold || so6Value > threshold || so5Value > threshold) {
           // TURN LEFT
           leftSpeed = MAX_SPEED / 3;
           rightSpeed = MAX_SPEED * 3;
           currentAction = LEFT_TURN;
         }
         
         break;      
       
     case BACKWARD:
       if(so4Value < threshold && so3Value < threshold){
           // FORWARD
           leftSpeed = MAX_SPEED;
           rightSpeed = MAX_SPEED;
           currentAction = FORWARD;
        }
       break;
     
     case LEFT_TURN:
       if(so2Value < threshold && so1Value < threshold && so0Value < threshold) {
         leftSpeed = MAX_SPEED;
         rightSpeed = MAX_SPEED;
         currentAction = FORWARD;
       }
       break;
       
     case RIGHT_TURN:
       if(so7Value < threshold && so6Value < threshold && so5Value < threshold) {
           leftSpeed = MAX_SPEED;
           rightSpeed = MAX_SPEED;
           currentAction = FORWARD;
       }
       break;
       
     case LEFT_SPIN:
      if(so4Value < threshold && so3Value < threshold){
           // FORWARD
           leftSpeed = MAX_SPEED;
           rightSpeed = MAX_SPEED;
           currentAction = FORWARD;
        }
       break;
     
     case RIGHT_SPIN:
       break;
       
     default:
       break;
    }
       
    wb_motor_set_velocity(frontLeftMotor, leftSpeed);
    wb_motor_set_velocity(backLeftMotor, leftSpeed);
    wb_motor_set_velocity(frontRightMotor, rightSpeed);
    wb_motor_set_velocity(backRightMotor, rightSpeed);
    
    //if (currentAction == LEFT_TURN || currentAction == RIGHT_TURN) delay(300);
  };

  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();

  return 0;
}
