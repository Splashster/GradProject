PROGRAM Mixer_Control_Program
  VAR
    Motor_State AT %QX0.0 : BOOL;
  END_VAR
  VAR
    TMP : BOOL;
  END_VAR
  VAR
    Activation_Switch AT %QX1.3 : BOOL;
    Motor_Direction AT %QX0.1 : BOOL;
  END_VAR
  VAR
    TON1 : TON;
    TON2 : TON;
  END_VAR

  Motor_State := NOT(TMP) AND Activation_Switch;
  TON1(IN := Motor_State OR Motor_State, PT := T#3000ms);
  Motor_Direction := TON1.Q;
  TON2(IN := Motor_Direction, PT := T#5000ms);
  TMP := TON2.Q;
END_PROGRAM

PROGRAM Fan_Control_Program
  VAR
    Fan1 AT %QX0.2 : BOOL;
    Fan2 AT %QX0.3 : BOOL;
    Critical_Temperature AT %QW4 : INT := 200;
    Overheat_Temperature AT %QW2 : INT := 120;
    Temperature AT %QW1 : INT := 0;
    Cooldown_Temperature AT %QW3 : INT := 70;
  END_VAR
  VAR
    GE13_OUT : BOOL;
    GT15_OUT : BOOL;
    OR19_OUT : BOOL;
    GE16_OUT : BOOL;
  END_VAR

  GE13_OUT := GE(Temperature, Overheat_Temperature);
  GT15_OUT := GT(Temperature, Cooldown_Temperature);
  OR19_OUT := OR(GE13_OUT, GT15_OUT);
  Fan1 := OR19_OUT;
  GE16_OUT := GE(Temperature, Critical_Temperature);
  Fan2 := GE16_OUT;
END_PROGRAM


CONFIGURATION Config0

  RESOURCE Res0 ON PLC
    TASK ControlMotors(INTERVAL := T#50ms,PRIORITY := 0);
    TASK ControlFan(INTERVAL := T#50ms,PRIORITY := 0);
    PROGRAM instance0 WITH ControlMotors : Mixer_Control_Program;
    PROGRAM instance1 WITH ControlMotors : Fan_Control_Program;
  END_RESOURCE
END_CONFIGURATION
