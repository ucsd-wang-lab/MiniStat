#include "main.h"
#include "bleHandler.h"

void appCallBack(uint32 event, void* eventParam);
void writeDeviceInfoToGATT(void);
void updateBleAdvert(void);
void updateBleData(uint32_t handle, uint8_t* addr, uint16_t size);
void bleStartAdvertising(void);

CYBLE_API_RESULT_T apiResult;
uint8_t bleHibernateFlag = FALSE; 
uint8_t mcuHibernateFlag = FALSE;

uint8 deviceConnected = FALSE;

CYBLE_GAP_CONN_UPDATE_PARAM_T connectionParameters = 
{
    600,                // Minimum connection interval - 400 x 1.25 = 500 ms 
    700,                // Maximum connection interval - 400 x 1.25 = 500 ms 
    0,                  // Slave latency - 1 
    500                 // Supervision timeout - 500 x 10 = 5000 ms 
};

void writeDeviceInfoToGATT(void)
{
    //Get device info from header files and write to device info characteristic
    #define VERSION_LENGTH  3   //major, minor, point
    #define DEVICE_INFO_LENGTH   (VERSION_LENGTH)
    
    uint8 devInfo[DEVICE_INFO_LENGTH] = {0};    
    devInfo[0] = FV_MAJOR;
    devInfo[1] = FV_MINOR;
    devInfo[2] = FV_POINT;
    
    updateBleData(CYBLE_DEVICE_INFO_FW_VERSION_CHAR_HANDLE, (uint8_t*)&devInfo[0], DEVICE_INFO_LENGTH);
    //DBG_PRINTF("Device info updated.\r\n");
}

void updateBleAdvert(void)
{
    char locName[BLE_ADVERT_LENGTH + 1] = { 0 }; // +1 to ensure null character is maintained
    uint32_t i = 0;
    uint32 UniqueData[2] = { 0, 0 };  
// Update advertisement with unique chip identifier
    
#define scanPayload      (cyBle_discoveryModeInfo.scanRspData->scanRspData)
#define U4toASC(x)      (((x >> 4) < 10) ? ((x >> 4) + 0x30) : ((x >> 4) + 0x37))
#define L4toASC(x)      (((x & 0xF) < 10) ? ((x & 0xF) + 0x30) : ((x & 0xF) + 0x37))
    
    CyGetUniqueId(&UniqueData[0]);
    
    strncpy(&locName[0], PROJECT_NAME, NAME_LENGTH);
    
    locName[NAME_LENGTH] = ' ';
    locName[NAME_LENGTH+1] = U4toASC((uint8)((UniqueData[0] & 0xFF000000) >> 24));
    locName[NAME_LENGTH+2] = L4toASC((uint8)((UniqueData[0] & 0xFF000000) >> 24));
    locName[NAME_LENGTH+3] = U4toASC((uint8)((UniqueData[0] & 0xFF0000) >> 16));
    locName[NAME_LENGTH+4] = L4toASC((uint8)((UniqueData[0] & 0xFF0000) >> 16));
    locName[NAME_LENGTH+5] = U4toASC((uint8)((UniqueData[0] & 0xFF00) >> 8));
    locName[NAME_LENGTH+6] = L4toASC((uint8)((UniqueData[0] & 0xFF00) >> 8));
    locName[NAME_LENGTH+7] = U4toASC((uint8)((UniqueData[0] & 0xFF)));
    locName[NAME_LENGTH+8] = L4toASC((uint8)((UniqueData[0] & 0xFF)));
    locName[NAME_LENGTH+9] = U4toASC((uint8)((UniqueData[1] & 0xFF000000) >> 24));
    locName[NAME_LENGTH+10] = L4toASC((uint8)((UniqueData[1] & 0xFF000000) >> 24));
    locName[NAME_LENGTH+11] = U4toASC((uint8)((UniqueData[1] & 0xFF0000) >> 16));
    locName[NAME_LENGTH+12] = L4toASC((uint8)((UniqueData[1] & 0xFF0000) >> 16));
    locName[NAME_LENGTH+13] = U4toASC((uint8)((UniqueData[1] & 0xFF00) >> 8));
    locName[NAME_LENGTH+14] = L4toASC((uint8)((UniqueData[1] & 0xFF00) >> 8));
    locName[NAME_LENGTH+15] = U4toASC((uint8)((UniqueData[1] & 0xFF)));
    locName[NAME_LENGTH+16] = L4toASC((uint8)((UniqueData[1] & 0xFF)));
    
    // zero-out remainder of buffer to ensure the prior buffer use doesn't affect the advertisement
    for(i = NAME_LENGTH+17; i < BLE_ADVERT_LENGTH; i++)
    {
        locName[i] = '\0';
    }
    CyBle_GapSetLocalName(locName);
    DBG_PRINTF("BLE Advert Updated: %s\r\n", locName);
}

