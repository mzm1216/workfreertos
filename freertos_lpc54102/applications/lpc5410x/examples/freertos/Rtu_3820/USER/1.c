signal void Write_SKY72310_ADF4001_Data(uint8_t Chip,uint8_t BitLen,uint32_t regdata)//chip : 0 - IF, 1 - TX, 2 - RX;BitLen: write bitlen;ulD4113:write to reg data
{//需要配置成16位数据，4位地址位，12位数据位
	int32_t i;
	uint32_t temp;
	    
	temp=0x00000001;
	temp = temp << (BitLen -1);

//   	I3C_Set_CS_high(Chip);
//	I3C_Set_Data_high(Chip);
//	I3C_Set_CLK_low(Chip);
	//for (j = 0; j < 2; j++);
	
	I3C_Set_CS_low(Chip);//cs low
	//for (j = 0; j < 2; j++);
    
	for (i = 0; i < BitLen; i++)
	{
		I3C_Set_CLK_low(Chip);             //CLOCK拉低
		if (regdata & temp) //0x00800000)
		{
			I3C_Set_Data_high(Chip);//data high
		}
		else 
		{
			I3C_Set_Data_low(Chip);//data low
		}

		regdata <<= 1;
		Delay_ms(1);
		I3C_Set_CLK_high(Chip);
		Delay_ms(1);

	}
    
	I3C_Set_CLK_low(Chip);
//	I3C_Set_Data_high(Chip);
	I3C_Set_CS_high(Chip);
	
	Delay_ms(50);
	return;                                                                                
}