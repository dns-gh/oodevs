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
// $Author: Ape $
// $Modtime: 4/08/04 15:48 $
// $Revision: 3 $
// $Workfile: MOS_Trace.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light2_pch.h"
#include "MOS_Trace.h"

#include "MOS_App.h"
#include "MOS_Agent.h"

using namespace DIN;

#ifndef MOS_USE_INLINE
#   include "MOS_Trace.inl"
#endif


//-----------------------------------------------------------------------------
// Name: MOS_Trace constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_Trace::MOS_Trace( MOS_Agent& agent )
    : MOS_Report_ABC( agent.GetPos() )
{
    eType_ = MOS_Report_ABC::eTrace;
}
 

//-----------------------------------------------------------------------------
// Name: MOS_Trace destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
MOS_Trace::~MOS_Trace()
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Trace::Initialize
// Created: NLD 2002-10-07
//-----------------------------------------------------------------------------
void MOS_Trace::Initialize( DIN_Input& input )
{   
    input >> strTitle_;
}
