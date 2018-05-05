#ifdef HAVE_PRAGMA_ONCE
#pragma once
#endif
#ifndef __DIPLOMATICREQUEST_DYNAMIC_ARRAY_H__
#define __DIPLOMATICREQUEST_DYNAMIC_ARRAY_H__

#include "gs/gameobj/DiplomaticRequest.h"
#include "robot/aibackdoor/dynarr.h"

class DiplomaticRequestDynamicArray : public DynamicArray<DiplomaticRequest> {

public:

    DiplomaticRequestDynamicArray();
    DiplomaticRequestDynamicArray(const sint32 size);
    DiplomaticRequestDynamicArray (const DynamicArray<DiplomaticRequest> &copyme);

};

#endif
