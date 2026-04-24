; teste para EQU, IF e reordenacao de secoes

ZERO: EQU 0
UM: EQU 1
DOIS: EQU 2

SECTION DATA
VAR1: SPACE
VAR2: CONST DOIS

SECTION TEXT
INPUT VAR1

IF UM
COPY VAR1, VAR2    ; Esta linha DEVE aparecer (UM = 1)

IF ZERO
JMP FIM            ; Esta linha NAO DEVE aparecer (ZERO = 0)

SUB DOIS           ; O "DOIS" aqui deve virar "2"

IF 0
ADD DOIS           ; Esta linha NAO DEVE aparecer (0 direto)

OUTPUT VAR2
FIM: STOP