; -------- DMA burst transfer demo --------
; Configure DMA registers and start a burst transfer

; Set source address
MSI r1 0x0100        ; Load low part of source address
MHI r1 0x0001        ; Load high part of source address (full addr = 0x0100)
MSI r2 0x0000        ; DMA source register address
STR r1 r2            ; Write source address into DMA_SRC

; Set destination address
MSI r1 0x0200        ; Load low part of destination address
MHI r1 0x0002        ; Load high part of destination address (full addr = 0x0200)
MSI r2 0x0001        ; DMA destination register address
STR r1 r2            ; Write destination address into DMA_DST

; Set transfer length (burst size)
MSI r1 0x0008        ; Transfer length = 8 words        ; Amir
MSI r2 0x0002        ; DMA length register address      ; Amir
STR r1 r2            ; Write transfer length into DMA_LEN ; Amir

; Start DMA
MSI r1 0x0001        ; Control value: start = 1         ; Amir
MSI r2 0x0003        ; DMA control register address     ; Amir
STR r1 r2            ; Write control value into DMA_CTRL ; Amir

; Wait for DMA to finish (polling status register)
wait_dma:
MSI r2 0x0004        ; DMA status register address      ; Amir
LDR r3 r2            ; Load status
CMI r3 0x0000        ; Compare with 0 (still busy?)
BRR wait_dma r3      ; If busy, branch back to wait

; End of program
JMI r5 0             ; Jump to 0 to stop