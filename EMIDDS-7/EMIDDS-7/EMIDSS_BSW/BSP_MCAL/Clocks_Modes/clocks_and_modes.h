
#ifndef CLOCKS_AND_MODES_H_
#define CLOCKS_AND_MODES_H_

extern void SOSC_init_8MHz (void);

extern void SPLL_init_160MHz (void);

extern void NormalRUNmode_80MHz (void);

extern void WDOG_Disable(void);

//void SystemCoreClockUpdate (void);

#endif /* CLOCKS_AND_MODES_H_ */

