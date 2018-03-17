#include "dropdowndemo.h"
#include "DIALOG.h"
#include "led.h"
#include "beep.h"
#include "usart.h"
#include "buf_switch.h"
#include "stmflash.h"

u32 Implementation_Password=0;//�û����������

u8 Passward_Buf[6]={1,2,3,4,5,3};  //��Ҫд�����������(��λ)
#define Passward_Size sizeof(Passward_Buf)   //����ĳ��ȣ���flash��д��ʹ��
#define Flash_Save_Passward_Addr 0x08070000  //��0x08000000----0x08070000�д������

u32 Test[6]={0};  //���ڱȽ�����ʹ��
u32 Test_Passward=123456;//��ʼ����
u8 Bit=0;//�ж��ǵڼ�λ
u8 six;//�������λ
u8 five;//�������λ
u8 four;//�������λ
u8 three;//�������λ
u8 two;//����ڶ�λ
u8 one;//�����һλ
u8 Passward_i=0; //��ȡ�������밴λ��ֵ

extern char buf_led[]; //ͨ���������ز����͵����ݻ�����

u8 data_Passward[Passward_Size]; //��ȡ����ʱʹ��

WM_HWIN CreateFramewin_1(void);     //������һ������
WM_HWIN CreateFramewin_2(void);     //�����ڶ�������
WM_HWIN CreateFramewin_3(void);     //��������������


/*****************��ʼ����Ի���***************************/
#define ID_FRAMEWIN_0   (GUI_ID_USER + 0x00)     //��ʼ����������
#define ID_BUTTON_0     (GUI_ID_USER + 0x01)     //LED0
#define ID_BUTTON_1     (GUI_ID_USER + 0x02)     //LED1
#define ID_BUTTON_2     (GUI_ID_USER + 0x03)     //LED2
#define ID_BUTTON_3     (GUI_ID_USER + 0x04)     //LED3
#define ID_BUTTON_4     (GUI_ID_USER + 0x05)     //LED4
#define ID_BUTTON_5     (GUI_ID_USER + 0x06)     //LED5
#define ID_BUTTON_6     (GUI_ID_USER + 0x07)     //LED6
#define ID_BUTTON_7     (GUI_ID_USER + 0x08)     //LED7
#define ID_BUTTON_8     (GUI_ID_USER + 0x09)     //LED8
#define ID_BUTTON_9     (GUI_ID_USER + 0x0A)     //LED9
#define ID_BUTTON_10    (GUI_ID_USER + 0x0B)     //LED10
#define ID_BUTTON_11    (GUI_ID_USER + 0x0C)     //ȡ��
#define ID_BUTTON_12    (GUI_ID_USER + 0x0D)     //�л��������
//#define ID_DROPDOWN_0   (GUI_ID_USER + 0x0E)     //����С����
#define ID_BUTTON_25    (GUI_ID_USER + 0x1D)     //�޸�����  
#define ID_BUTTON_26    (GUI_ID_USER + 0x1E)     //��������
#define ID_BUTTON_27    (GUI_ID_USER + 0x20)     //���ܴ���
#define ID_BUTTON_28    (GUI_ID_USER + 0x21)     //���ܴ���
#define ID_BUTTON_29    (GUI_ID_USER + 0x22)     //���ܴ���



/*****************���������Ի���*********************************/
#define ID_FRAMEWIN_1   (GUI_ID_USER + 0x10)     //����1����
#define ID_BUTTON_13    (GUI_ID_USER + 0x11)     //����0
#define ID_BUTTON_14    (GUI_ID_USER + 0x12)     //����1
#define ID_BUTTON_15    (GUI_ID_USER + 0x13)     //����2
#define ID_BUTTON_16    (GUI_ID_USER + 0x14)     //����3
#define ID_BUTTON_17    (GUI_ID_USER + 0x15)     //����4
#define ID_BUTTON_18    (GUI_ID_USER + 0x16)     //����5
#define ID_BUTTON_19    (GUI_ID_USER + 0x17)     //����6
#define ID_BUTTON_20    (GUI_ID_USER + 0x18)     //����7
#define ID_BUTTON_21    (GUI_ID_USER + 0x19)     //����8
#define ID_BUTTON_22    (GUI_ID_USER + 0x1A)     //����9
#define ID_BUTTON_23    (GUI_ID_USER + 0x1B)     //���س�ʼ��������
#define ID_BUTTON_24    (GUI_ID_USER + 0x1C)     //����ȷ�ϰ��������ü̵���ģ��������ȷ��򿪣�



