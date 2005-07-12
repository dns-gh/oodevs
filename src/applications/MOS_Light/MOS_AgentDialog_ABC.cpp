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
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_AgentDialog_ABC.cpp $
// $Author: Nld $
// $Modtime: 28/11/02 14:18 $
// $Revision: 2 $
// $Workfile: MOS_AgentDialog_ABC.cpp $
//
//*****************************************************************************

#ifdef __GNUG__
#    pragma implementation
#endif

#include "MOS_Light_Pch.h"

#include "MOS_AgentDialog_ABC.h"

#ifndef MOS_USE_INLINE
#   include "MOS_AgentDialog_ABC.inl"
#endif

#include <qcombobox.h>


//-----------------------------------------------------------------------------
// Name: MOS_AgentDialog_ABC constructor
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
MOS_AgentDialog_ABC::MOS_AgentDialog_ABC( QWidget* pParent )
    : MOS_Dialog_ABC( pParent ) 
    , pAgent_       ( 0 )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentDialog_ABC destructor
// Created: NLD 2002-08-06
//-----------------------------------------------------------------------------
MOS_AgentDialog_ABC::~MOS_AgentDialog_ABC()
{
    
}

//=============================================================================
// INIT
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_AgentDialog_ABC::Initialize
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_AgentDialog_ABC::Initialize( MOS_Agent& agent )
{
    MOS_Dialog_ABC::Initialize();
    assert( !pAgent_ );
    pAgent_ = &agent;
}


//-----------------------------------------------------------------------------
// Name: MOS_AgentDialog_ABC::Cleanup
// Created: NLD 2002-07-16
//-----------------------------------------------------------------------------
void MOS_AgentDialog_ABC::Cleanup()
{
    pAgent_ = 0;    
    MOS_Dialog_ABC::Cleanup();
}
