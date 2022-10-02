#line 1 "C:/Users/mhdab/OneDrive/Bureau/systm2/Can_Bus.c"
unsigned char Can_Init_Flags, Can_Send_Flags, Can_Rcv_Flags;
unsigned char Rx_Data_Len;
char Rx_Data[8];
char Tx_Data[8];
char Msg_Rcvd;
const long ID_1st = 12111, ID_2nd = 3;
long Rx_ID;


sbit CanSpi_CS at RC0_bit;
sbit CanSpi_CS_Direction at TRISC0_bit;
sbit CanSpi_Rst at RC2_bit;
sbit CanSpi_Rst_Direction at TRISC2_bit;



sbit LCD_RS at RB4_bit;
sbit LCD_EN at RB5_bit;
sbit LCD_D7 at RB3_bit;
sbit LCD_D6 at RB2_bit;
sbit LCD_D5 at RB1_bit;
sbit LCD_D4 at RB0_bit;


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
 ANSELD = 0;
 ANSELC = 0;
 C1ON_bit = 0;
 C2ON_bit = 0;

 PORTB = 0;
 TRISB = 0;
 TRISD = 255;

 Can_Init_Flags = 0;
 Can_Send_Flags = 0;
 Can_Rcv_Flags = 0;

 Can_Send_Flags = _CANSPI_TX_PRIORITY_0 &
 _CANSPI_TX_XTD_FRAME &
 _CANSPI_TX_NO_RTR_FRAME;

 Can_Init_Flags = _CANSPI_CONFIG_SAMPLE_THRICE &
 _CANSPI_CONFIG_PHSEG2_PRG_ON &
 _CANSPI_CONFIG_XTD_MSG &
 _CANSPI_CONFIG_DBL_BUFFER_ON &
 _CANSPI_CONFIG_VALID_XTD_MSG &
 _CANSPI_CONFIG_LINE_FILTER_OFF;

 SPI1_Init();
 Lcd_Init();
 Lcd_Cmd(_LCD_CLEAR);
 Lcd_Cmd(_LCD_CURSOR_OFF);
 Lcd_Out(1,1,"ADC:");
 Lcd_Out(1,10,"CP:");
 Lcd_Out(2,1,"ADC:");
 Lcd_Out(2,10,"CP:");

 CANSPIInitialize(3,8,3,3,1,Can_Init_Flags);
 CANSPISetOperationMode(_CANSPI_MODE_CONFIG,0xFF);
 CANSPISetMask(_CANSPI_MASK_B1,-1,_CANSPI_CONFIG_XTD_MSG);
 CANSPISetMask(_CANSPI_MASK_B2,-1,_CANSPI_CONFIG_XTD_MSG);
 CANSPISetFilter(_CANSPI_FILTER_B2_F3,ID_1st,_CANSPI_CONFIG_XTD_MSG);
 CANSPISetOperationMode(_CANSPI_MODE_NORMAL,0xFF);

 cptValeur = 0;
 cptValeurOld = cptValeur;
 valeurAdc = 0;
 valeurAdcOld = valeurAdc;

 while (1) {
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

 Msg_Rcvd = CANSPIRead(&Rx_ID , Rx_Data , &Rx_Data_Len, &Can_Rcv_Flags);
 if ((Rx_ID == ID_1st) && Msg_Rcvd) {
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
