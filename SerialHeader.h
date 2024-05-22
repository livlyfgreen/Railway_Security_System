void serialInit()
{
    TRISC |= (1 << 7);
    TXSTA = 0x24;
    RCSTA = 0x90;
    SPBRG = 25;
}

void serialPrint(char *str)
{
    int i = 0;
    while(str[i] != '\0')
    {
        TXREG = str[i];
        while(TRMT == 0);
        i++;
    }
}

void serialRead(char *str)
{
    int i = 0;
    while(1)
    {
        while(RCIF == 0);
        str[i] = RCREG;
        if(str[i] == '\n' || str[i] == '\r')
        {
            str[i] = '\0';
            break;
        }
        i++;
    }
}