/****************************��ʼ������************************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_1[] = {
  { FRAMEWIN_CreateIndirect, "Main Interface", ID_FRAMEWIN_0, 0, 0, 480, 320, 0, 0x0, 0 },//��ʾ����С��������Ļ��λ��
  { BUTTON_CreateIndirect, "LED0", ID_BUTTON_0, 0, 10, 80, 40, 0, 0x0, 0 },//��߿�ʼ������ ǰ��λ��ʾXY����λ�ã�������λ��ʾС���߿�Ĵ�С��������λ��δ�õ���д�㱣��
  { BUTTON_CreateIndirect, "LED1", ID_BUTTON_1, 90, 10, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED2", ID_BUTTON_2, 180, 10, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED3", ID_BUTTON_3, 0, 60, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED4", ID_BUTTON_4, 90, 60, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED5", ID_BUTTON_5, 180, 60, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED6", ID_BUTTON_6, 0, 110, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED7", ID_BUTTON_7, 90, 110, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED8", ID_BUTTON_8, 180, 110, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED9", ID_BUTTON_9, 0, 160, 80, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "LED10", ID_BUTTON_10, 90, 160, 80, 40, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "Abolish", ID_BUTTON_11, 70, 240, 120, 50, 0, 0x0, 0 },//ȡ��
  { BUTTON_CreateIndirect, "Affirm", ID_BUTTON_12, 290, 240, 120, 50, 0, 0x0, 0 },//ȷ��
//	{ DROPDOWN_CreateIndirect, "Dropdown", ID_DROPDOWN_0, 281, 59, 80, 80, 0, 0x0, 0 },//����ѡ��ͬ�����С����
	{ BUTTON_CreateIndirect, "Change Password", ID_BUTTON_25, 300, 10, 120, 40, 0, 0x0, 0 },//�޸�����
  { BUTTON_CreateIndirect, "Request Password", ID_BUTTON_26, 300, 60, 120, 40, 0, 0x0, 0 },//��������
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_27, 300, 110, 100, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_28, 300, 160, 100, 40, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Button", ID_BUTTON_29, 300, 210, 100, 40, 0, 0x0, 0 },
	
};



/**************************�����������*************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_2[] = {
	{ FRAMEWIN_CreateIndirect, "The password interface", ID_FRAMEWIN_1, 0, 0, 480, 320, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "0", ID_BUTTON_13, 160, 240, 120, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "1", ID_BUTTON_14, 0, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2", ID_BUTTON_15, 160, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3", ID_BUTTON_16, 320, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4", ID_BUTTON_17, 0, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5", ID_BUTTON_18, 160, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6", ID_BUTTON_19, 320, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "7", ID_BUTTON_20, 0, 170, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "8", ID_BUTTON_21, 160, 170, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "9", ID_BUTTON_22, 320, 170, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Return", ID_BUTTON_23, 0, 240, 120, 50, 0, 0x0, 0 },//����
  { BUTTON_CreateIndirect, "Affirm", ID_BUTTON_24, 320, 240, 120, 50, 0, 0x0, 0 },//ȷ��
};



/**************************�����޸Ľ���*************************************************/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate_3[] = {
	{ FRAMEWIN_CreateIndirect, "The password interface", ID_FRAMEWIN_1, 0, 0, 480, 320, 0, 0x0, 0 },
	{ BUTTON_CreateIndirect, "0", ID_BUTTON_13, 160, 240, 120, 50, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "1", ID_BUTTON_14, 0, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "2", ID_BUTTON_15, 160, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "3", ID_BUTTON_16, 320, 50, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "4", ID_BUTTON_17, 0, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "5", ID_BUTTON_18, 160, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "6", ID_BUTTON_19, 320, 110, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "7", ID_BUTTON_20, 0, 170, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "8", ID_BUTTON_21, 160, 170, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "9", ID_BUTTON_22, 320, 170, 150, 60, 0, 0x0, 0 },
  { BUTTON_CreateIndirect, "Return", ID_BUTTON_23, 0, 240, 120, 50, 0, 0x0, 0 },//����
  { BUTTON_CreateIndirect, "Affirm", ID_BUTTON_24, 320, 240, 120, 50, 0, 0x0, 0 },//ȷ��
};



