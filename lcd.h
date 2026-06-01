#include "types.h"
void WriteLCD(u8 data);
void CmdLCD(u8 cmd);
void CharLCD(u8 ascii);
void InitLCD(void);
void StrLCD(u8* str);
void U32LCD(u32 n);
void S32LCD(s32 n);
void BinLCD(u32 n, u8 nbd);
void BuildCGRAM(u8* p, u8 nBytes);
void f32LCD(f32 fnum, u8 ndp);
void OctLCD(u32 n);
void HexLCD(u32 n);

