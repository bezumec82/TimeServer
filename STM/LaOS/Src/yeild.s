  .syntax unified
  .thumb

.section .text
.global asmYield
.type  asmYield, %function
asmYield:
        /* r0 = &current */
        /* r1 = current->next */
        /* Stack content must be :
         * bottom ->                      top
         * pc -> r12..r0 -> ?s31..s16? -> control */
        stmdb sp!, {r4-r11,lr}      //save context where we arrived from
        mrs r2, control             //read current control

        tst r2, #0x4                //test bit 3 - Floating Point Context Active
        it ne                       //if 1 (Z clear)
        vstmdbne sp!, {s16-s31}     //save FPU registers

        stmdb sp!, {r2}             //save control - last to stack

        /* Check current control - switch to privileged */
        tst r2, #1  //test bit 1
        it ne       //if 1 (Z clear) == un-priveleged
        svcne #0    //change to priveleged

        ldr r3, [r0]    //dereference &context to context
                        //the first member of context structure - stack pointer
        str sp, [r3]    //re-write stack pointer

        /*--- current = current->next ---*/
        str r1, [r0]    //set next context as current
        ldr r0, [r0]    //dereference &context to context
                        //now context is the next context

        /* restore control - first from stack */
        ldr r3, [r0]    //dereference context to stack pointer
        ldr r3, [r3]    //dereference stack pointer to value
                        //last value on stack was control

                            //r3 contains control taken from stack
        msr control, r3     //restore control taken from stack
        isb

        ldr sp, [r0]                //get new stack pointer
        ldmia sp!, {r2}             //dummy read

        tst r3, #0x4                //test bit 3 - Floating Point Context Active
        it ne                       //if 1
        vldmiane sp!, {s16-s31}     //restore FPU registers

        ldmia sp!, {r4-r11, pc}     //continue execution of new context

.global asmStart
.type asmStart, %function
asmStart:
        /* r0 = &head */
        /* r1 - reserved for future expansion */
        //no return to where we arrived from
        stmdb sp!, {r4-r11,lr}      //just in case
        ldr r2, [r0]                //get head stack pointer
        ldr r2, [r2]                //get control - first at the stack top

        /* Check current control - that we actually can write control */
        mrs r3, control
        tst r3, #1  //test control bit 1
        it ne       //if 1 (Z clear) == un-priveleged
        svcne #0    //change to priveleged

        msr control, r2             //restore control from head stack
        isb
        ldr sp, [r0]                //get new stack pointer
        ldmia sp!, {r3}             //dummy read
        ldmia sp!, {r4-r11, pc}     //switch to the head's context

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

        /* SVC==0 -> change priority to priveleged */
        cmp r0, #0
        ittte eq
        mrseq r1, control   //read control
        biceq r1, #1        //clear bit 0
        msreq control, r1   //write control
        bne SVC_Handler_C   //other codes handle in 'C'
        .align 4


