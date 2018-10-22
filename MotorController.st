PROGRAM Test_Program
  VAR
    ESP0 AT %QX0.0 : BOOL;
  END_VAR
  VAR
    TMP : BOOL;
  END_VAR
  VAR
    TMP0 AT %QX1.3 : BOOL;
    ESP1 AT %QX0.1 : BOOL;
  END_VAR
  VAR
    TON1 : TON;
    TON2 : TON;
  END_VAR

  ESP0 := NOT(TMP) AND TMP0;
  TON1(IN := ESP0 OR ESP0, PT := T#3000ms);
  ESP1 := TON1.Q;
  TON2(IN := ESP1, PT := T#5000ms);
  TMP := TON2.Q;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK MyTask(INTERVAL := T#50ms,PRIORITY := 0);
    PROGRAM instance0 WITH MyTask : Test_Program;
  END_RESOURCE
END_CONFIGURATION