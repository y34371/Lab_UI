#ifndef CMDLIST_H
#define CMDLIST_H

#define PWM_DIS         0x10
#define PWM_EN          0x11
#define REC_DIS         0x12
#define REC_EN          0x13
#define DAB_PRI_DIS     0x14
#define DAB_PRI_EN      0x15

#define DAB_FREQ_INC    0x20
#define DAB_FREQ_DEC    0x21
#define DAB_FREQ_SET    0x22

#define DAB_PHS_INC     0x28
#define DAB_PHS_DEC     0x29
#define DAB_PHS_SET     0x2A

#define I_LOOP_PR       0x40
#define I_OFFSET        0x41

#define FAULT_DET_EN    0x60
#define FAULT_DET_DIS   0x61

#define RELAY_1_CLOSE   0x70
#define RELAY_1_OPEN    0x71
#define RELAY_2_CLOSE   0x72
#define RELAY_2_OPEN    0x73
#define LOAD_CLOSE      0x74
#define LOAD_OPEN       0x75

#define TRIGGER         0x78
#define RECT_ON         0x7A
#define RECT_OFF        0x7B
#define REAC_SET        0x80
#endif // CMDLIST_H
