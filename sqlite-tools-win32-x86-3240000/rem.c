/*  
 * File:         e-puck用コントローラ例  
 * Date:         2018/6/12  
 * Description:  e-puckを前進させ、前方の赤外線近接センサに障害物が近づいたら停止させる  
 */  
/*  
 * ロボットノード用のライブラリ、および各種センサノード用ライブラリの読み込み  
 * robot.h               : ロボットノード  
 * led.h                 : LEDノード  
 * differential_wheels.h : 差動二輪車両ノード  
 * camera.h              : カメラノード  
 * distance_sensor.h     : 距離センサノード  
 */  
#include <webots/robot.h>  
#include <webots/led.h>  
#include <webots/differential_wheels.h>  
#include <webots/camera.h>  
#include <webots/distance_sensor.h>  
#include <stdio.h>  
#include <string.h> /* strcpy()を使用するため必要 */  

#define TIME_STEP     64 /* ロボットの制御周期[ms] */  
#define ON             1 /* 論理値の真,LED点灯や各種フラグ値に使用 */  
#define OFF            0 /* 論理値の偽,LED消灯や各種フラグ値に使用 */  
#define NUM_LEDS      10 /* LEDの数 */  
#define NUM_DIST_SENS  8 /* LEDの数 */  
/*  
 * main関数。 引数はRobotノードの"controllerArgs"フィールドで設定する。  
 */  