/*********************************�����ʼ������*******************************************/
static void _cbDialog_1(WM_MESSAGE * pMsg) {
	WM_HWIN hItem_1;
  int NCode;
  int Id;


  switch (pMsg->MsgId) {
		case WM_INIT_DIALOG:
			hItem_1 = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_25);    //ָ����������ĳ����������壨�޸����룩
      BUTTON_SetFont(hItem_1, GUI_FONT_13B_1);                
		  hItem_1 = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_26);    //ָ����������ĳ����������壨�������룩
      BUTTON_SetFont(hItem_1, GUI_FONT_13B_1);
//			hItem_1 = pMsg->hWin;
//			//��ʼ��FRAMEWIN
//			FRAMEWIN_SetText(hItem_1, "Initialization Interface");  //���¶�������Ϊ���������
//			FRAMEWIN_SetFont(hItem_1, GUI_FONT_16_ASCII);           //����
//			FRAMEWIN_SetTextAlign(hItem_1, GUI_TA_HCENTER | GUI_TA_VCENTER);  //�����ı��Ķ��뷽ʽ
//			//��ʼ��DROPDOWN
//			hItem_1 = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);//���ضԻ���С���ߣ��Ĵ��ھ��
//			DROPDOWN_SetFont(hItem_1, GUI_FONT_13H_ASCII);
//			DROPDOWN_SetAutoScroll(hItem_1,1);	//�����Զ�ʹ�ù�����
//			DROPDOWN_AddString(hItem_1, "Master Bedroom");  //������
//			DROPDOWN_AddString(hItem_1, "Second Bedroom");  //������
//			DROPDOWN_AddString(hItem_1, "Drawing Room");    //����
//			DROPDOWN_AddString(hItem_1, "kitchen");         //����
//			DROPDOWN_AddString(hItem_1, "Toilet");          //����
			break;
		
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: //������Ϩ��LED0
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:  //����
				LED1=~LED1;
        printf("%c\r\n",buf_led[0]);
			break;
      case WM_NOTIFICATION_RELEASED:
				break;
      }
      break;
    case ID_BUTTON_1: //������Ϩ��LED1
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[1]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_2: //������Ϩ��LED2
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[2]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_3: //������Ϩ��LED3
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[3]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_4: //������Ϩ��LED4
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[4]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_5: //������Ϩ��LED5
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[5]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_6: //������Ϩ��LED6
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[6]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_7: //������Ϩ��LED7
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[7]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_8: //������Ϩ��LED8
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[8]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_9: //������Ϩ��LED9
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[9]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_10: //������Ϩ��LED10
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[10]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_11: //ȡ�����밴��
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_12: //ȷ�����밴��
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateFramewin_2();
			  STMFLASH_Write(Flash_Save_Passward_Addr,(u16*)Passward_Buf,Passward_Size);
        break;
      }
      break;
//			 case ID_DROPDOWN_0: // С���ߣ�����������в�ͬ�˽�����ŵĲ�ͬϰ��
//      switch(NCode) {
//      case WM_NOTIFICATION_CLICKED:
//        break;
//      case WM_NOTIFICATION_RELEASED:
//        break;
//      case WM_NOTIFICATION_SEL_CHANGED:
//				LED1=!LED1;
//        break;
//      }
//      break;
				case ID_BUTTON_25: //�޸����밴��
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_26: // �������밴��
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_27: // ��������
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_28:// ��������
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_29: // ��������
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}