/*Function to save parameter to GATT server*/
void updateBleData( uint32_t handle, uint8_t* addr, uint16_t size )
{
    CYBLE_GATT_HANDLE_VALUE_PAIR_T locHandleValuePair;
    locHandleValuePair.attrHandle = handle;
    locHandleValuePair.value.len = size;
    locHandleValuePair.value.val = addr;
    CyBle_GattsWriteAttributeValue( &locHandleValuePair, 0, NULL, CYBLE_GATT_DB_LOCALLY_INITIATED );
}

void bleManagePower(void)
{
    CyBle_ProcessEvents();
    
    if (bleHibernateFlag == TRUE)
    {
        
        CYBLE_BLESS_STATE_T blessState = CyBle_GetBleSsState();
        if (blessState != CYBLE_BLESS_STATE_HIBERNATE)
        {
            CyBle_EnterLPM(CYBLE_BLESS_HIBERNATE);
            mcuHibernateFlag = FALSE;
        }
        else 
        {
            mcuHibernateFlag = TRUE;
        }
    }
    else 
    {
        if (CyBle_GetBleSsState() != CYBLE_BLESS_STATE_SLEEP)
        {
            CyBle_EnterLPM(CYBLE_BLESS_SLEEP);
            mcuHibernateFlag = FALSE;
        }
    }
}

uint8_t getMcuHibernateFlag(void)
{
    return mcuHibernateFlag;
}

void bleHibernate(void)
{
    DBG_PRINTF("BLE Hibernating.\r\n");
    CyBle_GappStopAdvertisement();
    CyBle_GapDisconnect(cyBle_connHandle.bdHandle);
    bleHibernateFlag = TRUE;
}

void bleWake(void)
{
    bleHibernateFlag = FALSE;
    CYBLE_LP_MODE_T result = CyBle_ExitLPM();
    if (result != CYBLE_BLESS_ACTIVE)
    {
        DBG_PRINTF("BLE inactive! Starting...\r\n");
        bleStart();//BLE start also starts advertising
    }
    else
    {
        bleStartAdvertising();
    }
}

CYBLE_API_RESULT_T bleStart(void)
{
    // start CYBLE component and register generic event handler
    apiResult = CyBle_Start(appCallBack);
    /* Wait for BLE Component to Initialize */
    while (CyBle_GetState() == CYBLE_STATE_INITIALIZING)
    {
        CyBle_ProcessEvents(); 
    }
    //CyBle_BasRegisterAttrCallback(BasCallBack);
    if(apiResult != CYBLE_ERROR_OK)
    {
        DBG_PRINTF("CyBle_Start API Error: ");
    }
    else
    {
        bleStartAdvertising();
        DBG_PRINTF("Started BLE.\r\n");
    }
    return apiResult;
}

void bleStartAdvertising(void)
{
    apiResult = CyBle_GappStartAdvertisement(CYBLE_ADVERTISING_FAST);
    if(apiResult != CYBLE_ERROR_OK)
    {
        DBG_PRINTF("StartAdvertisement API Error: ");
    }
    else 
    {
        DBG_PRINTF("BLE advert started. \r\n");
        writeDeviceInfoToGATT();
    } 
}

