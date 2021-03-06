      SUBROUTINE DQAGIE(F,BOUND,INF,EPSABS,EPSREL,LIMIT,RESULT,ABSERR,
     *   NEVAL,IER,ALIST,BLIST,RLIST,ELIST,IORD,LAST)
C***BEGIN PROLOGUE  DQAGIE
C***DATE WRITTEN   800101   (YYMMDD)
C***REVISION DATE  830518   (YYMMDD)
C***CATEGORY NO.  H2A3A1,H2A4A1
C***KEYWORDS  AUTOMATIC INTEGRATOR, INFINITE INTERVALS,
C             GENERAL-PURPOSE, TRANSFORMATION, EXTRAPOLATION,
C             GLOBALLY ADAPTIVE
C***AUTHOR  PIESSENS,ROBERT,APPL. MATH & PROGR. DIV - K.U.LEUVEN
C           DE DONCKER,ELISE,APPL. MATH & PROGR. DIV - K.U.LEUVEN
C***PURPOSE  THE ROUTINE CALCULATES AN APPROXIMATION RESULT TO A GIVEN
C            INTEGRAL   I = INTEGRAL OF F OVER (BOUND,+INFINITY)
C            OR I = INTEGRAL OF F OVER (-INFINITY,BOUND)
C            OR I = INTEGRAL OF F OVER (-INFINITY,+INFINITY),
C            HOPEFULLY SATISFYING FOLLOWING CLAIM FOR ACCURACY
C            ABS(I-RESULT).LE.MAX(EPSABS,EPSREL*ABS(I))
C***DESCRIPTION
C
C INTEGRATION OVER INFINITE INTERVALS
C STANDARD FORTRAN SUBROUTINE
C
C            F      - SUBROUTINE F(X,IERR,RESULT) DEFINING THE INTEGRAND
C                     FUNCTION F(X). THE ACTUAL NAME FOR F NEEDS TO BE
C                     DECLARED E X T E R N A L IN THE DRIVER PROGRAM.
C
C            BOUND  - DOUBLE PRECISION
C                     FINITE BOUND OF INTEGRATION RANGE
C                     (HAS NO MEANING IF INTERVAL IS DOUBLY-INFINITE)
C
C            INF    - DOUBLE PRECISION
C                     INDICATING THE KIND OF INTEGRATION RANGE INVOLVED
C                     INF = 1 CORRESPONDS TO  (BOUND,+INFINITY),
C                     INF = -1            TO  (-INFINITY,BOUND),
C                     INF = 2             TO (-INFINITY,+INFINITY).
C
C            EPSABS - DOUBLE PRECISION
C                     ABSOLUTE ACCURACY REQUESTED
C            EPSREL - DOUBLE PRECISION
C                     RELATIVE ACCURACY REQUESTED
C                     IF  EPSABS.LE.0
C                     AND EPSREL.LT.MAX(50*REL.MACH.ACC.,0.5D-28),
C                     THE ROUTINE WILL END WITH IER = 6.
C
C            LIMIT  - INTEGER
C                     GIVES AN UPPER BOUND ON THE NUMBER OF SUBINTERVALS
C                     IN THE PARTITION OF (A,B), LIMIT.GE.1
C
C         ON RETURN
C            RESULT - DOUBLE PRECISION
C                     APPROXIMATION TO THE INTEGRAL
C
C            ABSERR - DOUBLE PRECISION
C                     ESTIMATE OF THE MODULUS OF THE ABSOLUTE ERROR,
C                     WHICH SHOULD EQUAL OR EXCEED ABS(I-RESULT)
C
C            NEVAL  - INTEGER
C                     NUMBER OF INTEGRAND EVALUATIONS
C
C            IER    - INTEGER
C                     IER = 0 NORMAL AND RELIABLE TERMINATION OF THE
C                             ROUTINE. IT IS ASSUMED THAT THE REQUESTED
C                             ACCURACY HAS BEEN ACHIEVED.
C                     IER.GT.0 ABNORMAL TERMINATION OF THE ROUTINE. THE
C                             ESTIMATES FOR RESULT AND ERROR ARE LESS
C                             RELIABLE. IT IS ASSUMED THAT THE REQUESTED
C                             ACCURACY HAS NOT BEEN ACHIEVED.
C                     IER.LT.0 EXIT REQUESTED FROM USER-SUPPLIED
C                             FUNCTION.
C
C            ERROR MESSAGES
C                     IER = 1 MAXIMUM NUMBER OF SUBDIVISIONS ALLOWED
C                             HAS BEEN ACHIEVED. ONE CAN ALLOW MORE
C                             SUBDIVISIONS BY INCREASING THE VALUE OF
C                             LIMIT (AND TAKING THE ACCORDING DIMENSION
C                             ADJUSTMENTS INTO ACCOUNT). HOWEVER,IF
C                             THIS YIELDS NO IMPROVEMENT IT IS ADVISED
C                             TO ANALYZE THE INTEGRAND IN ORDER TO
C                             DETERMINE THE INTEGRATION DIFFICULTIES.
C                             IF THE POSITION OF A LOCAL DIFFICULTY CAN
C                             BE DETERMINED (E.G. SINGULARITY,
C                             DISCONTINUITY WITHIN THE INTERVAL) ONE
C                             WILL PROBABLY GAIN FROM SPLITTING UP THE
C                             INTERVAL AT THIS POINT AND CALLING THE
C                             INTEGRATOR ON THE SUBRANGES. IF POSSIBLE,
C                             AN APPROPRIATE SPECIAL-PURPOSE INTEGRATOR
C                             SHOULD BE USED, WHICH IS DESIGNED FOR
C                             HANDLING THE TYPE OF DIFFICULTY INVOLVED.
C                         = 2 THE OCCURRENCE OF ROUNDOFF ERROR IS
C                             DETECTED, WHICH PREVENTS THE REQUESTED
C                             TOLERANCE FROM BEING ACHIEVED.
C                             THE ERROR MAY BE UNDER-ESTIMATED.
C                         = 3 EXTREMELY BAD INTEGRAND BEHAVIOUR OCCURS
C                             AT SOME POINTS OF THE INTEGRATION
C                             INTERVAL.
C                         = 4 THE ALGORITHM DOES NOT CONVERGE.
C                             ROUNDOFF ERROR IS DETECTED IN THE
C                             EXTRAPOLATION TABLE.
C                             IT IS ASSUMED THAT THE REQUESTED TOLERANCE
C                             CANNOT BE ACHIEVED, AND THAT THE RETURNED
C                             RESULT IS THE BEST WHICH CAN BE OBTAINED.
C                         = 5 THE INTEGRAL IS PROBABLY DIVERGENT, OR
C                             SLOWLY CONVERGENT. IT MUST BE NOTED THAT
C                             DIVERGENCE CAN OCCUR WITH ANY OTHER VALUE
C                             OF IER.
C                         = 6 THE INPUT IS INVALID, BECAUSE
C                             (EPSABS.LE.0 AND
C                              EPSREL.LT.MAX(50*REL.MACH.ACC.,0.5D-28),
C                             RESULT, ABSERR, NEVAL, LAST, RLIST(1),
C                             ELIST(1) AND IORD(1) ARE SET TO ZERO.
C                             ALIST(1) AND BLIST(1) ARE SET TO 0
C                             AND 1 RESPECTIVELY.
C
C            ALIST  - DOUBLE PRECISION
C                     VECTOR OF DIMENSION AT LEAST LIMIT, THE FIRST
C                      LAST  ELEMENTS OF WHICH ARE THE LEFT
C                     END POINTS OF THE SUBINTERVALS IN THE PARTITION
C                     OF THE TRANSFORMED INTEGRATION RANGE (0,1).
C
C            BLIST  - DOUBLE PRECISION
C                     VECTOR OF DIMENSION AT LEAST LIMIT, THE FIRST
C                      LAST  ELEMENTS OF WHICH ARE THE RIGHT
C                     END POINTS OF THE SUBINTERVALS IN THE PARTITION
C                     OF THE TRANSFORMED INTEGRATION RANGE (0,1).
C
C            RLIST  - DOUBLE PRECISION
C                     VECTOR OF DIMENSION AT LEAST LIMIT, THE FIRST
C                      LAST  ELEMENTS OF WHICH ARE THE INTEGRAL
C                     APPROXIMATIONS ON THE SUBINTERVALS
C
C            ELIST  - DOUBLE PRECISION
C                     VECTOR OF DIMENSION AT LEAST LIMIT,  THE FIRST
C                     LAST ELEMENTS OF WHICH ARE THE MODULI OF THE
C                     ABSOLUTE ERROR ESTIMATES ON THE SUBINTERVALS
C
C            IORD   - INTEGER
C                     VECTOR OF DIMENSION LIMIT, THE FIRST K
C                     ELEMENTS OF WHICH ARE POINTERS TO THE
C                     ERROR ESTIMATES OVER THE SUBINTERVALS,
C                     SUCH THAT ELIST(IORD(1)), ..., ELIST(IORD(K))
C                     FORM A DECREASING SEQUENCE, WITH K = LAST
C                     IF LAST.LE.(LIMIT/2+2), AND K = LIMIT+1-LAST
C                     OTHERWISE
C
C            LAST   - INTEGER
C                     NUMBER OF SUBINTERVALS ACTUALLY PRODUCED
C                     IN THE SUBDIVISION PROCESS
C
C***REFERENCES  (NONE)
C***ROUTINES CALLED  D1MACH,DQELG,DQK15I,DQPSRT
C***END PROLOGUE  DQAGIE
      DOUBLE PRECISION ABSEPS,ABSERR,ALIST,AREA,AREA1,AREA12,AREA2,A1,
     *  A2,BLIST,BOUN,BOUND,B1,B2,CORREC,DABS,DEFABS,DEFAB1,DEFAB2,
     *  DMAX1,DRES,D1MACH,ELIST,EPMACH,EPSABS,EPSREL,ERLARG,ERLAST,
     *  ERRBND,ERRMAX,ERROR1,ERROR2,ERRO12,ERRSUM,ERTEST,OFLOW,RESABS,
     *  RESEPS,RESULT,RES3LA,RLIST,RLIST2,SMALL,UFLOW
      INTEGER ID,IER,IERRO,INF,IORD,IROFF1,IROFF2,IROFF3,JUPBND,K,KSGN,
     *  KTMIN,LAST,LIMIT,MAXERR,NEVAL,NRES,NRMAX,NUMRL2
      LOGICAL EXTRAP,NOEXT
C
      DIMENSION ALIST(LIMIT),BLIST(LIMIT),ELIST(LIMIT),IORD(LIMIT),
     *  RES3LA(3),RLIST(LIMIT),RLIST2(52)
C
      EXTERNAL F
C
C            THE DIMENSION OF RLIST2 IS DETERMINED BY THE VALUE OF
C            LIMEXP IN SUBROUTINE DQELG.
C
C
C            LIST OF MAJOR VARIABLES
C            -----------------------
C
C           ALIST     - LIST OF LEFT END POINTS OF ALL SUBINTERVALS
C                       CONSIDERED UP TO NOW
C           BLIST     - LIST OF RIGHT END POINTS OF ALL SUBINTERVALS
C                       CONSIDERED UP TO NOW
C           RLIST(I)  - APPROXIMATION TO THE INTEGRAL OVER
C                       (ALIST(I),BLIST(I))
C           RLIST2    - ARRAY OF DIMENSION AT LEAST (LIMEXP+2),
C                       CONTAINING THE PART OF THE EPSILON TABLE
C                       WICH IS STILL NEEDED FOR FURTHER COMPUTATIONS
C           ELIST(I)  - ERROR ESTIMATE APPLYING TO RLIST(I)
C           MAXERR    - POINTER TO THE INTERVAL WITH LARGEST ERROR
C                       ESTIMATE
C           ERRMAX    - ELIST(MAXERR)
C           ERLAST    - ERROR ON THE INTERVAL CURRENTLY SUBDIVIDED
C                       (BEFORE THAT SUBDIVISION HAS TAKEN PLACE)
C           AREA      - SUM OF THE INTEGRALS OVER THE SUBINTERVALS
C           ERRSUM    - SUM OF THE ERRORS OVER THE SUBINTERVALS
C           ERRBND    - REQUESTED ACCURACY MAX(EPSABS,EPSREL*
C                       ABS(RESULT))
C           *****1    - VARIABLE FOR THE LEFT SUBINTERVAL
C           *****2    - VARIABLE FOR THE RIGHT SUBINTERVAL
C           LAST      - INDEX FOR SUBDIVISION
C           NRES      - NUMBER OF CALLS TO THE EXTRAPOLATION ROUTINE
C           NUMRL2    - NUMBER OF ELEMENTS CURRENTLY IN RLIST2. IF AN
C                       APPROPRIATE APPROXIMATION TO THE COMPOUNDED
C                       INTEGRAL HAS BEEN OBTAINED, IT IS PUT IN
C                       RLIST2(NUMRL2) AFTER NUMRL2 HAS BEEN INCREASED
C                       BY ONE.
C           SMALL     - LENGTH OF THE SMALLEST INTERVAL CONSIDERED UP
C                       TO NOW, MULTIPLIED BY 1.5
C           ERLARG    - SUM OF THE ERRORS OVER THE INTERVALS LARGER
C                       THAN THE SMALLEST INTERVAL CONSIDERED UP TO NOW
C           EXTRAP    - LOGICAL VARIABLE DENOTING THAT THE ROUTINE
C                       IS ATTEMPTING TO PERFORM EXTRAPOLATION. I.E.
C                       BEFORE SUBDIVIDING THE SMALLEST INTERVAL WE
C                       TRY TO DECREASE THE VALUE OF ERLARG.
C           NOEXT     - LOGICAL VARIABLE DENOTING THAT EXTRAPOLATION
C                       IS NO LONGER ALLOWED (TRUE-VALUE)
C
C            MACHINE DEPENDENT CONSTANTS
C            ---------------------------
C
C           EPMACH IS THE LARGEST RELATIVE SPACING.
C           UFLOW IS THE SMALLEST POSITIVE MAGNITUDE.
C           OFLOW IS THE LARGEST POSITIVE MAGNITUDE.
C
C***FIRST EXECUTABLE STATEMENT  DQAGIE
       EPMACH = D1MACH(4)
C
C           TEST ON VALIDITY OF PARAMETERS
C           -----------------------------
C
      IER = 0
      NEVAL = 0
      LAST = 0
      RESULT = 0.0D+00
      ABSERR = 0.0D+00
      ALIST(1) = 0.0D+00
      BLIST(1) = 0.1D+01
      RLIST(1) = 0.0D+00
      ELIST(1) = 0.0D+00
      IORD(1) = 0
      IF(EPSABS.LE.0.0D+00.AND.EPSREL.LT.DMAX1(0.5D+02*EPMACH,0.5D-28))
     *  IER = 6
       IF(IER.EQ.6) GO TO 999
C
C
C           FIRST APPROXIMATION TO THE INTEGRAL
C           -----------------------------------
C
C           DETERMINE THE INTERVAL TO BE MAPPED ONTO (0,1).
C           IF INF = 2 THE INTEGRAL IS COMPUTED AS I = I1+I2, WHERE
C           I1 = INTEGRAL OF F OVER (-INFINITY,0),
C           I2 = INTEGRAL OF F OVER (0,+INFINITY).
C
      BOUN = BOUND
      IF(INF.EQ.2) BOUN = 0.0D+00
      CALL DQK15I(F,BOUN,INF,0.0D+00,0.1D+01,RESULT,ABSERR,
     *  DEFABS,RESABS,IER)
      IF (IER .LT. 0) RETURN
C
C           TEST ON ACCURACY
C
      LAST = 1
      RLIST(1) = RESULT
      ELIST(1) = ABSERR
      IORD(1) = 1
      DRES = DABS(RESULT)
      ERRBND = DMAX1(EPSABS,EPSREL*DRES)
      IF(ABSERR.LE.1.0D+02*EPMACH*DEFABS.AND.ABSERR.GT.ERRBND) IER = 2
      IF(LIMIT.EQ.1) IER = 1
      IF(IER.NE.0.OR.(ABSERR.LE.ERRBND.AND.ABSERR.NE.RESABS).OR.
     *  ABSERR.EQ.0.0D+00) GO TO 130
C
C           INITIALIZATION
C           --------------
C
      UFLOW = D1MACH(1)
      OFLOW = D1MACH(2)
      RLIST2(1) = RESULT
      ERRMAX = ABSERR
      MAXERR = 1
      AREA = RESULT
      ERRSUM = ABSERR
      ABSERR = OFLOW
      NRMAX = 1
      NRES = 0
      KTMIN = 0
      NUMRL2 = 2
      EXTRAP = .FALSE.
      NOEXT = .FALSE.
      IERRO = 0
      IROFF1 = 0
      IROFF2 = 0
      IROFF3 = 0
      KSGN = -1
      IF(DRES.GE.(0.1D+01-0.5D+02*EPMACH)*DEFABS) KSGN = 1
C
C           MAIN DO-LOOP
C           ------------
C
      DO 90 LAST = 2,LIMIT
C
C           BISECT THE SUBINTERVAL WITH NRMAX-TH LARGEST ERROR ESTIMATE.
C
        A1 = ALIST(MAXERR)
        B1 = 0.5D+00*(ALIST(MAXERR)+BLIST(MAXERR))
        A2 = B1
        B2 = BLIST(MAXERR)
        ERLAST = ERRMAX
        CALL DQK15I(F,BOUN,INF,A1,B1,AREA1,ERROR1,RESABS,DEFAB1,IER)
        IF (IER .LT. 0) RETURN
        CALL DQK15I(F,BOUN,INF,A2,B2,AREA2,ERROR2,RESABS,DEFAB2,IER)
        IF (IER .LT. 0) RETURN
C
C           IMPROVE PREVIOUS APPROXIMATIONS TO INTEGRAL
C           AND ERROR AND TEST FOR ACCURACY.
C
        AREA12 = AREA1+AREA2
        ERRO12 = ERROR1+ERROR2
        ERRSUM = ERRSUM+ERRO12-ERRMAX
        AREA = AREA+AREA12-RLIST(MAXERR)
        IF(DEFAB1.EQ.ERROR1.OR.DEFAB2.EQ.ERROR2)GO TO 15
        IF(DABS(RLIST(MAXERR)-AREA12).GT.0.1D-04*DABS(AREA12)
     *  .OR.ERRO12.LT.0.99D+00*ERRMAX) GO TO 10
        IF(EXTRAP) IROFF2 = IROFF2+1
        IF(.NOT.EXTRAP) IROFF1 = IROFF1+1
   10   IF(LAST.GT.10.AND.ERRO12.GT.ERRMAX) IROFF3 = IROFF3+1
   15   RLIST(MAXERR) = AREA1
        RLIST(LAST) = AREA2
        ERRBND = DMAX1(EPSABS,EPSREL*DABS(AREA))
C
C           TEST FOR ROUNDOFF ERROR AND EVENTUALLY SET ERROR FLAG.
C
        IF(IROFF1+IROFF2.GE.10.OR.IROFF3.GE.20) IER = 2
        IF(IROFF2.GE.5) IERRO = 3
C
C           SET ERROR FLAG IN THE CASE THAT THE NUMBER OF
C           SUBINTERVALS EQUALS LIMIT.
C
        IF(LAST.EQ.LIMIT) IER = 1
C
C           SET ERROR FLAG IN THE CASE OF BAD INTEGRAND BEHAVIOUR
C           AT SOME POINTS OF THE INTEGRATION RANGE.
C
        IF(DMAX1(DABS(A1),DABS(B2)).LE.(0.1D+01+0.1D+03*EPMACH)*
     *  (DABS(A2)+0.1D+04*UFLOW)) IER = 4
C
C           APPEND THE NEWLY-CREATED INTERVALS TO THE LIST.
C
        IF(ERROR2.GT.ERROR1) GO TO 20
        ALIST(LAST) = A2
        BLIST(MAXERR) = B1
        BLIST(LAST) = B2
        ELIST(MAXERR) = ERROR1
        ELIST(LAST) = ERROR2
        GO TO 30
   20   ALIST(MAXERR) = A2
        ALIST(LAST) = A1
        BLIST(LAST) = B1
        RLIST(MAXERR) = AREA2
        RLIST(LAST) = AREA1
        ELIST(MAXERR) = ERROR2
        ELIST(LAST) = ERROR1
C
C           CALL SUBROUTINE DQPSRT TO MAINTAIN THE DESCENDING ORDERING
C           IN THE LIST OF ERROR ESTIMATES AND SELECT THE SUBINTERVAL
C           WITH NRMAX-TH LARGEST ERROR ESTIMATE (TO BE BISECTED NEXT).
C
   30   CALL DQPSRT(LIMIT,LAST,MAXERR,ERRMAX,ELIST,IORD,NRMAX)
        IF(ERRSUM.LE.ERRBND) GO TO 115
        IF(IER.NE.0) GO TO 100
        IF(LAST.EQ.2) GO TO 80
        IF(NOEXT) GO TO 90
        ERLARG = ERLARG-ERLAST
        IF(DABS(B1-A1).GT.SMALL) ERLARG = ERLARG+ERRO12
        IF(EXTRAP) GO TO 40
C
C           TEST WHETHER THE INTERVAL TO BE BISECTED NEXT IS THE
C           SMALLEST INTERVAL.
C
        IF(DABS(BLIST(MAXERR)-ALIST(MAXERR)).GT.SMALL) GO TO 90
        EXTRAP = .TRUE.
        NRMAX = 2
   40   IF(IERRO.EQ.3.OR.ERLARG.LE.ERTEST) GO TO 60
C
C           THE SMALLEST INTERVAL HAS THE LARGEST ERROR.
C           BEFORE BISECTING DECREASE THE SUM OF THE ERRORS OVER THE
C           LARGER INTERVALS (ERLARG) AND PERFORM EXTRAPOLATION.
C
        ID = NRMAX
        JUPBND = LAST
        IF(LAST.GT.(2+LIMIT/2)) JUPBND = LIMIT+3-LAST
        DO 50 K = ID,JUPBND
          MAXERR = IORD(NRMAX)
          ERRMAX = ELIST(MAXERR)
          IF(DABS(BLIST(MAXERR)-ALIST(MAXERR)).GT.SMALL) GO TO 90
          NRMAX = NRMAX+1
   50   CONTINUE
C
C           PERFORM EXTRAPOLATION.
C
   60   NUMRL2 = NUMRL2+1
        RLIST2(NUMRL2) = AREA
        CALL DQELG(NUMRL2,RLIST2,RESEPS,ABSEPS,RES3LA,NRES)
        KTMIN = KTMIN+1
        IF(KTMIN.GT.5.AND.ABSERR.LT.0.1D-02*ERRSUM) IER = 5
        IF(ABSEPS.GE.ABSERR) GO TO 70
        KTMIN = 0
        ABSERR = ABSEPS
        RESULT = RESEPS
        CORREC = ERLARG
        ERTEST = DMAX1(EPSABS,EPSREL*DABS(RESEPS))
        IF(ABSERR.LE.ERTEST) GO TO 100
C
C            PREPARE BISECTION OF THE SMALLEST INTERVAL.
C
   70   IF(NUMRL2.EQ.1) NOEXT = .TRUE.
        IF(IER.EQ.5) GO TO 100
        MAXERR = IORD(1)
        ERRMAX = ELIST(MAXERR)
        NRMAX = 1
        EXTRAP = .FALSE.
        SMALL = SMALL*0.5D+00
        ERLARG = ERRSUM
        GO TO 90
   80   SMALL = 0.375D+00
        ERLARG = ERRSUM
        ERTEST = ERRBND
        RLIST2(2) = AREA
   90 CONTINUE
C
C           SET FINAL RESULT AND ERROR ESTIMATE.
C           ------------------------------------
C
  100 IF(ABSERR.EQ.OFLOW) GO TO 115
      IF((IER+IERRO).EQ.0) GO TO 110
      IF(IERRO.EQ.3) ABSERR = ABSERR+CORREC
      IF(IER.EQ.0) IER = 3
      IF(RESULT.NE.0.0D+00.AND.AREA.NE.0.0D+00)GO TO 105
      IF(ABSERR.GT.ERRSUM)GO TO 115
      IF(AREA.EQ.0.0D+00) GO TO 130
      GO TO 110
  105 IF(ABSERR/DABS(RESULT).GT.ERRSUM/DABS(AREA))GO TO 115
C
C           TEST ON DIVERGENCE
C
  110 IF(KSGN.EQ.(-1).AND.DMAX1(DABS(RESULT),DABS(AREA)).LE.
     * DEFABS*0.1D-01) GO TO 130
      IF(0.1D-01.GT.(RESULT/AREA).OR.(RESULT/AREA).GT.0.1D+03.
     *OR.ERRSUM.GT.DABS(AREA)) IER = 6
      GO TO 130
C
C           COMPUTE GLOBAL INTEGRAL SUM.
C
  115 RESULT = 0.0D+00
      DO 120 K = 1,LAST
        RESULT = RESULT+RLIST(K)
  120 CONTINUE
      ABSERR = ERRSUM
  130 NEVAL = 30*LAST-15
      IF(INF.EQ.2) NEVAL = 2*NEVAL
      IF(IER.GT.2) IER=IER-1
  999 RETURN
      END
