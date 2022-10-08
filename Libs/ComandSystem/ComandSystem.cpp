#include "ComandSystem.h"

#include "..\Logging\Logging.h"

int InitHeader(Header* header, int comands_number)
{
    CHECK(header == nullptr, "Null header", -1);

    header->signature        = SIGNATURE;
    header->version          = ASM_VERSION;
    header->comands_number   = comands_number;

    return 0;
}