static void _cbDialog_3(WM_MESSAGE * pMsg) {
	WM_HWIN hItem_3;
  int NCode;
  int Id;


  switch (pMsg->MsgId) {
		case WM_INIT_DIALOG:
			hItem_3 = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_25);    //ָ����������ĳ����������壨�޸����룩
      BUTTON_SetFont(hItem_3, GUI_FONT_13B_1);                
		  hItem_3 = WM_GetDialogItem(pMsg->hWin, ID_BUTTON_26);    //ָ����������ĳ����������壨�������룩
      BUTTON_SetFont(hItem_3, GUI_FONT_13B_1);
//			hItem_1 = pMsg->hWin;
//			//��ʼ��FRAMEWIN
//			FRAMEWIN_SetText(hItem_1, "Initialization Interface");  //���¶�������Ϊ���������
//			FRAMEWIN_SetFont(hItem_1, GUI_FONT_16_ASCII);           //����
//			FRAMEWIN_SetTextAlign(hItem_1, GUI_TA_HCENTER | GUI_TA_VCENTER);  //�����ı��Ķ��뷽ʽ
//			//��ʼ��DROPDOWN
//			hItem_1 = WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0);//���ضԻ���С���ߣ��Ĵ��ھ��
//			DROPDOWN_SetFont(hItem_1, GUI_FONT_13H_ASCII);
//			DROPDOWN_SetAutoScroll(hItem_1,1);	//�����Զ�ʹ�ù�����
//			DROPDOWN_AddString(hItem_1, "Master Bedroom");  //������
//			DROPDOWN_AddString(hItem_1, "Second Bedroom");  //������
//			DROPDOWN_AddString(hItem_1, "Drawing Room");    //����
//			DROPDOWN_AddString(hItem_1, "kitchen");         //����
//			DROPDOWN_AddString(hItem_1, "Toilet");          //����
			break;
		
  case WM_NOTIFY_PARENT:
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
    case ID_BUTTON_0: //������Ϩ��LED0
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:  //����
				LED1=~LED1;
        printf("%c\r\n",buf_led[0]);
			break;
      case WM_NOTIFICATION_RELEASED:
				break;
      }
      break;
    case ID_BUTTON_1: //������Ϩ��LED1
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[1]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_2: //������Ϩ��LED2
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[2]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_3: //������Ϩ��LED3
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[3]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_4: //������Ϩ��LED4
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[4]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_5: //������Ϩ��LED5
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[5]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_6: //������Ϩ��LED6
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[6]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_7: //������Ϩ��LED7
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[7]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_8: //������Ϩ��LED8
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[8]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_9: //������Ϩ��LED9
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[9]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_10: //������Ϩ��LED10
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				printf("%c\r\n",buf_led[10]);
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_11: //ȡ�����밴��
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_12: //ȷ�����밴��
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateFramewin_2();
			  STMFLASH_Write(Flash_Save_Passward_Addr,(u16*)Passward_Buf,Passward_Size);
        break;
      }
      break;
//			 case ID_DROPDOWN_0: // С���ߣ�����������в�ͬ�˽�����ŵĲ�ͬϰ��
//      switch(NCode) {
//      case WM_NOTIFICATION_CLICKED:
//        break;
//      case WM_NOTIFICATION_RELEASED:
//        break;
//      case WM_NOTIFICATION_SEL_CHANGED:
//				LED1=!LED1;
//        break;
//      }
//      break;
				case ID_BUTTON_25: //�޸����밴��
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_26: // �������밴��
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_27: // ��������
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_28:// ��������
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_29: // ��������
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    }
    break;
  default:
    WM_DefaultProc(pMsg);
    break;
  }
}


