  .syntax unified
  .thumb

.section .text
.global asmYeild
.type  asmYeild, %function

asmYeild:
        /* r0 = &current */
        /* r1 = current->next */
        push {r0-r12,lr}   //save context where we arrived from

        ldr r3, [r0]        //dereference &current
        str sp, [r3]

        /* current = current->next */
        str r1, [r0]        /* set next context as current */
        ldr r0, [r0]

        ldr sp, [r0, #0]    /* get new stack pointer */
        pop {r0-r12, pc}
