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
  double threshold = 950;

  // Setando a velocidade inicial dos motores
  double leftSpeed = MAX_SPEED;
  double rightSpeed = MAX_SPEED;
  
  // Variáveis para o valor dos sensores 
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
    so0Value = wb_distance_sensor_get_value(so0);
    so1Value = wb_distance_sensor_get_value(so1);
    so2Value = wb_distance_sensor_get_value(so2);
    so3Value = wb_distance_sensor_get_value(so3);
    so4Value = wb_distance_sensor_get_value(so4);
    so5Value = wb_distance_sensor_get_value(so5);
    so6Value = wb_distance_sensor_get_value(so6);
    so7Value = wb_distance_sensor_get_value(so7);
    
    printf("s0: %f s1: %f s2: %f s3: %f s4: %f s5: %f s6: %f s7: %f\n", 
           so0Value, so1Value, so2Value, so3Value, so4Value, so5Value, so6Value,
           so7Value);
    fflush(stdout);
    
    switch(currentAction) {
      case FORWARD:
         if(so4Value > threshold || so5Value > threshold 
         || so6Value > threshold) {
           // SPIN LEFT
           leftSpeed = -MAX_SPEED;
           rightSpeed = MAX_SPEED;
           currentAction = LEFT_SPIN;
         }
         if(so3Value > threshold || so2Value > threshold 
         || so1Value > threshold) {
           // SPIN RIGHT
           leftSpeed = MAX_SPEED;
           rightSpeed = -MAX_SPEED;
           currentAction = RIGHT_SPIN;
         }
         else if (so0Value > threshold) {
           // TURN RIGHT
           leftSpeed = MAX_SPEED;
           rightSpeed = MAX_SPEED / 3;
           currentAction = RIGHT_TURN;
         }
         else if (so7Value > threshold) {
           // TURN LEFT
           leftSpeed = MAX_SPEED / 3;
           rightSpeed = MAX_SPEED;
           currentAction = LEFT_TURN;
         }
         
         break;      
     case LEFT_TURN:
       if(so7Value < threshold) {
         leftSpeed = MAX_SPEED;
         rightSpeed = MAX_SPEED;
         currentAction = FORWARD;
       }
       break;
       
     case RIGHT_TURN:
       if(so0Value < threshold) {
           leftSpeed = MAX_SPEED;
           rightSpeed = MAX_SPEED;
           currentAction = FORWARD;
       }
       break;
       
     case LEFT_SPIN:
       if(so4Value < threshold && so5Value < threshold 
       && so6Value < threshold){
           // FORWARD
           leftSpeed = MAX_SPEED;
           rightSpeed = MAX_SPEED;
           currentAction = FORWARD;
       }
       break;
     
     case RIGHT_SPIN:
       if(so3Value < threshold && so2Value < threshold 
       && so1Value < threshold){
           // FORWARD
           leftSpeed = MAX_SPEED;
           rightSpeed = MAX_SPEED;
           currentAction = FORWARD;
       }
       break;
       
     default:
       break;
    }
       
    wb_motor_set_velocity(frontLeftMotor, leftSpeed);
    wb_motor_set_velocity(backLeftMotor, leftSpeed);
    wb_motor_set_velocity(frontRightMotor, rightSpeed);
    wb_motor_set_velocity(backRightMotor, rightSpeed);
  };

  /* This is necessary to cleanup webots resources */
  wb_robot_cleanup();

  return 0;
}
