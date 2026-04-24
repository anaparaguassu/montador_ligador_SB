; teste para rotulos isolados e COPY

rotulo_isolado:
    ADD 2      ; O montador deve colocar isso na mesma linha do rotulo

   sub 3       ; Testando a conversao para maiusculas e remocao de espacos iniciais

copy_teste: COPY   VAR1 ,   VAR2    ; O montador deve tirar os espacos ao redor da virgula

ROTULO_NORMAL: copy X, Y            ; Aqui tambem deve tirar o espaco depois da virgula