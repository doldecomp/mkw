#include "NandUtil.hpp"

int NandUtil_safeOpen(const char* a1, NANDFileInfo* a2, u8 a3, void* a4,
                      u32 a5) {
  int v5;  // r28
  int v8;  // r25
  int v11; // r30
  int v12; // r29

  v5 = 0;
  v8 = a3;
  v11 = 1;
  v12 = 8;
  for (int v5 = 0; v5 < 3; ++v5) {
    switch (NANDSafeOpen(a1, a2, v8, a4, a5)) {
    case 0:
      v12 = 0;
      break;
    case -3:
    case -2:
      if (v5 >= 3)
        v12 = 2; // not reached
      else
        v11 = 0;
      break;
    case -4:
      v12 = 6;
      break;
    case -15:
    case -5:
    case -1:
      v12 = 3;
      break;
    case -12:
      v12 = 4;
      break;
    default:
      v12 = 8;
      break;
    }
    if (v11)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v12;
}

int NandUtil_open(const char* a1, NANDFileInfo* a2, u8 a3) {
  int v7; // r27
  int v8; // r26

  v7 = 1;
  v8 = 8;
  for (int v3 = 0; v3 < 3; ++v3) {
    switch (NANDOpen(a1, a2, a3)) {
    case 0:
      v8 = 0;
      break;
    case -3:
    case -2:
      if (v3 >= 3)
        v8 = 2; // not reached
      else
        v7 = 0;
      break;
    case -1:
      v8 = 3;
      break;
    case -12:
      v8 = 4;
      break;
    default:
      v8 = 8;
      break;
    }
    if (v7)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v8;
}

int NandUtil_safeClose(NANDFileInfo* a1) {
  int v3; // r27
  int v4; // r26

  v3 = 1;
  v4 = 8;
  for (int v1 = 0; v1 < 3; ++v1) {
    switch (NANDSafeClose(a1)) {
    case 0:
      v4 = 0;
      break;
    case -3:
    case -2:
      if (v1 >= 3)
        v4 = 2; // not reached
      else
        v3 = 0;
      break;
    case -4:
      v4 = 6;
      break;
    case -1:
    case -14:
      v4 = 3;
      break;
    default:
      v4 = 8;
      break;
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v4;
}

int NandUtil_close(NANDFileInfo* a1) {
  int v3; // r27
  int v4; // r26

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    switch (NANDClose(a1)) {
    case 0:
      v4 = 0;
      break;
    case -3:
    case -2:
      if (v1 >= 3)
        v4 = 2; // not reached
      else
        v3 = 0;
      break;
    case -4:
      v4 = 6;
      break;
    case -1:
      v4 = 3;
      break;
    default:
      v4 = 8;
      break;
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v4;
}

int NandUtil_read(NANDFileInfo* a1, u32 a2, u32 a3, int a4) {
  int v3;
  int v4;
  int v5;

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    v5 = NANDSeek(a1, a4, 0);
    if (v5 == a4) {
      v4 = 0;
    }
    else {
      switch (v5) {
      case -3:
      case -2:
        if (v1 >= 3)
          v4 = 2;
        else
          v3 = 0;
        break;
      case -1:
        v4 = 3;
        break;
      default:
        v4 = 8;
        break;
      }
    }

    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }

  if (v4 != 0) {
    return v4;
  }

  for (int v1 = 0; v1 < 3; ++v1) {
    v5 = NANDRead(a1, (void*)a2, a3);
    if (v5 == (u32)a3) {
      v4 = 0;
    }
    else if (v5 >= 0) {
      v4 = 3;
    }
    else {
      switch (v5) {
      case -3:
      case -2:
        if (v1 >= 3)
          v4 = 2; // not reached
        else
          v3 = 0;
        break;
      case -4:
        v4 = 6;
        break;
      case -15:
      case -5:
      case -1:
        v4 = 3;
        break;
      default:
        v4 = 8;
        break;
      }
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }

  return v4;
}

int NandUtil_getLength(NANDFileInfo* a1, u32* a2) {
  int v3;
  int v4;

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    switch (NANDGetLength(a1, a2)) {
    case 0:
      v4 = 0;
      break;
    case -3:
    case -2:
      if (v1 >= 3)
        v4 = 2; // not reached
      else
        v3 = 0;
      break;
    case -4:
      v4 = 6;
      break;
    default:
      v4 = 8;
      break;
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v4;
}

int NandUtil_write(NANDFileInfo* a1, u32 a2, u32 a3, int a4) {
  int v3;
  int v4;
  int v5;

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    v5 = NANDSeek(a1, a4, 0);
    if (v5 == a4) {
      v4 = 0;
    }
    else {
      switch (v5) {
      case -3:
      case -2:
        if (v1 >= 3)
          v4 = 2;
        else
          v3 = 0;
        break;
      case -1:
        v4 = 3;
        break;
      default:
        v4 = 8;
        break;
      }
    }

    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }

  if (v4 != 0) {
    return v4;
  }

  for (int v1 = 0; v1 < 3; ++v1) {
    v5 = NANDWrite(a1, (void*)a2, a3);
    if (v5 == (u32)a3) {
      v4 = 0;
    }
    else if (v5 >= 0) {
      v4 = 3;
    }
    else {
      switch (v5) {
      case -3:
      case -2:
        if (v1 >= 3)
          v4 = 2; // not reached
        else
          v3 = 0;
        break;
      case -4:
        v4 = 6;
        break;
      default:
        v4 = 8;
        break;
      }
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }

  return v4;
}

int NandUtil_check(u32 a1, u32 a2, u32* a3) {
  int v3;
  int v4;

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    switch (NANDCheck(a1, a2, a3)) {
    case 0:
      v4 = 0;
      break;
    case -3:
    case -2:
      if (v1 >= 3)
        v4 = 2; // not reached
      else
        v3 = 0;
      break;
    default:
      v4 = 8;
      break;
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v4;
}

int NandUtil_createDir(const char* a1, u8 a2) {
  int v3;
  int v4;

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    switch (NANDCreateDir(a1, a2, 0)) {
    case 0:
    case -6:
      v4 = 0;
      break;
    case -3:
    case -2:
      if (v1 >= 3)
        v4 = 2; // not reached
      else
        v3 = 0;
      break;
    case -4:
      v4 = 6;
      break;
    case -1:
      v4 = 3;
      break;
    default:
      v4 = 8;
      break;
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v4;
}

int NandUtil_getType(const char* a1, u32* a2) {
  int v3;
  int v4;
  u8 tmp[4];

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    switch (NANDGetType(a1, &tmp[0])) {
    case 0:
      v4 = 0;
      if (tmp[0] == 1)
        *a2 = 1;
      else
        *a2 = 2;
      break;
    case -12:
      v4 = 0;
      *a2 = 0;
      break;
    case -3:
    case -2:
      if (v1 >= 3)
        v4 = 2; // not reached
      else
        v3 = 0;
      break;
    case -1:
      v4 = 3;
      break;
    default:
      v4 = 8;
      break;
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v4;
}

int NandUtil_create(const char* a1, u8 a2) {
  int v3;
  int v4;

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    switch (NANDCreate(a1, a2, 0)) {
    case 0:
    case -6:
      v4 = 0;
      break;
    case -3:
    case -2:
      if (v1 >= 3)
        v4 = 2; // not reached
      else
        v3 = 0;
      break;
    case -4:
      v4 = 6;
      break;
    case -1:
      v4 = 3;
      break;
    default:
      v4 = 8;
      break;
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v4;
}

int NandUtil_delete(const char* a1) {
  int v3;
  int v4;

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    switch (NANDDelete(a1)) {
    case 0:
    case -12:
      v4 = 0;
      break;
    case -3:
    case -2:
      if (v1 >= 3)
        v4 = 2; // not reached
      else
        v3 = 0;
      break;
    case -4:
      v4 = 6;
      break;
    case -1:
      v4 = 3;
      break;
    default:
      v4 = 8;
      break;
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v4;
}

int NandUtil_setStatus(const char* a1, const NANDStatus* a2) {
  int v3;
  int v4;

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    switch (NANDSetStatus(a1, a2)) {
    case 0:
      v4 = 0;
      break;
    case -3:
    case -2:
      if (v1 >= 3)
        v4 = 2; // not reached
      else
        v3 = 0;
      break;
    case -12:
      v4 = 4;
      break;
    case -4:
      v4 = 6;
      break;
    case -1:
      v4 = 3;
      break;
    default:
      v4 = 8;
      break;
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v4;
}

int NandUtil_getStatus(const char* a1, NANDStatus* a2) {
  int v3;
  int v4;

  v3 = 1;
  v4 = 8;

  for (int v1 = 0; v1 < 3; ++v1) {
    switch (NANDGetStatus(a1, a2)) {
    case 0:
      v4 = 0;
      break;
    case -3:
    case -2:
      if (v1 >= 3)
        v4 = 2; // not reached
      else
        v3 = 0;
      break;
    case -12:
      v4 = 4;
      break;
    case -4:
      v4 = 6;
      break;
    case -1:
      v4 = 3;
      break;
    default:
      v4 = 8;
      break;
    }
    if (v3)
      break;
    OSSleepTicks(OSMillisecondsToTicks(100ll));
  }
  return v4;
}

void unk_8052bd38(const char* a1) {
  NANDStatus tmp;
  NandUtil_getStatus(a1, &tmp);
}
