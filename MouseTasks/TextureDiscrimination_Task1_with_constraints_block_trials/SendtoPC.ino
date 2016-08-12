void sendToPC(int TrialCount, int trial_result, int success_trial_count) {
	// if (curMillis - prevReplyToPCmillis >= replyToPCinterval) {
	// 	prevReplyToPCmillis += replyToPCinterval;
	// 	int valForPC = curMillis >> 9; // approx half seconds
	// 	Serial.print('<');
	// 	Serial.print(valForPC);
	// 	Serial.print('>');
	// }

  Serial.print(TrialCount);
  Serial.print(",");
  Serial.print(trial_result);
  Serial.print(",");
  Serial.print(success_trial_count);
  Serial.println(",");
}
