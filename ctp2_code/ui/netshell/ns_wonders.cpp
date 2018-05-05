//----------------------------------------------------------------------------
//
// Project      : Call To Power 2
// File type    : C++ source
// Description  : Multiplayer wonders list
// Id           : $Id$
//
//----------------------------------------------------------------------------
//
// Disclaimer
//
// THIS FILE IS NOT GENERATED OR SUPPORTED BY ACTIVISION.
//
// This material has been developed at apolyton.net by the Apolyton CtP2
// Source Code Project. Contact the authors at ctp2source@apolyton.net.
//
//----------------------------------------------------------------------------
//
// Compiler flags
//
// - None
//
//----------------------------------------------------------------------------
//
// Modifications from the original Activision code:
//
// - Initialized local variables. (Sep 9th 2005 Martin G�hmann)
//
//----------------------------------------------------------------------------
#include "ctp/c3.h"

#include "ui/netshell/ns_wonders.h"

#include <algorithm>
#include "ui/aui_common/aui_stringtable.h"
#include "gs/database/StrDB.h"              // g_theStringDB
#include "gs/newdb/WonderRecord.h"       // g_theWonderDB


ns_Wonders *    g_nsWonders = NULL;

ns_Wonders::ns_Wonders()
:
    m_stringtable   (NULL)
{
	Assert(g_theWonderDB->NumRecords() <= k_WONDERS_MAX);
    sint32      numWonders  =
        std::min(k_WONDERS_MAX, g_theWonderDB->NumRecords());

	AUI_ERRCODE errcode     = AUI_ERRCODE_OK;
	m_stringtable = new aui_StringTable( &errcode, numWonders );
	Assert( AUI_NEWOK(m_stringtable,errcode) );
	if ( !AUI_NEWOK(m_stringtable,errcode) ) return;

	for ( sint32 i = 0; i < numWonders; i++ )
	{
		StringId stringNum = g_theWonderDB->Get(i)->m_name;
		m_stringtable->SetString(g_theStringDB->GetNameStr(stringNum), i);
	}
}

ns_Wonders::~ns_Wonders()
{
	delete m_stringtable;
}
