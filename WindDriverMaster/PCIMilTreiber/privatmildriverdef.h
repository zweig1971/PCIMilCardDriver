//-----------------------------
// GSI Gesellschaft für 
// Schwerionenforschung mbH, 
// Darmstadt, Germany 
//
// MilDriver
//
// Autor           : Zweig,Marcus
// Version         : 0.5
// letzte Änderung : 06.11.03
//------------------------------

// Grundlegende Routinen
//--------------------------
extern "C" MYDRIVER_HANDLE MYDRIVER_LocateAndOpenBoard (DWORD dwVendorID, DWORD dwDeviceID, BOOL fUseInt, int *status, int cards);
extern "C" void MYDRIVER_Close(MYDRIVER_HANDLE hMYDRIVER);
extern "C" DWORD MYDRIVER_CountCards (DWORD dwVendorID, DWORD dwDeviceID, int *status);

// Register Zugriffe
//----------------------

// MilBus lesen/schreiben Zugriffsart
extern "C" void  MYDRIVER_WriteMil_cmd( MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" void  MYDRIVER_WriteMil_write( MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" DWORD MYDRIVER_ReadMil_read( MYDRIVER_HANDLE hMYDRIVER);

// Timer 
extern "C" void MYDRIVER_WriteSet_Timer2_10us( MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" void MYDRIVER_WriteSet_Timer1( MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" void MYDRIVER_WriteSet_Timer2_1ms( MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" DWORD MYDRIVER_ReadEV_Time_Rd( MYDRIVER_HANDLE hMYDRIVER);

// Filter
extern "C" DWORD MYDRIVER_Read_C_Filter_ADR(MYDRIVER_HANDLE hMYDRIVER, DWORD OffsetValue);
extern "C" void  MYDRIVER_Write_C_Filter_ADR(MYDRIVER_HANDLE hMYDRIVER, DWORD data, DWORD OffsetValue);

// Interrupts
extern "C" DWORD MYDRIVER_ReadIntrrrupt_Mask( MYDRIVER_HANDLE hMYDRIVER);
extern "C" void MYDRIVER_WriteIntrrrupt_Mask( MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" BOOL MYDRIVER_IntEnable (MYDRIVER_HANDLE hMYDRIVER, MYDRIVER_INT_HANDLER funcIntHandler, void *pUserDat, WORD SET_MY_PRIORITY);
extern "C" void MYDRIVER_IntDisable (MYDRIVER_HANDLE hMYDRIVER);
//extern "C" void MYDRIVER_IntHandlerRoutine(DWORD MainAdress, DWORD InterruptID,CALLBACK_INT_HANDLER funcCallHandler);
extern "C" DWORD MYDRIVER_ReadINTCSR (MYDRIVER_HANDLE hMYDRIVER);
extern "C" void MYDRIVER_WriteINTCSR (MYDRIVER_HANDLE hMYDRIVER, DWORD data);

// Software generierte pulse
extern "C" void MYDRIVER_WritePuls1_Start_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" void MYDRIVER_WritePuls1_Stop_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" void MYDRIVER_WritePuls2_Start_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" void MYDRIVER_WritePuls2_Stop_Soft (MYDRIVER_HANDLE hMYDRIVER, DWORD data); 

// Sonstige
extern "C" DWORD MYDRIVER_ReadMil_Status( MYDRIVER_HANDLE hMYDRIVER);
extern "C" void  MYDRIVER_WriteReset_Card( MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" void  MYDRIVER_WriteControlRegister1( MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" DWORD MYDRIVER_ReadEvent_Read(MYDRIVER_HANDLE hMYDRIVER);

// Vorerst nicht von Bedeutung
extern "C" void MYDRIVER_WriteSlave_Mil_interlog( MYDRIVER_HANDLE hMYDRIVER, DWORD data);
extern "C" DWORD MYDRIVER_ReadAux_IO( MYDRIVER_HANDLE hMYDRIVER);
extern "C" void MYDRIVER_WriteAux_IO( MYDRIVER_HANDLE hMYDRIVER, DWORD data);
