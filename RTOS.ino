
/*
 *  Copyright(C) 2018 by Yukiya Ishioka
 *  https://qiita.com/Yukiya_Ishioka/items/ae21b4a1aecf69a885f2
 *  https://westgate-lab.hatenablog.com/entry/2020/03/28/113621
 */

#include <freertos/FreeRTOS.h>    /* FreeRTOSを用いるためのヘッダファイル */
#include <freertos/timers.h>      /* タイマを用いるためのヘッダファイル */
TimerHandle_t thand_test;

#include <freertos/queue.h>    /* メッセージキューを用いるためのヘッダファイル */
QueueHandle_t  qhand_test;
#define DEF_ITEM_SIZE   32

#define  LED1PIN   0    /* LED1のポート番号 */
#define  LED2PIN   2    /* LED2のポート番号 */


void timer_handler( void *param )
{
  Serial.print("timer_handler()\n");
}


void  task1( void *param )
{
  while( 1 ) {
    vTaskDelay(500);
    digitalWrite(LED1PIN, HIGH);  /* LED on */
    vTaskDelay(500);
    digitalWrite(LED1PIN, LOW);   /* LED off */
    vTaskDelay(1);
  }
}

void  task2( void *param )
{
  while( 1 ) {
    vTaskDelay(200);
    digitalWrite(LED2PIN, HIGH);  /* LED on */
    vTaskDelay(200);
    digitalWrite(LED2PIN, LOW);   /* LED off */
    vTaskDelay(1);
  }
}

void  task3( void *param )
{
  char  buff[ DEF_ITEM_SIZE ];

  while( 1 ) {
    /* メッセージ受信待ち */
    xQueueReceive( qhand_test, buff, portMAX_DELAY );
    Serial.printf( "xQueueReceive(): msg=%s\n", buff );
    vTaskDelay(1);
  }
}

void setup()
{
  Serial.begin(115200);
  Serial.print("setup()\n");

  pinMode(LED1PIN, OUTPUT);
  pinMode(LED2PIN, OUTPUT);
  digitalWrite(LED1PIN, LOW);  /* LED1 off */
  digitalWrite(LED2PIN, LOW);  /* LED2 off */

  /* create task */
  xTaskCreatePinnedToCore( task1,   /* タスクの入口となる関数名 */
                           "TASK1", /* タスクの名称 */
                           0x800,   /* スタックサイズ */
                           NULL,    /* パラメータのポインタ */
                           1,       /* プライオリティ */
                           NULL,    /* ハンドル構造体のポインタ */
                           0 );     /* 割り当てるコア (0/1) */

  xTaskCreatePinnedToCore( task2,
                           "TASK2",
                           0x800,
                           NULL,
                           2,
                           NULL,
                           1 );

/* create timer */
  thand_test = xTimerCreate( "TIM_TEST",  /* タイマの名称 */
                             2000,        /* 遅延時間 */
                             pdTRUE,      /* 自動繰り返しの有無 */
                             NULL,        /* ID変数のポインタ */
                             timer_handler ); /* タイマハンドラの関数名 */

  xTimerStart( thand_test, 0 ); /* タイマの開始 */
  vTaskDelay(500);
                           
}

void loop()
{
  Serial.print("loop()\n");
  vTaskDelay(1000);
}
