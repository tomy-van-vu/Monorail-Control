
/*
 * check if number is within tolerance range of target
 */
boolean withinTolerance (int num, int target, float tolerance) {
  
  int upper = target*(1+tolerance);
  int lower = target*(1-tolerance);

  if (num >= lower && num <= upper) {return true;}

  return false;
  
}
