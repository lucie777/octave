      SUBROUTINE XERRWV (MSG, NMES, NERR, LEVEL, NI, I1, I2, NR, R1, R2)
      INTEGER MSG, NMES, NERR, LEVEL, NI, I1, I2, NR,
     1   I, LUN, LUNIT, MESFLG, NCPW, NCH, NWDS
      DOUBLE PRECISION R1, R2 
      DIMENSION MSG(NMES)
C-----------------------------------------------------------------------
C SUBROUTINES XERRWV, XSETF, AND XSETUN, AS GIVEN HERE, CONSTITUTE
C A SIMPLIFIED VERSION OF THE SLATEC ERROR HANDLING PACKAGE.
C WRITTEN BY A. C. HINDMARSH AT LLNL.  VERSION OF MARCH 30, 1987.
C THIS VERSION IS IN DOUBLE PRECISION.
C
C ALL ARGUMENTS ARE INPUT ARGUMENTS.
C
C MSG    = THE MESSAGE (HOLLERITH LITERAL OR INTEGER ARRAY).
C NMES   = THE LENGTH OF MSG (NUMBER OF CHARACTERS).
C NERR   = THE ERROR NUMBER (NOT USED). 
C LEVEL  = THE ERROR LEVEL..
C          0 OR 1 MEANS RECOVERABLE (CONTROL RETURNS TO CALLER).
C          2 MEANS FATAL (RUN IS ABORTED--SEE NOTE BELOW).
C NI     = NUMBER OF INTEGERS (0, 1, OR 2) TO BE PRINTED WITH MESSAGE.
C I1,I2  = INTEGERS TO BE PRINTED, DEPENDING ON NI.
C NR     = NUMBER OF REALS (0, 1, OR 2) TO BE PRINTED WITH MESSAGE.
C R1,R2  = REALS TO BE PRINTED, DEPENDING ON NR.
C
C NOTE..  THIS ROUTINE IS MACHINE-DEPENDENT AND SPECIALIZED FOR USE
C IN LIMITED CONTEXT, IN THE FOLLOWING WAYS..
C 1. THE NUMBER OF HOLLERITH CHARACTERS STORED PER WORD, DENOTED
C    BY NCPW BELOW, IS A DATA-LOADED CONSTANT.
C 2. THE VALUE OF NMES IS ASSUMED TO BE AT MOST 60.
C    (MULTI-LINE MESSAGES ARE GENERATED BY REPEATED CALLS.) 
C 3. IF LEVEL = 2, CONTROL PASSES TO THE STATEMENT   STOP
C    TO ABORT THE RUN.  THIS STATEMENT MAY BE MACHINE-DEPENDENT.
C 4. R1 AND R2 ARE ASSUMED TO BE IN DOUBLE PRECISION AND ARE PRINTED
C    IN D21.13 FORMAT.
C 5. THE COMMON BLOCK /EH0001/ BELOW IS DATA-LOADED (A MACHINE-
C    DEPENDENT FEATURE) WITH DEFAULT VALUES.
C    THIS BLOCK IS NEEDED FOR PROPER RETENTION OF PARAMETERS USED BY
C    THIS ROUTINE WHICH THE USER CAN RESET BY CALLING XSETF OR XSETUN.
C    THE VARIABLES IN THIS BLOCK ARE AS FOLLOWS.. 
C       MESFLG = PRINT CONTROL FLAG..
C                1 MEANS PRINT ALL MESSAGES (THE DEFAULT).
C                0 MEANS NO PRINTING.
C       LUNIT  = LOGICAL UNIT NUMBER FOR MESSAGES.
C                THE DEFAULT IS 6 (MACHINE-DEPENDENT).
C-----------------------------------------------------------------------
C THE FOLLOWING ARE INSTRUCTIONS FOR INSTALLING THIS ROUTINE
C IN DIFFERENT MACHINE ENVIRONMENTS.
C
C TO CHANGE THE DEFAULT OUTPUT UNIT, CHANGE THE DATA STATEMENT
C IN THE BLOCK DATA SUBPROGRAM BELOW.
C
C FOR A DIFFERENT NUMBER OF CHARACTERS PER WORD, CHANGE THE 
C DATA STATEMENT SETTING NCPW BELOW, AND FORMAT 10.  ALTERNATIVES FOR 
C VARIOUS COMPUTERS ARE SHOWN IN COMMENT CARDS.
C
C FOR A DIFFERENT RUN-ABORT COMMAND, CHANGE THE STATEMENT FOLLOWING
C STATEMENT 100 AT THE END.
C-----------------------------------------------------------------------
      COMMON /EH0001/ MESFLG, LUNIT
