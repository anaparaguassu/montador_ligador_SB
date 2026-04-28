; o código transforma o input em binário
SECTION TEXT              
INPUT OLD_DATA            ; Le e guarda no OLD_DATA
LOAD OLD_DATA             ; Copia o OLD_DATA para o ACC

L1: DIV DOIS              ; Divide o ACC por DOIS
STORE  NEW_DATA           ; Salva no endereco NEW_DATA
MUL DOIS                  ; Multiplica o ACC por 2
STORE TMP_DATA            ; Salva em TMP_DATA
LOAD OLD_DATA             ; Carrega o OLD_DATA para o ACC
SUB TMP_DATA              ; Subtrai o TMP_DATA do ACC
STORE TMP_DATA            ; Salva em TMP_DATA.
OUTPUT TMP_DATA           ; Imprime (0 ou 1) no monitor.

COPY NEW_DATA, OLD_DATA   ; Copia o NEW_DATA para o OLD_DATA
LOAD OLD_DATA             ; Carrega esse novo OLD_DATA no ACC
JMPP L1                   ; Se o ACC > 0, pula de volta para o rotulo L1
STOP                      ; Para a execucao do programa quando o ACC == 0.

SECTION DATA              
DOIS: CONST 2             ; Cria uma constante DOIS com o valor 2
OLD_DATA: SPACE           ; Reserva 1 espaco na memoria para a variavel OLD_DATA
NEW_DATA: SPACE           ; Reserva 1 espaco na memoria para a variavel NEW_DATA
TMP_DATA: SPACE           ; Reserva 1 espaco na memoria para a variavel TMP_DATA