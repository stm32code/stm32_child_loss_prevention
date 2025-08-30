#include "git.h"

Data_TypeDef Data_init;						  // �豸���ݽṹ��
Threshold_Value_TypeDef threshold_value_init; // �豸��ֵ���ýṹ��
Device_Satte_Typedef device_state_init;		  // �豸״̬

extern int32_t n_sp02;		 // SPO2 value
extern int8_t ch_spo2_valid; // indicator to show if the SP02 calculation is valid
extern int32_t n_heart_rate; // heart rate value
extern int8_t ch_hr_valid;	 // indicator to show if the heart rate calculation is valid
void errorLog(U8 num)
{
	while (1)
	{
		printf("ERROR%d\r\n", num);
	}
}
// ��ȡGPS��λ��Ϣ
void parseGpsBuffer()
{
	char *subString;
	char *subStringNext;
	char i = 0;
	char usefullBuffer[2];
	if (Save_Data.isGetData)
	{
		Save_Data.isGetData = false;
		for (i = 0; i <= 6; i++)
		{
			if (i == 0)
			{
				if ((subString = strstr(Save_Data.GPS_Buffer, ",")) == NULL)
					errorLog(1); // ��������
			}
			else
			{
				subString++;
				if ((subStringNext = strstr(subString, ",")) != NULL)
				{
					switch (i)
					{
					case 1:
						memcpy(Save_Data.UTCTime, subString, subStringNext - subString);
						break; // ��ȡUTCʱ��
					case 2:
						memcpy(usefullBuffer, subString, subStringNext - subString);
						break; // ��ȡUTCʱ��
					case 3:
						memcpy(Save_Data.latitude, subString, subStringNext - subString);

						break; // ��ȡγ����Ϣ
					case 4:
						memcpy(Save_Data.N_S, subString, subStringNext - subString);
						break; // ��ȡN/S
					case 5:
						memcpy(Save_Data.longitude, subString, subStringNext - subString);
						break; // ��ȡ������Ϣ
					case 6:
						memcpy(Save_Data.E_W, subString, subStringNext - subString);
						break; // ��ȡE/W

					default:
						break;
					}

					subString = subStringNext;
					Save_Data.isParseData = true;
					if (usefullBuffer[0] == 'A')
					{
						device_state_init.location_state = 1;
						Save_Data.isUsefull = true;
					}

					else if (usefullBuffer[0] == 'V')
						Save_Data.isUsefull = false;

					// if (Save_Data.latitude > 0 && Save_Data.longitude > 0)
					// {
					// }
				}
				else
				{
					errorLog(2); // ��������
				}
			}
		}
	}
}
F32 longitude_sum, latitude_sum;
U8 longitude_int, latitude_int;
void printGpsBuffer()
{
	// ת��Ϊ����
	longitude_sum = atof(Save_Data.longitude);
	latitude_sum = atof(Save_Data.latitude);
	printf("ά�� = %.6f %.6f\r\n",longitude_sum,latitude_sum);
	// ����
	longitude_int = longitude_sum / 100;
	latitude_int = latitude_sum / 100;

	// ת��Ϊ��γ��
	longitude_sum = longitude_int + ((longitude_sum / 100 - longitude_int) * 100) / 60;
	latitude_sum = latitude_int + ((latitude_sum / 100 - latitude_int) * 100) / 60;
	device_state_init.location_state = 1;
	if (Save_Data.isParseData)
	{
		Save_Data.isParseData = false;

		// printf("Save_Data.UTCTime = %s\r\n", Save_Data.UTCTime);
		if (Save_Data.isUsefull)
		{
			Save_Data.isUsefull = false;
		}
		else
		{
			// printf("GPS DATA is not usefull!\r\n");
		}
	}
}
// ��ȡ���ݲ���
mySta Read_Data(Data_TypeDef *Device_Data)
{
	char str[50];
	// ��ȡ��γ��
	parseGpsBuffer();
	if (device_state_init.location_state == 1)
	{
		// ������γ��
		printGpsBuffer();
	}



	return MY_SUCCESSFUL;
}
// ��ʼ��
mySta Reset_Threshole_Value(Threshold_Value_TypeDef *Value, Device_Satte_Typedef *device_state)
{
	
//	longitude_sum =103.939483;
//	latitude_sum = 30.829862;
//	// д
//	W_Test();
	// ��
	R_Test();
	// ״̬����
	device_state->check_device = 0;

	return MY_SUCCESSFUL;
}
mySta Update_oled_massage()
{
#if OLED // �Ƿ��
	char str[50];
	// ��λ�ɹ�
	if (device_state_init.location_state == 1)
	{
		sprintf(str, "��λ�ɹ�: ʵʱ ");
		OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "����: %.6f     ", longitude_sum);
		OLED_ShowCH(0, 2, (unsigned char *)str);
		sprintf(str, "����: %.6f     ", latitude_sum);
		OLED_ShowCH(0, 4, (unsigned char *)str);
		sprintf(str, "	 ״̬����    ");
		OLED_ShowCH(0, 6, (unsigned char *)str);
	}
	else if (device_state_init.location_state == 0)
	{
		sprintf(str, "��λʧ��: ���� ");
		OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "����: %.6f     ", longitude_sum);
		OLED_ShowCH(0, 2, (unsigned char *)str);
		sprintf(str, "����: %.6f     ", latitude_sum);
		OLED_ShowCH(0, 4, (unsigned char *)str);
		sprintf(str, "�뵽�����ش���λ");

		OLED_ShowCH(0, 6, (unsigned char *)str);
	}

