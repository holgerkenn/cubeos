/* general cpu */

#define SIM_MCR 	SIM_BASE + 0x00 // Module Control register
#define SIM_SYNCR  	SIM_BASE + 0x04 // Clock synthesiser control register
#define SIM_RSR 	SIM_BASE + 0x07 // Reset Status
#define SIM_SYPCR 	SIM_BASE + 0x21 // System Protection
#define SIM_PICR 	SIM_BASE + 0x22 // Periodic Timer
#define SIM_PITR 	SIM_BASE + 0x24 //
#define SIM_SWSR 	SIM_BASE + 0x27 //
#define SIM_CSPAR0 	SIM_BASE + 0x44 // chip sellect pin assignment
#define SIM_CSPAR1 	SIM_BASE + 0x46 //
#define SIM_CSBARBT	SIM_BASE + 0x48 // boot chip select
#define SIM_CSORBT 	SIM_BASE + 0x4a //
#define SIM_CSBAR0 	SIM_BASE + 0x4c // chip selects
#define SIM_CSOR0 	SIM_BASE + 0x4e
#define SIM_CSBAR1 	SIM_BASE + 0x50
#define SIM_CSOR1 	SIM_BASE + 0x52
#define SIM_CSBAR2 	SIM_BASE + 0x54
#define SIM_CSOR2 	SIM_BASE + 0x56
#define SIM_CSBAR3      SIM_BASE + 0x58
#define SIM_CSOR3       SIM_BASE + 0x5a
#define SIM_CSBAR4      SIM_BASE + 0x5c
#define SIM_CSOR4       SIM_BASE + 0x5e
#define SIM_CSBAR5      SIM_BASE + 0x60
#define SIM_CSOR5       SIM_BASE + 0x62
#define SIM_CSBAR6      SIM_BASE + 0x64
#define SIM_CSOR6       SIM_BASE + 0x66

#define SIM_PEPAR	SIM_BASE + 0x17
#define SIM_DDRE	SIM_BASE + 0x15
#define SIM_PORTE	SIM_BASE + 0x11

#define SIM_PFPAR	SIM_BASE + 0x1F
#define SIM_DDRF	SIM_BASE + 0x1D
#define SIM_PORTF	SIM_BASE + 0x19

#define QSM_QSMCR	SCI_BASE + 0x00
#define QSM_QTEST	SCI_BASE + 0x02
#define QSM_QILR	SCI_BASE + 0x04
#define QSM_QIVR	SCI_BASE + 0x05

#define SCI_SCSR 	SCI_BASE + 0x0c
#define SCI_SCDR 	SCI_BASE + 0x0e
#define SCI_SCCR0       SCI_BASE + 0x08
#define SCI_SCCR1       SCI_BASE + 0x0a

#define SPI_SPCR0	SCI_BASE + 0x18
#define SPI_SPCR1	SCI_BASE + 0x1a
#define SPI_SPCR2	SCI_BASE + 0x1c
#define SPI_SPCR3	SCI_BASE + 0x1e
#define SPI_SPSR	SCI_BASE + 0x1f
#define SPI_QPDR	SCI_BASE + 0x15
#define SPI_QPAR	SCI_BASE + 0x16
#define SPI_QDDR	SCI_BASE + 0x17

#define CPURAMCTRL_MCR  CPURAMCTRL_BASE + 0x0
#define CPURAMCTRL_TST  CPURAMCTRL_BASE + 0x2
#define CPURAMCTRL_BAR  CPURAMCTRL_BASE + 0x4


