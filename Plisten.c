// Алгоритм обнаружения входного выхода IoT

void pinListen(void *pvParameters) {
  uint8_t pState1 = 0, pState2 = 0, pState3 = 0, count = 0, current = 0,pCount = 0; 
  while (true) {
    // Convert to digital according to threashold and Combine the two states using bitwise operations
    uint8_t current = (((analogRead(4) > 2000) ? 1 : 0) << 1) | ((analogRead(5) > 2000) ? 1 : 0);
    if (current != 3) {  // Skip if "11"
      if (pState1 != current) {  // Only proceed if "current" is different from the previous state
        if (current == 0) { // When both sensors are "00"
          if (pState2 != 0) {
            // Here see if count is 1 then set 1 or if zero then set 0 
            pCount = (count == 1) ? 1 : (count == 0) ? 0 : pCount;
            // Check the previous states and update the count
            if (pState1 == 1 && pState3 != pState1) {
                count--;
            } else if (pState1 == 2 && pState3 != pState1) {
                count++;
            }
            // Here check the prvious and see how the state of count change
            if(count == 1 && pCount == 0 || count == 0 && pCount == 1){
              // From count 0 to 1 OR From count 1 to 0 (SEND INTERUPT)
              xTaskNotifyGive(InterptNotifHandle);
            }
          }
        }
        // Update the states after processing
        pState3 = pState2;
        pState2 = pState1;
        pState1 = current;
      }
    }
  }
}
