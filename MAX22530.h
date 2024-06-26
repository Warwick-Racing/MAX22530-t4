#ifndef _IS32FL3236A_h
#define _IS32FL3236A_h

#include <Arduino.h>
#include <SPI.h>

/*MAX22530 Registers*/
#define PROD_ID 0x00
#define ADC1_REG 0x01
#define ADC2_REG 0x02
#define ADC3_REG 0x03
#define ADC4_REG 0x04
#define FADC1 0x05
#define FADC2 0x06
#define FADC3 0x07
#define FADC4 0x08
#define COUTHI1 0x09
#define COUTHI2 0x0a
#define COUTHI3 0x0b
#define COUTHI4 0x0c
#define COUTLO1 0x0d
#define COUTLO2 0x0e
#define COUTLO3 0x0f
#define COUTLO4 0x10
#define COUT_STATUS 0x11
#define INTERRUPT_STATUS 0x12
#define INTERRUPT_ENABLE 0x13
#define CONTROL 0x14

#define MAX22530_ID 0x81
#define VREF 1.80

class MAX22530
{
public:
    explicit MAX22530(uint8_t cspin, SPIClass *theSPI = &SPI);                                 /* Starts ADC with CS PIN,SPI class,Interupt pin */
    bool begin(int frequency);                                                                 /*Opens connection with specified frequency (max 10 MHz) */
    int readADC(int channel);                                                                  /* Reads ADC, channels 1-4 */
    int readFiltered(int channel);                                                             /* Reads filtered value for more info see datashett, channels 1-4*/
    float CNVTVoltage(int count);                                                              /* Convertes count to voltage*/
    void setComp(int channel, uint16_t up_value, uint16_t down_value, bool source, bool mode); /* Sets the comparator for givin channel (1-4), with up and down value, source (0 -> unfiltered 1-> filtered), mode (0 -> digital status mode 1-> digital input mode)*/
    uint16_t readCompReg(int channel, bool highlow);                                           /* Reads the selected channel register (1-4) highlow selects if COUTHI (1) or COUTLO (0) is read */
    uint16_t readCompStat(void);                                                               /* Reads the COUT STATUS register */
    uint16_t readInterrupt(void);                                                              /* Reads the I´NTERUPT STAUS register !READING CLEARS ALL INTERUPT FLAGS! */
    void setInterruptEnable(uint16_t data);                                                    /* Sets the INTERUPT ENABLE REGISTER after this operation interupts will be issued over the #INT pin! */
    void SPI_CRC(bool enable);                                                                 /* ENABLES/DISABELS CRC*/
    void COMTThreshold(bool enable);                                                           /* ENABLE COMMON THRESHOLD only COUTH1 and COUTLO1 are used for all comparators*/
    void CLEARFilter(int channel);                                                             /* Clears the specific filter (1-4) */
    void VDDF(bool enable);                                                                    /* ON/OFF of VDFF (1 VDFF off)*/
    void SoftwareReset(void);                                                                  /* SOFTWARE RESET*/
    void HardwareReset(void);                                                                  /* HARDWARE RESET*/
    void clearPOR(void);                                                                       /* CLEARS THE POR BIT in 0x00*/

private:
    long MAX22530_read_register(uint8_t regAddress);                             /* Reads the value of the selected register. */
    void MAX22530_write_register(uint8_t regAddress, uint16_t regValue);         /* Writes a value to the register. */
    void MAX22530_Register_bit_toggle(uint8_t regAddress, uint8_t bit_position); /* Toggles a bit based on bit position to the given register. */
    uint8_t MAX22530_Init(void);                                                 /* Initialize MAX22530 and check if the device is present*/
    void MAX22530_Reset(void);                                                   /* Resets the device*/
    void MAX22530_softReset(void);                                               /* Softresets the device*/
    void MAX22530_EN_CRC(void);
    void MAX22530_DIS_CRC(void);
    uint8_t crc_compute_2(uint32_t frame);
    uint8_t getCRC(uint8_t[], unsigned int);
    bool MAX22530_INIT(void);

