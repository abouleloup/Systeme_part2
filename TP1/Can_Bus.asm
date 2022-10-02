
_main:

;Can_Bus.c,47 :: 		void main() {
;Can_Bus.c,48 :: 		ANSELA = 1;
	MOVLW       1
	MOVWF       ANSELA+0 
;Can_Bus.c,49 :: 		ANSELB = 0;
	CLRF        ANSELB+0 
;Can_Bus.c,50 :: 		ANSELD  = 0;                                                 // Configure AN pins as digital I/O
	CLRF        ANSELD+0 
;Can_Bus.c,51 :: 		ANSELC = 0;
	CLRF        ANSELC+0 
;Can_Bus.c,52 :: 		C1ON_bit = 0;                                               // Disable comparators
	BCF         C1ON_bit+0, BitPos(C1ON_bit+0) 
;Can_Bus.c,53 :: 		C2ON_bit = 0;
	BCF         C2ON_bit+0, BitPos(C2ON_bit+0) 
;Can_Bus.c,55 :: 		PORTB = 0;                                                  // clear PORTB
	CLRF        PORTB+0 
;Can_Bus.c,56 :: 		TRISB = 0;                                               // set PORTB as output
	CLRF        TRISB+0 
;Can_Bus.c,57 :: 		TRISD = 255;
	MOVLW       255
	MOVWF       TRISD+0 
;Can_Bus.c,59 :: 		Can_Init_Flags = 0;                                         //
	CLRF        _Can_Init_Flags+0 
;Can_Bus.c,60 :: 		Can_Send_Flags = 0;                                         // clear flags
	CLRF        _Can_Send_Flags+0 
;Can_Bus.c,61 :: 		Can_Rcv_Flags  = 0;                                         //
	CLRF        _Can_Rcv_Flags+0 
;Can_Bus.c,65 :: 		_CANSPI_TX_NO_RTR_FRAME;
	MOVLW       244
	MOVWF       _Can_Send_Flags+0 
;Can_Bus.c,72 :: 		_CANSPI_CONFIG_LINE_FILTER_OFF;
	MOVLW       209
	MOVWF       _Can_Init_Flags+0 
;Can_Bus.c,74 :: 		SPI1_Init();                                                              // initialize SPI1 module
	CALL        _SPI1_Init+0, 0
;Can_Bus.c,75 :: 		Lcd_Init();
	CALL        _Lcd_Init+0, 0
;Can_Bus.c,76 :: 		Lcd_Cmd(_LCD_CLEAR);
	MOVLW       1
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;Can_Bus.c,77 :: 		Lcd_Cmd(_LCD_CURSOR_OFF);
	MOVLW       12
	MOVWF       FARG_Lcd_Cmd_out_char+0 
	CALL        _Lcd_Cmd+0, 0
;Can_Bus.c,78 :: 		Lcd_Out(1,1,"ADC:");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr1_Can_Bus+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr1_Can_Bus+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Can_Bus.c,79 :: 		Lcd_Out(1,10,"CP:");
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       10
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr2_Can_Bus+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr2_Can_Bus+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Can_Bus.c,80 :: 		Lcd_Out(2,1,"ADC:");
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       1
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr3_Can_Bus+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr3_Can_Bus+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Can_Bus.c,81 :: 		Lcd_Out(2,10,"CP:");
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       10
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       ?lstr4_Can_Bus+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(?lstr4_Can_Bus+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Can_Bus.c,83 :: 		CANSPIInitialize(3,8,3,3,1,Can_Init_Flags);                                 // initialize external CANSPI module
	MOVLW       3
	MOVWF       FARG_CANSPIInitialize_SJW+0 
	MOVLW       8
	MOVWF       FARG_CANSPIInitialize_BRP+0 
	MOVLW       3
	MOVWF       FARG_CANSPIInitialize_PHSEG1+0 
	MOVLW       3
	MOVWF       FARG_CANSPIInitialize_PHSEG2+0 
	MOVLW       1
	MOVWF       FARG_CANSPIInitialize_PROPSEG+0 
	MOVF        _Can_Init_Flags+0, 0 
	MOVWF       FARG_CANSPIInitialize_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPIInitialize+0, 0
;Can_Bus.c,84 :: 		CANSPISetOperationMode(_CANSPI_MODE_CONFIG,0xFF);                           // set CONFIGURATION mode
	MOVLW       128
	MOVWF       FARG_CANSPISetOperationMode_mode+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetOperationMode_WAIT+0 
	CALL        _CANSPISetOperationMode+0, 0
;Can_Bus.c,85 :: 		CANSPISetMask(_CANSPI_MASK_B1,-1,_CANSPI_CONFIG_XTD_MSG);                   // set all mask1 bits to ones
	CLRF        FARG_CANSPISetMask_CANSPI_MASK+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+1 
	MOVWF       FARG_CANSPISetMask_val+2 
	MOVWF       FARG_CANSPISetMask_val+3 
	MOVLW       247
	MOVWF       FARG_CANSPISetMask_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPISetMask+0, 0
;Can_Bus.c,86 :: 		CANSPISetMask(_CANSPI_MASK_B2,-1,_CANSPI_CONFIG_XTD_MSG);                   // set all mask2 bits to ones
	MOVLW       1
	MOVWF       FARG_CANSPISetMask_CANSPI_MASK+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetMask_val+1 
	MOVWF       FARG_CANSPISetMask_val+2 
	MOVWF       FARG_CANSPISetMask_val+3 
	MOVLW       247
	MOVWF       FARG_CANSPISetMask_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPISetMask+0, 0
;Can_Bus.c,87 :: 		CANSPISetFilter(_CANSPI_FILTER_B2_F3,ID_1st,_CANSPI_CONFIG_XTD_MSG);        // set id of filter B2_F3 to 1st node ID
	MOVLW       4
	MOVWF       FARG_CANSPISetFilter_CANSPI_FILTER+0 
	MOVLW       79
	MOVWF       FARG_CANSPISetFilter_val+0 
	MOVLW       47
	MOVWF       FARG_CANSPISetFilter_val+1 
	MOVLW       0
	MOVWF       FARG_CANSPISetFilter_val+2 
	MOVLW       0
	MOVWF       FARG_CANSPISetFilter_val+3 
	MOVLW       247
	MOVWF       FARG_CANSPISetFilter_CANSPI_CONFIG_FLAGS+0 
	CALL        _CANSPISetFilter+0, 0
;Can_Bus.c,88 :: 		CANSPISetOperationMode(_CANSPI_MODE_NORMAL,0xFF);                           // set NORMAL mode
	CLRF        FARG_CANSPISetOperationMode_mode+0 
	MOVLW       255
	MOVWF       FARG_CANSPISetOperationMode_WAIT+0 
	CALL        _CANSPISetOperationMode+0, 0
;Can_Bus.c,90 :: 		cptValeur = 0;
	CLRF        _cptValeur+0 
;Can_Bus.c,91 :: 		cptValeurOld = cptValeur;
	CLRF        _cptValeurOld+0 
;Can_Bus.c,92 :: 		valeurAdc = 0;
	CLRF        _valeurAdc+0 
	CLRF        _valeurAdc+1 
;Can_Bus.c,93 :: 		valeurAdcOld = valeurAdc;
	CLRF        _valeurAdcOld+0 
	CLRF        _valeurAdcOld+1 
;Can_Bus.c,95 :: 		while (1) {                                                                   // endless loop
L_main0:
;Can_Bus.c,96 :: 		valeurAdc = ADC_Read(0);
	CLRF        FARG_ADC_Read_channel+0 
	CALL        _ADC_Read+0, 0
	MOVF        R0, 0 
	MOVWF       _valeurAdc+0 
	MOVF        R1, 0 
	MOVWF       _valeurAdc+1 
;Can_Bus.c,97 :: 		if (valeurAdc != valeurAdcOld) {
	MOVF        R1, 0 
	XORWF       _valeurAdcOld+1, 0 
	BTFSS       STATUS+0, 2 
	GOTO        L__main25
	MOVF        _valeurAdcOld+0, 0 
	XORWF       R0, 0 
L__main25:
	BTFSC       STATUS+0, 2 
	GOTO        L_main2
;Can_Bus.c,98 :: 		memset(bufferLCD, 0, sizeof(bufferLCD));
	MOVLW       _bufferLCD+0
	MOVWF       FARG_memset_p1+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_memset_p1+1 
	CLRF        FARG_memset_character+0 
	MOVLW       7
	MOVWF       FARG_memset_n+0 
	MOVLW       0
	MOVWF       FARG_memset_n+1 
	CALL        _memset+0, 0
;Can_Bus.c,99 :: 		sprinti(bufferLCD,"%04u",valeurAdc);
	MOVLW       _bufferLCD+0
	MOVWF       FARG_sprinti_wh+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_sprinti_wh+1 
	MOVLW       ?lstr_5_Can_Bus+0
	MOVWF       FARG_sprinti_f+0 
	MOVLW       hi_addr(?lstr_5_Can_Bus+0)
	MOVWF       FARG_sprinti_f+1 
	MOVLW       higher_addr(?lstr_5_Can_Bus+0)
	MOVWF       FARG_sprinti_f+2 
	MOVF        _valeurAdc+0, 0 
	MOVWF       FARG_sprinti_wh+5 
	MOVF        _valeurAdc+1, 0 
	MOVWF       FARG_sprinti_wh+6 
	CALL        _sprinti+0, 0
;Can_Bus.c,100 :: 		Lcd_Out(1,5,bufferLCD);
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       5
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _bufferLCD+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Can_Bus.c,101 :: 		Tx_Data[0] = valeurAdc >> 8;
	MOVF        _valeurAdc+1, 0 
	MOVWF       R0 
	MOVLW       0
	BTFSC       _valeurAdc+1, 7 
	MOVLW       255
	MOVWF       R1 
	MOVF        R0, 0 
	MOVWF       _Tx_Data+0 
;Can_Bus.c,102 :: 		Tx_Data[1] = valeurAdc;
	MOVF        _valeurAdc+0, 0 
	MOVWF       _Tx_Data+1 
;Can_Bus.c,103 :: 		CANSPIWrite(ID_2nd, Tx_Data, 3, Can_Send_Flags);
	MOVLW       3
	MOVWF       FARG_CANSPIWrite_id+0 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+1 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+2 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+3 
	MOVLW       _Tx_Data+0
	MOVWF       FARG_CANSPIWrite_wr_data+0 
	MOVLW       hi_addr(_Tx_Data+0)
	MOVWF       FARG_CANSPIWrite_wr_data+1 
	MOVLW       3
	MOVWF       FARG_CANSPIWrite_data_len+0 
	MOVF        _Can_Send_Flags+0, 0 
	MOVWF       FARG_CANSPIWrite_CANSPI_TX_MSG_FLAGS+0 
	CALL        _CANSPIWrite+0, 0
;Can_Bus.c,104 :: 		valeurAdcOld = valeurAdc;
	MOVF        _valeurAdc+0, 0 
	MOVWF       _valeurAdcOld+0 
	MOVF        _valeurAdc+1, 0 
	MOVWF       _valeurAdcOld+1 
;Can_Bus.c,105 :: 		}
L_main2:
;Can_Bus.c,107 :: 		Msg_Rcvd = CANSPIRead(&Rx_ID , Rx_Data , &Rx_Data_Len, &Can_Rcv_Flags); // receive message
	MOVLW       _Rx_ID+0
	MOVWF       FARG_CANSPIRead_id+0 
	MOVLW       hi_addr(_Rx_ID+0)
	MOVWF       FARG_CANSPIRead_id+1 
	MOVLW       _Rx_Data+0
	MOVWF       FARG_CANSPIRead_rd_data+0 
	MOVLW       hi_addr(_Rx_Data+0)
	MOVWF       FARG_CANSPIRead_rd_data+1 
	MOVLW       _Rx_Data_Len+0
	MOVWF       FARG_CANSPIRead_data_len+0 
	MOVLW       hi_addr(_Rx_Data_Len+0)
	MOVWF       FARG_CANSPIRead_data_len+1 
	MOVLW       _Can_Rcv_Flags+0
	MOVWF       FARG_CANSPIRead_CANSPI_RX_MSG_FLAGS+0 
	MOVLW       hi_addr(_Can_Rcv_Flags+0)
	MOVWF       FARG_CANSPIRead_CANSPI_RX_MSG_FLAGS+1 
	CALL        _CANSPIRead+0, 0
	MOVF        R0, 0 
	MOVWF       _Msg_Rcvd+0 
;Can_Bus.c,108 :: 		if ((Rx_ID == ID_1st) && Msg_Rcvd) {                                      // if message received check id
	MOVF        _Rx_ID+3, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L__main26
	MOVF        _Rx_ID+2, 0 
	XORLW       0
	BTFSS       STATUS+0, 2 
	GOTO        L__main26
	MOVF        _Rx_ID+1, 0 
	XORLW       47
	BTFSS       STATUS+0, 2 
	GOTO        L__main26
	MOVF        _Rx_ID+0, 0 
	XORLW       79
L__main26:
	BTFSS       STATUS+0, 2 
	GOTO        L_main5
	MOVF        _Msg_Rcvd+0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_main5
L__main23:
;Can_Bus.c,109 :: 		ValeurAdcRcv = (int)Rx_Data[0] << 8 | (int)Rx_Data[1];
	MOVF        _Rx_Data+0, 0 
	MOVWF       _ValeurAdcRcv+0 
	MOVLW       0
	MOVWF       _ValeurAdcRcv+1 
	MOVF        _ValeurAdcRcv+0, 0 
	MOVWF       _ValeurAdcRcv+1 
	CLRF        _ValeurAdcRcv+0 
	MOVF        _Rx_Data+1, 0 
	MOVWF       R0 
	MOVLW       0
	MOVWF       R1 
	MOVF        R0, 0 
	IORWF       _ValeurAdcRcv+0, 1 
	MOVF        R1, 0 
	IORWF       _ValeurAdcRcv+1, 1 
;Can_Bus.c,110 :: 		memset(bufferLCD, 0, sizeof(bufferLCD));
	MOVLW       _bufferLCD+0
	MOVWF       FARG_memset_p1+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_memset_p1+1 
	CLRF        FARG_memset_character+0 
	MOVLW       7
	MOVWF       FARG_memset_n+0 
	MOVLW       0
	MOVWF       FARG_memset_n+1 
	CALL        _memset+0, 0
;Can_Bus.c,111 :: 		sprinti(bufferLCD,"%04u",ValeurAdcRcv);
	MOVLW       _bufferLCD+0
	MOVWF       FARG_sprinti_wh+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_sprinti_wh+1 
	MOVLW       ?lstr_6_Can_Bus+0
	MOVWF       FARG_sprinti_f+0 
	MOVLW       hi_addr(?lstr_6_Can_Bus+0)
	MOVWF       FARG_sprinti_f+1 
	MOVLW       higher_addr(?lstr_6_Can_Bus+0)
	MOVWF       FARG_sprinti_f+2 
	MOVF        _ValeurAdcRcv+0, 0 
	MOVWF       FARG_sprinti_wh+5 
	MOVF        _ValeurAdcRcv+1, 0 
	MOVWF       FARG_sprinti_wh+6 
	CALL        _sprinti+0, 0
;Can_Bus.c,112 :: 		Lcd_Out(2,5,bufferLCD);
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       5
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _bufferLCD+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Can_Bus.c,113 :: 		memset(bufferLCD, 0, sizeof(bufferLCD));
	MOVLW       _bufferLCD+0
	MOVWF       FARG_memset_p1+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_memset_p1+1 
	CLRF        FARG_memset_character+0 
	MOVLW       7
	MOVWF       FARG_memset_n+0 
	MOVLW       0
	MOVWF       FARG_memset_n+1 
	CALL        _memset+0, 0
;Can_Bus.c,114 :: 		sprinti(bufferLCD,"%03u",(int)Rx_Data[2]);
	MOVLW       _bufferLCD+0
	MOVWF       FARG_sprinti_wh+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_sprinti_wh+1 
	MOVLW       ?lstr_7_Can_Bus+0
	MOVWF       FARG_sprinti_f+0 
	MOVLW       hi_addr(?lstr_7_Can_Bus+0)
	MOVWF       FARG_sprinti_f+1 
	MOVLW       higher_addr(?lstr_7_Can_Bus+0)
	MOVWF       FARG_sprinti_f+2 
	MOVF        _Rx_Data+2, 0 
	MOVWF       FARG_sprinti_wh+5 
	MOVLW       0
	MOVWF       FARG_sprinti_wh+6 
	CALL        _sprinti+0, 0
;Can_Bus.c,115 :: 		Lcd_Out(2,13,bufferLCD);
	MOVLW       2
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       13
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _bufferLCD+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Can_Bus.c,116 :: 		}
L_main5:
;Can_Bus.c,118 :: 		if (Button (&PORTD,0, 1,1)) {
	MOVLW       PORTD+0
	MOVWF       FARG_Button_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Button_port+1 
	CLRF        FARG_Button_pin+0 
	MOVLW       1
	MOVWF       FARG_Button_time_ms+0 
	MOVLW       1
	MOVWF       FARG_Button_active_state+0 
	CALL        _Button+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_main6
;Can_Bus.c,119 :: 		BP1 = 1;
	BSF         _BP1+0, BitPos(_BP1+0) 
;Can_Bus.c,120 :: 		}
L_main6:
;Can_Bus.c,121 :: 		if (BP1 && Button (&PORTD,0, 1,0)) {
	BTFSS       _BP1+0, BitPos(_BP1+0) 
	GOTO        L_main9
	MOVLW       PORTD+0
	MOVWF       FARG_Button_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Button_port+1 
	CLRF        FARG_Button_pin+0 
	MOVLW       1
	MOVWF       FARG_Button_time_ms+0 
	CLRF        FARG_Button_active_state+0 
	CALL        _Button+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_main9
L__main22:
;Can_Bus.c,122 :: 		cptValeur++;
	INCF        _cptValeur+0, 1 
;Can_Bus.c,123 :: 		BP1 = 0;
	BCF         _BP1+0, BitPos(_BP1+0) 
;Can_Bus.c,124 :: 		}
L_main9:
;Can_Bus.c,126 :: 		if (Button (&PORTD,1, 1,1)) {
	MOVLW       PORTD+0
	MOVWF       FARG_Button_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Button_port+1 
	MOVLW       1
	MOVWF       FARG_Button_pin+0 
	MOVLW       1
	MOVWF       FARG_Button_time_ms+0 
	MOVLW       1
	MOVWF       FARG_Button_active_state+0 
	CALL        _Button+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_main10
;Can_Bus.c,127 :: 		BP2 = 1;
	BSF         _BP2+0, BitPos(_BP2+0) 
;Can_Bus.c,128 :: 		}
L_main10:
;Can_Bus.c,129 :: 		if (BP2 && Button (&PORTD,1, 1,0)) {
	BTFSS       _BP2+0, BitPos(_BP2+0) 
	GOTO        L_main13
	MOVLW       PORTD+0
	MOVWF       FARG_Button_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Button_port+1 
	MOVLW       1
	MOVWF       FARG_Button_pin+0 
	MOVLW       1
	MOVWF       FARG_Button_time_ms+0 
	CLRF        FARG_Button_active_state+0 
	CALL        _Button+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_main13
L__main21:
;Can_Bus.c,130 :: 		cptValeur--;
	DECF        _cptValeur+0, 1 
;Can_Bus.c,131 :: 		BP2 = 0;
	BCF         _BP2+0, BitPos(_BP2+0) 
;Can_Bus.c,132 :: 		}
L_main13:
;Can_Bus.c,134 :: 		if (Button (&PORTD,2, 1,1)) {
	MOVLW       PORTD+0
	MOVWF       FARG_Button_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Button_port+1 
	MOVLW       2
	MOVWF       FARG_Button_pin+0 
	MOVLW       1
	MOVWF       FARG_Button_time_ms+0 
	MOVLW       1
	MOVWF       FARG_Button_active_state+0 
	CALL        _Button+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_main14
;Can_Bus.c,135 :: 		BP3 = 1;
	BSF         _BP3+0, BitPos(_BP3+0) 
;Can_Bus.c,136 :: 		}
L_main14:
;Can_Bus.c,137 :: 		if (BP3 && Button (&PORTD,2, 1,0)) {
	BTFSS       _BP3+0, BitPos(_BP3+0) 
	GOTO        L_main17
	MOVLW       PORTD+0
	MOVWF       FARG_Button_port+0 
	MOVLW       hi_addr(PORTD+0)
	MOVWF       FARG_Button_port+1 
	MOVLW       2
	MOVWF       FARG_Button_pin+0 
	MOVLW       1
	MOVWF       FARG_Button_time_ms+0 
	CLRF        FARG_Button_active_state+0 
	CALL        _Button+0, 0
	MOVF        R0, 1 
	BTFSC       STATUS+0, 2 
	GOTO        L_main17
L__main20:
;Can_Bus.c,138 :: 		cptValeur = 0;
	CLRF        _cptValeur+0 
;Can_Bus.c,139 :: 		BP3 = 0;
	BCF         _BP3+0, BitPos(_BP3+0) 
;Can_Bus.c,140 :: 		}
L_main17:
;Can_Bus.c,143 :: 		if (cptValeurOld != cptValeur) {
	MOVF        _cptValeurOld+0, 0 
	XORWF       _cptValeur+0, 0 
	BTFSC       STATUS+0, 2 
	GOTO        L_main18
;Can_Bus.c,144 :: 		memset(bufferLCD, 0, sizeof(bufferLCD));
	MOVLW       _bufferLCD+0
	MOVWF       FARG_memset_p1+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_memset_p1+1 
	CLRF        FARG_memset_character+0 
	MOVLW       7
	MOVWF       FARG_memset_n+0 
	MOVLW       0
	MOVWF       FARG_memset_n+1 
	CALL        _memset+0, 0
;Can_Bus.c,145 :: 		sprinti(bufferLCD,"%03u",(int)cptValeur);
	MOVLW       _bufferLCD+0
	MOVWF       FARG_sprinti_wh+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_sprinti_wh+1 
	MOVLW       ?lstr_8_Can_Bus+0
	MOVWF       FARG_sprinti_f+0 
	MOVLW       hi_addr(?lstr_8_Can_Bus+0)
	MOVWF       FARG_sprinti_f+1 
	MOVLW       higher_addr(?lstr_8_Can_Bus+0)
	MOVWF       FARG_sprinti_f+2 
	MOVF        _cptValeur+0, 0 
	MOVWF       FARG_sprinti_wh+5 
	MOVLW       0
	MOVWF       FARG_sprinti_wh+6 
	CALL        _sprinti+0, 0
;Can_Bus.c,146 :: 		Lcd_Out(1,13,bufferLCD);
	MOVLW       1
	MOVWF       FARG_Lcd_Out_row+0 
	MOVLW       13
	MOVWF       FARG_Lcd_Out_column+0 
	MOVLW       _bufferLCD+0
	MOVWF       FARG_Lcd_Out_text+0 
	MOVLW       hi_addr(_bufferLCD+0)
	MOVWF       FARG_Lcd_Out_text+1 
	CALL        _Lcd_Out+0, 0
;Can_Bus.c,147 :: 		Tx_Data[2] = cptValeur;
	MOVF        _cptValeur+0, 0 
	MOVWF       _Tx_Data+2 
;Can_Bus.c,148 :: 		CANSPIWrite(ID_2nd, Tx_Data, 3, Can_Send_Flags);
	MOVLW       3
	MOVWF       FARG_CANSPIWrite_id+0 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+1 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+2 
	MOVLW       0
	MOVWF       FARG_CANSPIWrite_id+3 
	MOVLW       _Tx_Data+0
	MOVWF       FARG_CANSPIWrite_wr_data+0 
	MOVLW       hi_addr(_Tx_Data+0)
	MOVWF       FARG_CANSPIWrite_wr_data+1 
	MOVLW       3
	MOVWF       FARG_CANSPIWrite_data_len+0 
	MOVF        _Can_Send_Flags+0, 0 
	MOVWF       FARG_CANSPIWrite_CANSPI_TX_MSG_FLAGS+0 
	CALL        _CANSPIWrite+0, 0
;Can_Bus.c,149 :: 		cptValeurOld = cptValeur;
	MOVF        _cptValeur+0, 0 
	MOVWF       _cptValeurOld+0 
;Can_Bus.c,150 :: 		}
L_main18:
;Can_Bus.c,151 :: 		Delay_ms(100);
	MOVLW       2
	MOVWF       R11, 0
	MOVLW       4
	MOVWF       R12, 0
	MOVLW       186
	MOVWF       R13, 0
L_main19:
	DECFSZ      R13, 1, 1
	BRA         L_main19
	DECFSZ      R12, 1, 1
	BRA         L_main19
	DECFSZ      R11, 1, 1
	BRA         L_main19
	NOP
;Can_Bus.c,152 :: 		}
	GOTO        L_main0
;Can_Bus.c,153 :: 		}
L_end_main:
	GOTO        $+0
; end of _main
