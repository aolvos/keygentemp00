_TITLE "bin encoder by aolvos"
REM $DYNAMIC
rest:
LINE INPUT ; "Enter Source Filename: ", f$
PRINT " "
IF NOT _FILEEXISTS(f$) THEN PRINT "File not found!": GOTO rest

SCREEN _LOADIMAGE(f$, 32)

xsize = _WIDTH(0)
ysize = _HEIGHT(0)


DIM ctable(3)
DIM datarray(xsize, ysize)

x = 0
y = 0
n = 0
s = 0
asw = 0
cnum = 1
DO
    pointcolor& = POINT(x, y)
    r = _RED32(pointcolor&)
    g = _GREEN32(pointcolor&)
    b = _BLUE32(pointcolor&)
    'PRINT #1, r; " "; g; " "; b; " "
    IF (s = 0) THEN
        s = 1
        ctable(asw) = r
        ctable(asw + 1) = g
        ctable(asw + 2) = b
    END IF
    ctablemaker:
    FOR i = 0 TO n STEP 3
        IF ctable(i) = r AND ctable(i + 1) = g AND ctable(i + 2) = b THEN
            asw = i
            GOTO outp
        END IF
    NEXT i
    n = n + 3
    cnum = cnum + 1
    REDIM _PRESERVE ctable(n + 3)
    ctable(n) = r
    ctable(n + 1) = g
    ctable(n + 2) = b
    GOTO ctablemaker
    outp:
    datarray(x, y) = asw / 3

    x = x + 1
    IF x = xsize THEN
        x = 0
        y = y + 1
    END IF
LOOP UNTIL y = ysize

f$ = LEFT$(f$, LEN(f$) - 4)

OPEN f$ + ".bin" FOR OUTPUT AS #1

oneb% = FIX(xsize / 256)
twob% = xsize - oneb% * 256
PRINT #1, CHR$(twob%);
PRINT #1, CHR$(oneb%);

oneb% = FIX(ysize / 256)
twob% = ysize - oneb% * 256
PRINT #1, CHR$(twob%);
PRINT #1, CHR$(oneb%);


PRINT #1, CHR$(cnum);

FOR i = 0 TO cnum * 3 - 3 STEP 3
    PRINT #1, CHR$(ctable(i));
    PRINT #1, CHR$(ctable(i + 1));
    PRINT #1, CHR$(ctable(i + 2));
NEXT i

x = 0
y = 0
n = 1
DO
    a% = datarray(x, y) + 1
    x = x + 1
    IF x = xsize THEN
        x = 0
        y = y + 1
    END IF
    b% = datarray(x, y) + 1
    IF a% = b% THEN
        n = n + 1
    ELSE
        ending:
        IF n > 1 AND n < 6 THEN
            DO
                PRINT #1, CHR$(a%);
                n = n - 1
            LOOP UNTIL n = 1
        END IF
        IF n >= 6 THEN
            na% = FIX(n / 65536)
            nb% = FIX((n - na% * 65536) / 256)
            nc% = n - na% * 65536 - nb% * 256
            PRINT #1, CHR$(0);
            PRINT #1, CHR$(nc%);
            PRINT #1, CHR$(nb%);
            PRINT #1, CHR$(na%);
            n = 1
        END IF
        PRINT #1, CHR$(a%);
    END IF
    IF y = ysize AND n > 1 THEN GOTO ending
LOOP UNTIL y = ysize AND n = 1
CLOSE #1

SYSTEM