    SPIClass *_SPI;
    uint8_t _cs;
    uint16_t _frequency;

    bool crc_enable = false;
    const unsigned char CRCTable[256] = {
        0x00, 0x07, 0x0e, 0x09, 0x1c, 0x1b, 0x12, 0x15, 0x38, 0x3f, 0x36, 0x31, 0x24, 0x23, 0x2a, 0x2d,
        0x70, 0x77, 0x7e, 0x79, 0x6c, 0x6b, 0x62, 0x65, 0x48, 0x4f, 0x46, 0x41, 0x54, 0x53, 0x5a, 0x5d,
        0xe0, 0xe7, 0xee, 0xe9, 0xfc, 0xfb, 0xf2, 0xf5, 0xd8, 0xdf, 0xd6, 0xd1, 0xc4, 0xc3, 0xca, 0xcd,
        0x90, 0x97, 0x9e, 0x99, 0x8c, 0x8b, 0x82, 0x85, 0xa8, 0xaf, 0xa6, 0xa1, 0xb4, 0xb3, 0xba, 0xbd,
        0xc7, 0xc0, 0xc9, 0xce, 0xdb, 0xdc, 0xd5, 0xd2, 0xff, 0xf8, 0xf1, 0xf6, 0xe3, 0xe4, 0xed, 0xea,
        0xb7, 0xb0, 0xb9, 0xbe, 0xab, 0xac, 0xa5, 0xa2, 0x8f, 0x88, 0x81, 0x86, 0x93, 0x94, 0x9d, 0x9a,
        0x27, 0x20, 0x29, 0x2e, 0x3b, 0x3c, 0x35, 0x32, 0x1f, 0x18, 0x11, 0x16, 0x03, 0x04, 0x0d, 0x0a,
        0x57, 0x50, 0x59, 0x5e, 0x4b, 0x4c, 0x45, 0x42, 0x6f, 0x68, 0x61, 0x66, 0x73, 0x74, 0x7d, 0x7a,
        0x89, 0x8e, 0x87, 0x80, 0x95, 0x92, 0x9b, 0x9c, 0xb1, 0xb6, 0xbf, 0xb8, 0xad, 0xaa, 0xa3, 0xa4,
        0xf9, 0xfe, 0xf7, 0xf0, 0xe5, 0xe2, 0xeb, 0xec, 0xc1, 0xc6, 0xcf, 0xc8, 0xdd, 0xda, 0xd3, 0xd4,
        0x69, 0x6e, 0x67, 0x60, 0x75, 0x72, 0x7b, 0x7c, 0x51, 0x56, 0x5f, 0x58, 0x4d, 0x4a, 0x43, 0x44,
        0x19, 0x1e, 0x17, 0x10, 0x05, 0x02, 0x0b, 0x0c, 0x21, 0x26, 0x2f, 0x28, 0x3d, 0x3a, 0x33, 0x34,
        0x4e, 0x49, 0x40, 0x47, 0x52, 0x55, 0x5c, 0x5b, 0x76, 0x71, 0x78, 0x7f, 0x6a, 0x6d, 0x64, 0x63,
        0x3e, 0x39, 0x30, 0x37, 0x22, 0x25, 0x2c, 0x2b, 0x06, 0x01, 0x08, 0x0f, 0x1a, 0x1d, 0x14, 0x13,
        0xae, 0xa9, 0xa0, 0xa7, 0xb2, 0xb5, 0xbc, 0xbb, 0x96, 0x91, 0x98, 0x9f, 0x8a, 0x8d, 0x84, 0x83,
        0xde, 0xd9, 0xd0, 0xd7, 0xc2, 0xc5, 0xcc, 0xcb, 0xe6, 0xe1, 0xe8, 0xef, 0xfa, 0xfd, 0xf4, 0xf3};
};

#endif
