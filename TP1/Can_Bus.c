unsigned char Can_Init_Flags, Can_Send_Flags, Can_Rcv_Flags; // can flags
unsigned char Rx_Data_Len;                                   // received data length in bytes
char Rx_Data[8];                                           // can rx data buffer
char Tx_Data[8];                                           // can tx data buffer
char Msg_Rcvd;                                               // reception flag
const long ID_1st = 12111, ID_2nd = 3;                       // node IDs
long Rx_ID;

// CANSPI module connections
sbit CanSpi_CS            at  RC0_bit;
sbit CanSpi_CS_Direction  at  TRISC0_bit;
sbit CanSpi_Rst           at  RC2_bit;
sbit CanSpi_Rst_Direction at  TRISC2_bit;
// End CANSPI module connections

// Lcd pinout settings
sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D4 at RB0_bit;

// Pin direction
sbit LCD_RS_Direction at TRISB4_bit;
sbit LCD_EN_Direction at TRISB5_bit;
sbit LCD_D7_Direction at TRISB3_bit;
sbit LCD_D6_Direction at TRISB2_bit;
sbit LCD_D5_Direction at TRISB1_bit;
sbit LCD_D4_Direction at TRISB0_bit;

int Nombre = 0;
float volt = 0;
int pot = 0;
int valeurAdc, valeurAdcOld, ValeurAdcRcv;
unsigned short cptValeur, cptValeurOld;
char bufferLCD[7];
bit BP1;
bit BP2;
bit BP3;

void main() {
  ANSELA = 1;
  ANSELB = 0;
  ANSELD  = 0;                                                 // Configure AN pins as digital I/O
  ANSELC = 0;
  C1ON_bit = 0;                                               // Disable comparators
  C2ON_bit = 0;

  PORTB = 0;                                                  // clear PORTB
  TRISB = 0;                                               // set PORTB as output
  TRISD = 255;
  
  Can_Init_Flags = 0;                                         //
  Can_Send_Flags = 0;                                         // clear flags
  Can_Rcv_Flags  = 0;                                         //

  Can_Send_Flags = _CANSPI_TX_PRIORITY_0 &                    // form value to be used
                   _CANSPI_TX_XTD_FRAME &                     //   with CANSPIWrite
                   _CANSPI_TX_NO_RTR_FRAME;

  Can_Init_Flags = _CANSPI_CONFIG_SAMPLE_THRICE &             // Form value to be used
                   _CANSPI_CONFIG_PHSEG2_PRG_ON &             //  with CANSPIInit
                   _CANSPI_CONFIG_XTD_MSG &
                   _CANSPI_CONFIG_DBL_BUFFER_ON &
                   _CANSPI_CONFIG_VALID_XTD_MSG &
                   _CANSPI_CONFIG_LINE_FILTER_OFF;

  SPI1_Init();                                                              // initialize SPI1 module
  Lcd_Init();
  Lcd_Cmd(_LCD_CLEAR);
  Lcd_Cmd(_LCD_CURSOR_OFF);
  Lcd_Out(1,1,"ADC:");
  Lcd_Out(1,10,"CP:");
  Lcd_Out(2,1,"ADC:");
  Lcd_Out(2,10,"CP:");
  
  CANSPIInitialize(3,8,3,3,1,Can_Init_Flags);                                 // initialize external CANSPI module
  CANSPISetOperationMode(_CANSPI_MODE_CONFIG,0xFF);                           // set CONFIGURATION mode
  CANSPISetMask(_CANSPI_MASK_B1,-1,_CANSPI_CONFIG_XTD_MSG);                   // set all mask1 bits to ones
  CANSPISetMask(_CANSPI_MASK_B2,-1,_CANSPI_CONFIG_XTD_MSG);                   // set all mask2 bits to ones
  CANSPISetFilter(_CANSPI_FILTER_B2_F3,ID_1st,_CANSPI_CONFIG_XTD_MSG);        // set id of filter B2_F3 to 1st node ID
  CANSPISetOperationMode(_CANSPI_MODE_NORMAL,0xFF);                           // set NORMAL mode

  cptValeur = 0;
  cptValeurOld = cptValeur;
  valeurAdc = 0;
  valeurAdcOld = valeurAdc;
  
  while (1) {                                                                   // endless loop
    valeurAdc = ADC_Read(0);
    if (valeurAdc != valeurAdcOld) {
      memset(bufferLCD, 0, sizeof(bufferLCD));
      sprinti(bufferLCD,"%04u",valeurAdc);
      Lcd_Out(1,5,bufferLCD);
      Tx_Data[0] = valeurAdc >> 8;
      Tx_Data[1] = valeurAdc;
      CANSPIWrite(ID_2nd, Tx_Data, 3, Can_Send_Flags);
      valeurAdcOld = valeurAdc;
    }

   Msg_Rcvd = CANSPIRead(&Rx_ID , Rx_Data , &Rx_Data_Len, &Can_Rcv_Flags); // receive message
    if ((Rx_ID == ID_1st) && Msg_Rcvd) {                                      // if message received check id
      ValeurAdcRcv = (int)Rx_Data[0] << 8 | (int)Rx_Data[1];
      memset(bufferLCD, 0, sizeof(bufferLCD));
      sprinti(bufferLCD,"%04u",ValeurAdcRcv);
      Lcd_Out(2,5,bufferLCD);
      memset(bufferLCD, 0, sizeof(bufferLCD));
      sprinti(bufferLCD,"%03u",(int)Rx_Data[2]);
      Lcd_Out(2,13,bufferLCD);
    }

    if (Button (&PORTD,0, 1,1)) {
       BP1 = 1;
    }
    if (BP1 && Button (&PORTD,0, 1,0)) {
       cptValeur++;
       BP1 = 0;
    }
    
    if (Button (&PORTD,1, 1,1)) {
       BP2 = 1;
    }
    if (BP2 && Button (&PORTD,1, 1,0)) {
       cptValeur--;
       BP2 = 0;
    }

    if (Button (&PORTD,2, 1,1)) {
       BP3 = 1;
    }
    if (BP3 && Button (&PORTD,2, 1,0)) {
       cptValeur = 0;
       BP3 = 0;
    }
    
    // Envoie du compteur selement si changement de valeur!
    if (cptValeurOld != cptValeur) {
      memset(bufferLCD, 0, sizeof(bufferLCD));
      sprinti(bufferLCD,"%03u",(int)cptValeur);
      Lcd_Out(1,13,bufferLCD);
      Tx_Data[2] = cptValeur;
      CANSPIWrite(ID_2nd, Tx_Data, 3, Can_Send_Flags);
      cptValeurOld = cptValeur;
    }
    Delay_ms(100);
  }
}