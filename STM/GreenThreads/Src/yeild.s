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
        stmdb sp!, {r2}             //save control - last to stack

        ldr r3, [r0]                //dereference context to the first member - stack
        str sp, [r3]                //save last value of stack pointer

        /* current = current->next */
        str r1, [r0]                //set next context as current
        ldr r0, [r0]                //dereference context to the first member - stack

        /* restore control - first from stack */
        ldr r3, [r0]
        ldr r3, [r3]

        tst r2, #1  //test bit 1
        it ne       //if 1 == un-priveleged
        svcne #0    //change to priveleged

        msr control, r3             //restore control
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
        stmdb sp!, {r0-r12,lr}      //just in case
        ldr r2, [r0]                //get head stack
        ldr r2, [r2]                //get control - first at the top
        msr control, r2             //restore control - useless - it is already 0
        isb
        ldr sp, [r0]                //get new stack pointer
        ldmia sp!, {r3}             //dummy read
        ldmia sp!, {r0-r12, pc}     //switch to the head's context

.global SVC_Handler
.type SVC_Handler, %function
SVC_Handler:
        tst lr, #4      //test bit 2 of EXC_RETURN
        ite eq
        mrseq r0, msp   //if 0 - MSP stack was used
        mrsne r0, psp   //if 1 - PSP stack was used
        /* When exception occures next stacking take place :
        psr             -> sp+28    ||
        pc              -> sp+24    ||
        lr              -> sp+20   \  /
        r12             -> sp+16    \/
        r3              -> sp+12
        r2              -> sp+8
        r1              -> sp+4
        r0              -> sp
        Get stacked PC from the stack frame
        (stacked PC = address of instruction after SVC)
        */
        ldr r0, [r0, #24]
        ldrb r0, [r0, #-2]  //Get first byte of the SVC instruction
                            //now the SVC number is in R0

        /* Change priority to priveleged */
        cmp r0, #0
        ittte eq
        mrseq r1, control
        biceq r1, #1
        msreq control, r1
        bne SVC_Handler_C
        .align 4


