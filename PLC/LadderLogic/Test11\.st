PROGRAM Test_Program
  VAR
    ESP0 AT %QX0.0 : BOOL;
    TMP AT %QX1.3 : BOOL;
    ESP1 AT %QX0.1 : BOOL;
  END_VAR
  VAR
    TMP2 : BOOL;
    TMP4 : BOOL;
    TMP3 : BOOL;
    T0 : TOF;
    TON1 : TON;
    TON2 : TON;
    TON3 : TON;
  END_VAR

  T0(IN := TMP, PT := T#10000ms);
  ESP0 := T0.Q;
  TON1(IN := ESP0, PT := T#10000ms);
  ESP1 := TON1.Q;
  TON2(IN := ESP1, PT := T#10000ms);
  IF TON2.Q THEN
    ESP1 := FALSE; (*reset*)
  END_IF;
  TMP4 := NOT(TMP3);
  TON3(IN := TMP4, PT := T#30000ms);
  IF TON3.Q THEN
    ESP0 := FALSE; (*reset*)
  END_IF;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK MyTask(INTERVAL := T#50ms,PRIORITY := 0);
    PROGRAM instance0 WITH MyTask : Test_Program;
  END_RESOURCE
END_CONFIGURATION
