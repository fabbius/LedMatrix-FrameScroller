
#include "FrameScroller.h" 

Arduino_LED_Matrix matrix;
FrameScroller scroller(&matrix);

void serialLog(const char* message) {
  Monitor.println(message);
}

void setup() {
  delay(500);
  Serial.begin(115200);
  delay(50);
  while (!Serial) delay(50);
  delay(500);
  Monitor.begin();
  delay(500);
  Monitor.println("Loading");
  delay(500);
  Monitor.print("is_use_test: ");
  delay(50);
  Monitor.println(is_use_test);
  delay(500);
  matrix.begin(); 

  scroller.setLog(FrameScroller::INFO, serialLog);  
  scroller.startTest(FrameScroller::TEST_WIDE_HOR_BIDIR, 0); 
}

void loop() { 
  scroller.ticker();
  scroller.show(); 
  delay(scroller.getWaitFrame());
}
