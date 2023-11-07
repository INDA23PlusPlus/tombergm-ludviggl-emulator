
start:
    lxi     h, msg
    mvi     a, 0
loop:
    mov     a, m
    cpi     0
    jz      end
    out     0
    inx     h
    jmp     loop

end:
    hlt

msg: db 'Hello, world!', 0
