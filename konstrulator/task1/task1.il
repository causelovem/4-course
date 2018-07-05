#data $fmt = { b "%d ", b 0 }  # fmt = "%d ";
#data $arr = {w 10}

#export function w $main() {
#@start
#    %size =w copy 10
#    call $sieve(w %size, l $arr)

#    %i =l copy 0
#@loopPrint
#    %cond =w csltw %i, %n
#    jnz %cond, @loopPrintBody, @loopPrintEnd
#@loopPrintBody

#    %p1 =l mul %i, 4
#    %p2 =l add %prime, %p1
#    %p3 =w loadsb %p2

#    %cond =w csgtw %p3, 0
#    jnz %cond, @print, @incr

#@print
#    %t = w call $printf(l $fmt, w %i)

#@incr
#    %i =l add %i, 1
#    jmp @loopPrint
    
#@loopPrintEnd

#    ret 0
#}

export function $sieve(w %n, l %prime) {
@start
    %cond =w csgtw %n, 0
    jnz %cond, @cond1, @goNext1
@cond1
    storeb 0, %prime
@goNext1
    %cond =w csgtw %n, 1
    jnz %cond, @cond2, @goNext2
@cond2
    # %p1 =l add %prime, 4
    %p1 =l add %prime, 1
    storeb 0, %p1
@goNext2

    %i =l copy 2
@loop1
    %cond =w csltw %i, %n
    jnz %cond, @loop1Body, @loop1End
@loop1Body
                                
    # %p1 =l mul %i, 4
    %p2 =l add %prime, %i
    storeb 1, %p2

    %i =l add %i, 1
    jmp @loop1
@loop1End

    %i =l copy 1
@mainLoop
    %i =l add %i, 1
    %cond =w csltw %i, %n
    jnz %cond, @mainLoopBody, @mainLoopEnd
@mainLoopBody
    # %p1 =l mul %i, 4
    %p2 =l add %prime, %i
    %p3 =w loadsb %p2

    %cond =w ceqw %p3, 1
    jnz %cond, @inLoopInit, @mainLoop
@inLoopInit
    %j =l mul %i, %i
@inLoop
    %cond =w csltw %j, %n
    jnz %cond, @inLoopBody, @mainLoop
@inLoopBody
    # %p1 =l mul %j, 4
    %p2 =l add %prime, %j
    storeb 0, %p2

    %j =l add %j, %i
    jmp @inLoop
@mainLoopEnd

    ret
}