/************************************�����������**************************************/
static void _cbDialog_2(WM_MESSAGE * pMsg) {
	WM_HWIN hItem_2;
  int NCode;
  int Id;
  switch (pMsg->MsgId) {
  case WM_NOTIFY_PARENT:
		//���´������������壬�����С
		hItem_2 = pMsg->hWin;
	  FRAMEWIN_SetText(hItem_2, "Password Unlock Interface");
	  FRAMEWIN_SetFont(hItem_2, GUI_FONT_16_ASCII);
		FRAMEWIN_SetTextAlign(hItem_2, GUI_TA_HCENTER | GUI_TA_VCENTER);	
    Id    = WM_GetId(pMsg->hWinSrc);
    NCode = pMsg->Data.v;
    switch(Id) {
			case ID_BUTTON_13: // Notifications sent by '0'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=0;
				Passward_Buf[5]=0;
			}
			if(Bit==2)
			{
				five=0;
				Passward_Buf[4]=0;
			}
			if(Bit==3)
			{
				four=0;
				Passward_Buf[3]=0;
			}
			if(Bit==4)
			{
				three=0;
				Passward_Buf[2]=0;
			}
			if(Bit==5)
			{
				two=0;
				Passward_Buf[1]=0;
			}
			if(Bit==6)
			{
				one=0;
				Passward_Buf[0]=0;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_14: // Notifications sent by '1'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=1;
				Passward_Buf[5]=1;
			}
			if(Bit==2)
			{
				five=1;
				Passward_Buf[4]=1;
			}
			if(Bit==3)
			{
				four=1;
				Passward_Buf[3]=1;
			}
			if(Bit==4)
			{
				three=1;
				Passward_Buf[2]=1;
			}
			if(Bit==5)
			{
				two=1;
				Passward_Buf[1]=1;
			}
			if(Bit==6)
			{
				one=1;
				Passward_Buf[0]=1;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_15: // Notifications sent by '2'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=2;
				Passward_Buf[5]=2;
			}
			if(Bit==2)
			{
				five=2;
				Passward_Buf[4]=2;
			}
			if(Bit==3)
			{
				four=2;
				Passward_Buf[3]=2;
			}
			if(Bit==4)
			{
				three=2;
				Passward_Buf[2]=2;
			}
			if(Bit==5)
			{
				two=2;
				Passward_Buf[1]=2;
			}
			if(Bit==6)
			{
				one=2;
				Passward_Buf[0]=2;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_16: // Notifications sent by '3'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=3;
				Passward_Buf[5]=3;
			}
			if(Bit==2)
			{
				five=3;
				Passward_Buf[4]=3;
			}
			if(Bit==3)
			{
				four=3;
				Passward_Buf[3]=3;
			}
			if(Bit==4)
			{
				three=3;
				Passward_Buf[2]=3;
			}
			if(Bit==5)
			{
				two=3;
				Passward_Buf[1]=3;
			}
			if(Bit==6)
			{
				one=3;
				Passward_Buf[0]=3;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_17: // Notifications sent by '4'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=4;
				Passward_Buf[5]=4;
			}
			if(Bit==2)
			{
				five=4;
				Passward_Buf[4]=4;
			}
			if(Bit==3)
			{
				four=4;
				Passward_Buf[3]=4;
			}
			if(Bit==4)
			{
				three=4;
				Passward_Buf[2]=4;
			}
			if(Bit==5)
			{
				two=4;
				Passward_Buf[1]=4;
			}
			if(Bit==6)
			{
				one=4;
				Passward_Buf[0]=4;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_18: // Notifications sent by '5'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=5;
				Passward_Buf[5]=5;
			}
			if(Bit==2)
			{
				five=5;
				Passward_Buf[4]=5;
			}
			if(Bit==3)
			{
				four=5;
				Passward_Buf[3]=5;
			}
			if(Bit==4)
			{
				three=5;
				Passward_Buf[2]=5;
			}
			if(Bit==5)
			{
				two=5;
				Passward_Buf[1]=5;
			}
			if(Bit==6)
			{
				one=5;
				Passward_Buf[0]=5;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_19: // Notifications sent by '6'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=6;
				Passward_Buf[5]=6;
			}
			if(Bit==2)
			{
				five=6;
				Passward_Buf[4]=6;
			}
			if(Bit==3)
			{
				four=6;
				Passward_Buf[3]=6;
			}
			if(Bit==4)
			{
				three=6;
				Passward_Buf[2]=6;
			}
			if(Bit==5)
			{
				two=6;
				Passward_Buf[1]=6;
			}
			if(Bit==6)
			{
				one=6;
				Passward_Buf[0]=6;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_20: // Notifications sent by '7'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=7;
				Passward_Buf[5]=7;
			}
			if(Bit==2)
			{
				five=7;
				Passward_Buf[4]=7;
			}
			if(Bit==3)
			{
				four=7;
				Passward_Buf[3]=7;
			}
			if(Bit==4)
			{
				three=7;
				Passward_Buf[2]=7;
			}
			if(Bit==5)
			{
				two=7;
				Passward_Buf[1]=7;
			}
			if(Bit==6)
			{
				one=7;
				Passward_Buf[0]=7;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_21: // Notifications sent by '8'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=8;
				Passward_Buf[5]=8;
			}
			if(Bit==2)
			{
				five=8;
				Passward_Buf[4]=8;
			}
			if(Bit==3)
			{
				four=8;
				Passward_Buf[3]=8;
			}
			if(Bit==4)
			{
				three=8;
				Passward_Buf[2]=8;
			}
			if(Bit==5)
			{
				two=8;
				Passward_Buf[1]=8;
			}
			if(Bit==6)
			{
				one=8;
				Passward_Buf[0]=8;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_22: // Notifications sent by '9'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				Bit++;
			if(Bit==1)
			{
				six=9;
				Passward_Buf[5]=9;
			}
			if(Bit==2)
			{
				five=9;
				Passward_Buf[4]=9;
			}
			if(Bit==3)
			{
				four=9;
				Passward_Buf[3]=9;
			}
			if(Bit==4)
			{
				three=9;
				Passward_Buf[2]=9;
			}
			if(Bit==5)
			{
				two=9;
				Passward_Buf[1]=9;
			}
			if(Bit==6)
			{
				one=9;
				Passward_Buf[0]=9;
			}
			if(Bit==7)
			{
				Bit=6;
			}
        break;
      case WM_NOTIFICATION_RELEASED:
        break;
      }
      break;
    case ID_BUTTON_23: // Notifications sent by 'Return'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
        break;
      case WM_NOTIFICATION_RELEASED:
				GUI_EndDialog(pMsg->hWin, 0);
			  CreateFramewin_1();
        break;
      }
      break;
    case ID_BUTTON_24: // Notifications sent by 'Affirm'
      switch(NCode) {
      case WM_NOTIFICATION_CLICKED:
				  STMFLASH_Read(Flash_Save_Passward_Addr,(u16*)data_Passward,Passward_Size);
			for(Passward_i=0;Passward_i<6;Passward_i++)
			{
				 Test[Passward_i]=data_Passward[Passward_i];
			}
//			    Test[0]=data_Passward[0];
//			    Test[1]=data_Passward[1];
//			    Test[2]=data_Passward[2];
//			    Test[3]=data_Passward[3];
//			    Test[4]=data_Passward[4];
//			    Test[5]=data_Passward[5];
			    Test_Passward=Test[0]*100000+Test[1]*10000+Test[2]*1000+Test[3]*100+Test[4]*10+Test[5];
			    printf("%d\r\n",Test[0]);
			    printf("%d\r\n",Test[1]);
			    printf("%d\r\n",Test[2]);
			    printf("%d\r\n",Test[3]);
			    printf("%d\r\n",Test[4]);
			    printf("%d\r\n",Test[5]);
					Implementation_Password=six*100000+five*10000+four*1000+three*100+two*10+one;
			  if(Test_Passward==Implementation_Password)
				{
					printf("%c\r\n",buf_led[11]);
//					printf("correct\r\n");
				}
//				else
//					printf("error\r\n");
//				printf("%d\r\n",Implementation_Password);
        break;
      case WM_NOTIFICATION_RELEASED:
				Implementation_Password=0;
			   Bit=0;
         six=0;
         five=0;
         four=0;
         three=0;
         two=0;
         one=0;	
        break;
      }
      break;
    }
    break;
  default:
	
    WM_DefaultProc(pMsg);
    break;
  }
}



/*********************������ʼ������***********************/
WM_HWIN CreateFramewin_1(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_1, GUI_COUNTOF(_aDialogCreate_1), _cbDialog_1, WM_HBKWIN, 0, 0);
  return hWin;
}



/*********************���������������*************************/
WM_HWIN CreateFramewin_2(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_2, GUI_COUNTOF(_aDialogCreate_2), _cbDialog_2, WM_HBKWIN, 0, 0); //��������
  return hWin;
}


/*********************���������޸Ľ���*************************/
WM_HWIN CreateFramewin_3(void) {
  WM_HWIN hWin;

  hWin = GUI_CreateDialogBox(_aDialogCreate_3, GUI_COUNTOF(_aDialogCreate_3), _cbDialog_3, WM_HBKWIN, 0, 0); //��������
  return hWin;
}



/********************������ʼ���Ի���**************************/
void dropdown_demo(void) 
{
	WM_HWIN hWin;
	hWin = GUI_CreateDialogBox(_aDialogCreate_1, GUI_COUNTOF(_aDialogCreate_1), _cbDialog_1, WM_HBKWIN, 0, 0);//��������
	while(1)
	{
		GUI_Delay(100);
	}
}