#endif

	return MY_SUCCESSFUL;
}

// �����豸״̬
mySta Update_device_massage()
{
	// �Զ�ģʽ
	char str[50];
	if (device_state_init.Key_State == 1 || device_state_init.Fall_time > 1 || device_state_init.Disce_time > 1 )
	{
		BEEP = ~BEEP;
		
	}
	else
	{
		BEEP = 0;
	}
	// �����涨
	if (device_state_init.massgae_wanning == 1)
	{
		 // ������ʾ
		OLED_Clear();
		sprintf(str, "---------------");
		OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "--SendMassage--");
		OLED_ShowCH(0, 2, (unsigned char *)str);
		sprintf(str, "---------------");
		OLED_ShowCH(0, 4, (unsigned char *)str);
		GA10_Send_GMS(2);
		device_state_init.massgae_wanning = 2;
		Connect_Net = 0; // ����
		delay_ms(300);
	}
	// ����
	if (device_state_init.waning == 1)
	{
		 // ������ʾ
		OLED_Clear();
		sprintf(str, "---------------");
		OLED_ShowCH(0, 0, (unsigned char *)str);
		sprintf(str, "--SendMassage--");
		OLED_ShowCH(0, 2, (unsigned char *)str);
		sprintf(str, "---------------");
		OLED_ShowCH(0, 4, (unsigned char *)str);
		GA10_Send_GMS(1);
		device_state_init.waning = 2;
		Connect_Net = 0; // ����
		delay_ms(300);
	}
	return MY_SUCCESSFUL;
}

// ��ʱ��
void Automation_Close(void)
{
	if (Data_init.App)
	{
		switch (Data_init.App)
		{
		case 1:
			// ������Ϣ
			Mqtt_Pub(1);
			break;
		case 2:
	
			break;
		}
		Data_init.App = 0;
		Data_init.SendTime = 0;
	}

}
// ��ⰴ���Ƿ���
static U8 num_on = 0;
static U8 key_old = 0;
void Check_Key_ON_OFF()
{
	U8 key;
	
	key = KEY_Scan(1);
	// ����һ�εļ�ֵ�Ƚ� �������ȣ������м�ֵ�ı仯����ʼ��ʱ
	if (key != 0 && num_on == 0)
	{
		key_old = key;
		num_on = 1;
	}
	if (key != 0 && num_on >= 1 && num_on <= Key_Scan_Time) // 25*10ms
	{
		num_on++; // ʱ���¼��
	}
	if (key == 0 && num_on > 0 && num_on < Key_Scan_Time) // �̰�
	{
		switch (key_old)
		{
		case KEY1_PRES:
			printf("Key1_Short\n");

			break;

		default:
			break;
		}
		num_on = 0;
	}
	else if (key == 0 && num_on >= Key_Scan_Time) // ����
	{
		switch (key_old)
		{
		case KEY1_PRES:
			printf("Key1_Long\n");
			if (device_state_init.waning == 0)
			{
				device_state_init.waning = 1;
			
			}
			else
			{
				device_state_init.waning = 0;
			}


			break;

		default:
			break;
		}
		num_on = 0;
	}
}

