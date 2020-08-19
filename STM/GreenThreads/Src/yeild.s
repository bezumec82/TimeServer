  .syntax unified
  .thumb

.section .text
.global asmYield
.type  asmYield, %function
asmYield:
        /* r0 = &current */
        /* r1 = current->next */
        stmdb sp!, {r0-r12,lr}      //save context where we arrived from
        mrs r2, control             //read current control
#if(1)
        tst r2, #0x4                //test bit 3 - Floating Point Context Active
        it ne                       //if 1
        vstmdbne sp!, {s16-s31}     //save FPU registers
#endif
        stmdb sp!, {r2}             //save control to stack

        ldr r2, [r0]                //dereference context to the first member - stack
        str sp, [r2]                //save last value of stack pointer

        /* current = current->next */
        str r1, [r0]                //set next context as current
        ldr r0, [r0]                //dereference context to the first member - stack

        /* restore control first from stack */
        ldr r2, [r0]
        ldr r2, [r2]
        msr control, r2             //restore control
        isb

        ldr sp, [r0]                /* get new stack pointer */
        ldmia sp!, {r3}             //dummy read
#if(1)
        tst r2, #0x4                //test bit 3 - Floating Point Context Active
        it ne                       //if 1
        vldmiane sp!, {s16-s31}     //restore FPU registers
#endif
        ldmia sp!, {r0-r12, pc}     //continue execution of new context

.global asmStart
.type asmStart, %function
asmStart:
        /* r0 = &head */
        //no return to where we arrived from
        stmdb sp!, {r0-r12,lr} //just in case
        //get head stack
        ldr r0, [r0, #0]    //get head stack
        ldmia r0!, {r2}
        msr control, r2     //restore control - useless - it is already 0-0
        ldr sp, [r0, #0]    //get new stack pointer
        pop {r0-r12, pc}    //switch to the head's context
