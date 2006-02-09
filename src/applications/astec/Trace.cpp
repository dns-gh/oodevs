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
// $ATracehive: /MVW_v10/Build/SDK/Light/sTrace/Trace.cpp $
// $Author: Ape $
// $Modtime: 4/08/04 15:48 $
// $Revision: 3 $
// $Workfile: Trace.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "astec_pch.h"
#include "Trace.h"

#include "App.h"
#include "Agent_ABC.h"

using namespace DIN;

//-----------------------------------------------------------------------------
// Name: Trace constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
Trace::Trace( Agent_ABC& agent, DIN_Input& input )
    : Report_ABC( agent.GetPos() )
{
    eType_ = Report_ABC::eTrace;
    input >> strTitle_;
}

//-----------------------------------------------------------------------------
// Name: Trace destructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
Trace::~Trace()
{
}

