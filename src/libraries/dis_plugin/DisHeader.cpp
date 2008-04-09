// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dis_plugin_pch.h"
#include "DisHeader.h"
#include "EntityStatePDU.h"
#include "DetonationPDU.h"

using namespace dis;

// -----------------------------------------------------------------------------
// Name: DisHeader constructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisHeader::DisHeader( unsigned char exercise )
     : protocolVersion_   ( 6 )
     , exerciseIdentifier_( exercise )
     , PDUType_           ( 0 )
     , protocolFamily_    ( 0 )
     , timeStamp_         ( 0 )
     , length_            ( 0 )
     , padding_           ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisHeader destructor
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisHeader::~DisHeader()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DisHeader::EntityStatePDU
// Created: AGE 2008-03-10
// -----------------------------------------------------------------------------
DisHeader DisHeader::EntityStatePDU( unsigned long time, unsigned char exercise )
{
    DisHeader result( exercise );
    result.PDUType_ = 1;
    result.protocolFamily_ = 1;
    result.timeStamp_ = time;
    result.length_ = EntityStatePDU::size;
    return result;
};

// -----------------------------------------------------------------------------
// Name: DisHeader::DetonationPDU
// Created: AGE 2008-04-08
// -----------------------------------------------------------------------------
DisHeader DisHeader::DetonationPDU( unsigned long time, unsigned char exercise )
{
    DisHeader result( exercise );
    result.PDUType_ = 1;
    result.protocolFamily_ = 3;
    result.timeStamp_ = time;
    result.length_ = DetonationPDU::size;
    return result;
}