/*******************************************************************************
* Function Name: AppCallBack
********************************************************************************
* Summary:
*        General callback event handler function for BLE component through which
* application recieves various BLE events.
*
* Parameters:
*  event:		event type. See "CYBLE_EVENT_T" structure in BLE_Stack.h
*  eventParam: 	parameter associated with the event
*
* Return:
*  void
*
*******************************************************************************/
void appCallBack(uint32 event, void* eventParam)
{
    /* Structure to store data written by Client */	
	CYBLE_GATTS_WRITE_REQ_PARAM_T *wrReqParam;
    CYBLE_BLESS_CLK_CFG_PARAMS_T clockConfig;

    switch (event)
	{
        /**********************************************************
        *                       General Events
        ***********************************************************/
		case CYBLE_EVT_STACK_ON: /* This event received when component is Started */
            /* C8. Get the configured clock parameters for BLE subsystem */
            CyBle_GetBleClockCfgParam(&clockConfig);
            /* C8. Set the device sleep-clock accuracy (SCA) based on the tuned ppm
            of the WCO */
            clockConfig.bleLlSca = CYBLE_LL_SCA_031_TO_050_PPM;
            /* C8. Set the clock parameter of BLESS with updated values */
            CyBle_SetBleClockCfgParam(&clockConfig);
            updateBleAdvert();
            /* Enter in to discoverable mode so that remote can search it. */
            DBG_PRINTF("CYBLE_EVT_STACK_ON: %x \r\n", *(uint8 *)eventParam);
            break;
        case CYBLE_EVT_HARDWARE_ERROR:    // This event indicates that some internal HW error has occurred. 
            DBG_PRINTF("CYBLE_EVT_HARDWARE_ERROR \r\n");
			break;
        /**********************************************************
        *                       GAP Events
        ***********************************************************/
        case CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP:
            updateBleAdvert();
            DBG_PRINTF("CYBLE_EVT_GAPP_ADVERTISEMENT_START_STOP: %x \r\n", *(uint8 *)eventParam);
            break;
        case CYBLE_EVT_GAP_DEVICE_CONNECTED:
            DBG_PRINTF("CYBLE_EVT_GAP_DEVICE_CONNECTED: %x \r\n", *(uint8 *)eventParam);
            break;
        case CYBLE_EVT_GAP_DEVICE_DISCONNECTED:
            DBG_PRINTF("CYBLE_EVT_GAP_DEVICE_DISCONNECTED \r\n");
            break;
            
        /**********************************************************
        *                       GATT Events
        ***********************************************************/
        case CYBLE_EVT_GATT_CONNECT_IND:
            deviceConnected = TRUE;
            DBG_PRINTF("EVT_GATT_CONNECT_IND: %x, %x \r\n", cyBle_connHandle.attId, cyBle_connHandle.bdHandle);
            break;
        case CYBLE_EVT_GATT_DISCONNECT_IND:
            deviceConnected = FALSE;
            DBG_PRINTF("EVT_GATT_DISCONNECT_IND \r\n");
            break;
        case CYBLE_EVT_GATTS_WRITE_REQ:
            DBG_PRINTF("EVT_GATT_WRITE_REQ \r\n");
            wrReqParam = (CYBLE_GATTS_WRITE_REQ_PARAM_T *) eventParam;
            switch (wrReqParam->handleValPair.attrHandle)
            {
                default:
                    //Add a case to handle each write characteristic 
                break;
            }
            break;
        case CYBLE_EVT_GATTS_XCNHG_MTU_REQ:
            //DBG_PRINTF("CYBLE_EVT_GATTS_XCNHG_MTU_REQ \r\n");
            break;            
        case CYBLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ:
            //DBG_PRINTF("CYBLE_EVT_GATTS_READ_CHAR_VAL_ACCESS_REQ \r\n");
            //occurs when GATT value is updated
            break;            
        case CYBLE_EVT_GATTS_INDICATION_ENABLED:
            DBG_PRINTF("CYBLE_EVT_GATTS_INDICATION_ENABLED \r\n");
            //set the connection interval to a more reasonable value
            //CyBle_L2capLeConnectionParamUpdateRequest(cyBle_connHandle.bdHandle, &connectionParameters);
            break;
		default:
            DBG_PRINTF("\tOTHER BLE: 0x%x%x \r\n", ((event >> 8) & 0xFF), (event & 0xFF));
			break;
	}
}

uint8 getBleHibernateFlag(void)
{
    return bleHibernateFlag;
}

void setBleHibernateFlag(uint8 flag)
{
    bleHibernateFlag = flag;
}

CYBLE_API_RESULT_T getApiResult(void)
{
    
    return apiResult;
}

void setApiResult(CYBLE_API_RESULT_T api)
{
    apiResult = api;
    return;
}

uint8 getBleConnection(void)
{
    return deviceConnected;
}

/* [] END OF FILE */

