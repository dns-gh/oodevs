//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Mathématiques Appliquées SA (MASA)
// All Rights Reserved. DIN is a trademark of MASA Corporation.
//
//*****************************************************************************
//
// $Created: NLD 2002-07-15 $
// $ATracehive: /MVW_v10/Build/SDK/MOS_Light/sTrace/MOS_Trace.cpp $
// $Author: Agn $
// $Modtime: 3/06/03 12:08 $
// $Revision: 5 $
// $Workfile: MOS_Trace.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_pch.h"

#include "MOS_Trace.h"

#ifndef MOS_USE_INLINE
#   include "MOS_Trace.inl"
#endif

#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_MOSServer.h"
#include "MOS_World.h"
#include "MOS_WorldWidget.h"
#include "MOS_Agent.h"
#include "MOS_AttrEditor.h"
#include "MOS_TraceListViewItem.h"
#include "MOS_TraceSubItem.h"

#include <QWidgetStack.h>


using namespace DIN;

//-----------------------------------------------------------------------------
// Name: MOS_Trace constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_Trace::MOS_Trace( MOS_Agent& agent )
:   agent_  ( agent )
,   vPos_   ( agent_.GetPos() )
,   nTime_  ( MOS_App::GetApp().GetTime() )
,   pItem_  ( 0 )
{
}
 
//-----------------------------------------------------------------------------
// Name: MOS_Trace destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_Trace::~MOS_Trace()
{
    if ( pItem_ ) // if != 0 then attrEditor != 0
        agent_.GetAttributeEditor()->DeleteTraceItem( *pItem_ );
}

//-----------------------------------------------------------------------------
// Name: MOS_Trace::Initialize
// Created: NLD 2002-10-07
//-----------------------------------------------------------------------------
void MOS_Trace::Initialize( DIN_Input& input )
{   
    input >> sText_;
    if( agent_.GetAttributeEditor() != 0 )
        agent_.GetAttributeEditor()->CreateTraceItem( *this );
}


//-----------------------------------------------------------------------------
// Name: std::string
// Created: FBD 03-01-27
//-----------------------------------------------------------------------------
const std::string& MOS_Trace::GetName() const
{
    return sText_;
}