int main(int argc, char **argv)  
{  
  /* 変数宣言する */  
  int i;  
  double current_time=0.0;  
  char device_name[13]; /* デバイス名を格納する文字列配列 */  
  double dist_value[NUM_DIST_SENS]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};  
         /* 赤外線近接センサの測定値 */  
           
  double dist_avr_value[NUM_DIST_SENS]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};  
    
  double sum_dist_value[NUM_DIST_SENS]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};  
    
  double dist_cal_value[NUM_DIST_SENS]={0.0,0.0,0.0,0.0,0.0,0.0,0.0,0.0};  
    
  int dist_count=0;  
    
    
  int led_value[NUM_LEDS] = {OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF,OFF};  
      /* 各LEDの点灯状態(ON/OFF)の設定 (1(ON), 0(OFF)) */  
  int speed_l = 0, speed_r = 0; /*左・右車輪の指令角速度 (×speedUnit[rad/s]) */  
   
  //int flag = 0; 
  //double time = 0; 
    
  /* デバイスタグの宣言…デバイスを取り扱うためのタグを宣言する。 */  
  WbDeviceTag camera;                         /* カメラ */  
  WbDeviceTag led[NUM_LEDS];                  /* LED */  
  WbDeviceTag distance_sensor[NUM_DIST_SENS]; /* 距離センサ */  
    
  /* WEBOTSの状態を初期化する */  
  wb_robot_init();  
    
  /* デバイス類を初期化する */  
  camera=wb_robot_get_device("camera");  /* デバイス名"camera"に対応するデバイスタグの取得 */  
  //wb_camera_enable(camera,TIME_STEP*16); /* TIME_STEP*16[ms]周期でカメラの動作を開始 */  
  //printf("Initialize camera ... done\n");  
    
  strcpy(device_name,"led0");           /* led設定用のデバイス名の設定 */  
  for(i=0;i<NUM_LEDS;i++) {  
    (device_name[3]='0'+i); /* 先頭から4文字目(数字)の文字コードを設定 */  
    (led[i]=wb_robot_get_device(device_name));/* デバイス名に対応するデバイスタグの取得 */  
    wb_led_set(led[i],led_value[i]); /* LEDを初期状態にする */  
  }  
  printf("Initialize LED... done\n");  
    
  strcpy(device_name,"ps0");           /* デバイス名の設定 */  
  for(i=0;i<NUM_DIST_SENS;i++) {  
    device_name[2]='0'+i; /* 先頭から3文字目(数字)の文字コードを設定 */  
    distance_sensor[i]=wb_robot_get_device(device_name);  
                          /* デバイス名に対応するデバイスタグの取得 */  
    wb_distance_sensor_enable(distance_sensor[i],TIME_STEP*4);  
                          /* TIME_STEP*4[ms]周期でセンサの動作を開始 */  
  }  
  printf("Initialize distance sensor... done\n");  
    
  /* シミュレーション開始(以下無限ループ) */  
  do {  
    /* センサ情報の取得 */  
    
    current_time=wb_robot_get_time();  
    for(i=0;i<NUM_DIST_SENS;i++) {  
      /* 赤外線近接センサ情報の取得 */  
      dist_value[i] = wb_distance_sensor_get_value(distance_sensor[i]);  
    /* 情報処理・制御計算 */  
    if(wb_robot_get_mode()!=0){/*先頭のセンサのうちどちらかが100以上なら*/  
      dist_value[i]=(double)(short)(unsigned short)dist_value[i];  
    }  
      
    dist_cal_value[i]=dist_value[i]-dist_avr_value[i];  
      
    }  
    if((current_time <= 10)){  
      dist_count++;  
      for(i=0;i<NUM_LEDS;i++){  
        sum_dist_value[i]+=dist_value[i];  
        dist_avr_value[i]=sum_dist_value[i]/dist_count;  
      }  
      speed_l = 0;  
      speed_r = 0;  
    }  
    else if((current_time>=10 && current_time<=11)){  
      led_value[8]=ON;  
      /*胴体のLEDを光らせてキャリブレーション完了を合図*/  
      speed_l = 0;   
      speed_r = 0;  
    }  
    else if((current_time>11 && current_time<=14)){  
      led_value[8]=ON;  
      /*胴体のLEDを光らせてキャリブレーション完了を合図*/  
      speed_l = 50;   
      speed_r = 50;  
    }  
    else{  
      led_value[8]=OFF;/*胴体のLEDを消す*/ 
	  for(i = 1; i < 4; i++){
		if((dist_cal_value[i]) > 20.0)
			led_value[i] = ON;
		else
			led_value[i] = OFF;
	}
	for(i = 4; i < 7; i++){
		if((dist_cal_value[i]) > 20.0)
			led_value[i+1] = ON;
		else
	led_value[i+1] = OFF;
	}
	if(i = 0){
		if((dist_cal_value[0]> 20.0)&&(dist_cal_value[7]> 20.0))
			led_value[i] = ON;
		else
			led_value[i] = OFF;
	}
      if((dist_cal_value[7]) > 100.0){/**/  
        speed_l = 100;  
        speed_r = -100;  
        printf("case1\n"); 
      }
      else if(((dist_cal_value[5]) > 100.0)&&((dist_cal_value[6]) < 100.0)){  
        speed_l = 40;  
        speed_r = 50;  
        printf("case2\n"); 
      } 
      else if((dist_cal_value[6]) < 100.0){  
        speed_l = 40;  
        speed_r = 90; 
        printf("case3\n"); 
      }
      else if((dist_cal_value[6]) >= 100.0){  
        speed_l = 90;  
        speed_r = 40; 
        printf("case9\n"); 
      }
      /*else if(((dist_cal_value[5]) > 50.0) && ((dist_cal_value[6]) > 50.0)){ 
        speed_l = 70;  
        speed_r = 40; 
        printf("case8\n"); 
      } */
      else if((dist_cal_value[5]) >= 100.0){ 
        speed_l = 70;  
        speed_r = 40; 
        printf("case4\n"); 
      } 
      
      else{    
        speed_l = 40;  
        speed_r = 70; 
        printf("case5\n"); 
      }  
    }    
      
    for(i=0;i<NUM_LEDS;i++){  
      wb_led_set(led[i],led_value[i]);  
    }  
    wb_differential_wheels_set_speed(speed_l,speed_r); /* ロボットの車輪用モータに指令を送る */  
    printf("TIME: %lf \n",current_time);  
    printf("Distance sensor:0:  %lf,1:  %lf,2:  %lf,3:  %lf,4:  %lf,5:  %lf,6:  %lf,7:  %lf\n"  
            , dist_cal_value[0], dist_cal_value[1], dist_cal_value[2], dist_cal_value[3]  
            , dist_cal_value[4], dist_cal_value[5], dist_cal_value[6], dist_cal_value[7]);  
    printf("SPEED: %d, %d\n", speed_l, speed_r);  
          
    /* 制御の実行周期(TIME_STEP[ms])だけ、時間を経過させる。 */  
    /* 終了を指示されたらループを終了する */  
  } while (wb_robot_step(TIME_STEP) != -1);  
    
  /* WEBOTSの状態をクリーンアップする(終了処理) */  
  wb_robot_cleanup();  
    
  return 0;  
}