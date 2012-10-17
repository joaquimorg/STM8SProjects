stm8/

    #include "mapping.inc" 

    segment 'rom'

start
    ldw X,#$stack_segment_end ; initialize SP
    ldw SP,X

    ldw X,#$ram0_segment_start ; clear all the ram
clear_ram
    clr (X)
    incw X
    cpw X,#$stack_segment_end
    jrule clear_ram
    
    bset $5011,#0 ; init our program
loop_forever
    ldw X,#$FFFF 
loop_delay
    nop
    decw X
    jrne loop_delay
    bcpl $500F,#0 
    jra loop_forever


; the default interupt handler
    interrupt NonHandledInterrupt
NonHandledInterrupt
    iret

; the interupt table
     segment 'vectit'
     dc.l {$82000000+start} ; reset
     dc.l {$82000000+NonHandledInterrupt} ; trap
     dc.l {$82000000+NonHandledInterrupt} ; irq0
     dc.l {$82000000+NonHandledInterrupt} ; irq1
     dc.l {$82000000+NonHandledInterrupt} ; irq2
     dc.l {$82000000+NonHandledInterrupt} ; irq3
     dc.l {$82000000+NonHandledInterrupt} ; irq4
     dc.l {$82000000+NonHandledInterrupt} ; irq5
     dc.l {$82000000+NonHandledInterrupt} ; irq6
     dc.l {$82000000+NonHandledInterrupt} ; irq7
     dc.l {$82000000+NonHandledInterrupt} ; irq8
     dc.l {$82000000+NonHandledInterrupt} ; irq9
     dc.l {$82000000+NonHandledInterrupt} ; irq10
     dc.l {$82000000+NonHandledInterrupt} ; irq11
     dc.l {$82000000+NonHandledInterrupt} ; irq12
     dc.l {$82000000+NonHandledInterrupt} ; irq13
     dc.l {$82000000+NonHandledInterrupt} ; irq14
     dc.l {$82000000+NonHandledInterrupt} ; irq15
     dc.l {$82000000+NonHandledInterrupt} ; irq16
     dc.l {$82000000+NonHandledInterrupt} ; irq17
     dc.l {$82000000+NonHandledInterrupt} ; irq18
     dc.l {$82000000+NonHandledInterrupt} ; irq19
     dc.l {$82000000+NonHandledInterrupt} ; irq20
     dc.l {$82000000+NonHandledInterrupt} ; irq21
     dc.l {$82000000+NonHandledInterrupt} ; irq22
     dc.l {$82000000+NonHandledInterrupt} ; irq23
     dc.l {$82000000+NonHandledInterrupt} ; irq24
     dc.l {$82000000+NonHandledInterrupt} ; irq25
     dc.l {$82000000+NonHandledInterrupt} ; irq26
     dc.l {$82000000+NonHandledInterrupt} ; irq27
     dc.l {$82000000+NonHandledInterrupt} ; irq28
     dc.l {$82000000+NonHandledInterrupt} ; irq29

    end
