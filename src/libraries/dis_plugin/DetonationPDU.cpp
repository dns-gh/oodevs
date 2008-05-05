// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "DetonationPDU.h"

using namespace dis;

// -----------------------------------------------------------------------------
// Name: DetonationPDU constructor
// Created: AGE 2008-04-30
// -----------------------------------------------------------------------------
DetonationPDU::DetonationPDU( unsigned long time, unsigned char exercise )
    : header_( DisHeader::DetonationPDU( time, exercise ) )
    , detonationResult_              ( 0 )
    , numberOfArticulationParameters_( 0 )
    , padding_                       ( 0 )
{
    ::memset( locInEntityCoords_, 0, sizeof( locInEntityCoords_ ) );
}

// -----------------------------------------------------------------------------
// Name: DetonationPDU destructor
// Created: AGE 2008-04-30
// -----------------------------------------------------------------------------
DetonationPDU::~DetonationPDU()
{
    // NOTHING
}
