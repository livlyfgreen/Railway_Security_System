void analogInit()
{
    TRISA |= 0xFF;
    ADCON0 = 0x41;
    ADCON1 = 0x80;
}
short analogRead(int channel)
{
    short result;
    switch(channel)
    {
        case 0: ADCON0 = 0x41; break;
        case 1: ADCON0 = 0x49; break;
        case 2: ADCON0 = 0x51; break;
        case 3: ADCON0 = 0x59; break;
        case 4: ADCON0 = 0x61; break;
    }
    GO = 1;
    while(GO == 1);
    result = (ADRESH << 8) | ADRESL;
    return result;
}