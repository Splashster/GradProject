PROGRAM Test_Program
  VAR
    ESP0 AT %QX0.0 : BOOL;
    TMP AT %QX0.2 : BOOL;
    ESP1 AT %QX0.1 : BOOL;
  END_VAR
  VAR
    TMP2 : BOOL;
    TMP4 : BOOL;
    TMP3 : BOOL;
    TON1 : TON;
    TOF0 : TOF;
  END_VAR

  ESP0 := NOT(TMP);
  TON1(IN := ESP0, PT := T#10000ms);
  IF TON1.Q THEN
    ESP0 := FALSE; (*reset*)
  END_IF;
  TOF0(IN := TON1.Q, PT := T#5000ms);
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK MyTask(INTERVAL := T#50ms,PRIORITY := 0);
    PROGRAM instance0 WITH MyTask : Test_Program;
  END_RESOURCE
END_CONFIGURATION
