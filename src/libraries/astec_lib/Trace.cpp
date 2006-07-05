//*****************************************************************************
//
// This file is part of MASA DIN library.
//
// Refer to the included end-user license agreement (License.rtf) for
// restrictions.
//
// Copyright (c) 2002 Math�matiques Appliqu�es SA (MASA)
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

#include "astec_pch.h"
#include "Trace.h"

//-----------------------------------------------------------------------------
// Name: Trace constructor
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
Trace::Trace( const Agent_ABC& agent, const Simulation& simulation, const TraceMessage& input )
    : Report_ABC( agent, simulation )
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
    // NOTHING
}

