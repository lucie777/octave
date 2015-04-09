      SUBROUTINE CSHCH(Z, CSH, CCH)
C***BEGIN PROLOGUE  CSHCH
C***REFER TO  CBESK,CBESH
C
C     CSHCH COMPUTES THE COMPLEX HYPERBOLIC FUNCTIONS CSH=SINH(X+I*Y)
C     AND CCH=COSH(X+I*Y), WHERE I**2=-1.
C
C***ROUTINES CALLED  (NONE)
C***END PROLOGUE  CSHCH
      COMPLEX CCH, CSH, Z
      REAL CCHI, CCHR, CH, CN, CSHI, CSHR, SH, SN, X, Y, COSH, SINH
      X = REAL(Z)
      Y = AIMAG(Z)
      SH = SINH(X)
      CH = COSH(X)
      SN = SIN(Y)
      CN = COS(Y)
      CSHR = SH*CN
      CSHI = CH*SN
      CSH = CMPLX(CSHR,CSHI)
      CCHR = CH*CN
      CCHI = SH*SN
      CCH = CMPLX(CCHR,CCHI)
      RETURN
      END