C-----------------------------------------------------------------------
C THE FOLLOWING DATA-LOADED VALUE OF NCPW IS VALID FOR THE CDC-6600
C AND CDC-7600 COMPUTERS.
C     DATA NCPW/10/ 
C THE FOLLOWING IS VALID FOR THE CRAY-1 COMPUTER. 
C     DATA NCPW/8/
C THE FOLLOWING IS VALID FOR THE BURROUGHS 6700 AND 7800 COMPUTERS.
C     DATA NCPW/6/
C THE FOLLOWING IS VALID FOR THE PDP-10 COMPUTER. 
C     DATA NCPW/5/
C THE FOLLOWING IS VALID FOR THE VAX COMPUTER WITH 4 BYTES PER INTEGER,
C AND FOR THE IBM-360, IBM-370, IBM-303X, AND IBM-43XX COMPUTERS.
      DATA NCPW/4/
C THE FOLLOWING IS VALID FOR THE PDP-11, OR VAX WITH 2-BYTE INTEGERS. 
C     DATA NCPW/2/
C-----------------------------------------------------------------------
      IF (MESFLG .EQ. 0) GO TO 100
C GET LOGICAL UNIT NUMBER. ---------------------------------------------
      LUN = LUNIT
C GET NUMBER OF WORDS IN MESSAGE. --------------------------------------
      NCH = MIN0(NMES,60)
      NWDS = NCH/NCPW
      IF (NCH .NE. NWDS*NCPW) NWDS = NWDS + 1
C WRITE THE MESSAGE. ---------------------------------------------------
      WRITE (LUN, 10) (MSG(I),I=1,NWDS) 
C-----------------------------------------------------------------------
C THE FOLLOWING FORMAT STATEMENT IS TO HAVE THE FORM
C 10  FORMAT(1X,MMANN)
C WHERE NN = NCPW AND MM IS THE SMALLEST INTEGER .GE. 60/NCPW.
C THE FOLLOWING IS VALID FOR NCPW = 10. 
C 10  FORMAT(1X,6A10)
C THE FOLLOWING IS VALID FOR NCPW = 8.
C 10  FORMAT(1X,8A8)
C THE FOLLOWING IS VALID FOR NCPW = 6.
C 10  FORMAT(1X,10A6)
C THE FOLLOWING IS VALID FOR NCPW = 5.
C 10  FORMAT(1X,12A5)
C THE FOLLOWING IS VALID FOR NCPW = 4.
  10  FORMAT(1X,15A4)
C THE FOLLOWING IS VALID FOR NCPW = 2.
C 10  FORMAT(1X,30A2)
C-----------------------------------------------------------------------
      IF (NI .EQ. 1) WRITE (LUN, 20) I1 
 20   FORMAT(6X,23HIN ABOVE MESSAGE,  I1 =,I10)
      IF (NI .EQ. 2) WRITE (LUN, 30) I1,I2
 30   FORMAT(6X,23HIN ABOVE MESSAGE,  I1 =,I10,3X,4HI2 =,I10)
      IF (NR .EQ. 1) WRITE (LUN, 40) R1 
 40   FORMAT(6X,23HIN ABOVE MESSAGE,  R1 =,D21.13)
      IF (NR .EQ. 2) WRITE (LUN, 50) R1,R2
 50   FORMAT(6X,15HIN ABOVE,  R1 =,D21.13,3X,4HR2 =,D21.13) 
C ABORT THE RUN IF LEVEL = 2. ------------------------------------------
 100  IF (LEVEL .NE. 2) RETURN
      CALL XSTOPX (' ')
C----------------------- END OF SUBROUTINE XERRWV ----------------------
      END 
