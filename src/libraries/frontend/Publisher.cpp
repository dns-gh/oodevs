// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "frontend_pch.h"
#include "Publisher.h"
#include "Master.h"
#include "DIN/MessageService/DIN_BufferedMessage.h"

using namespace frontend;

// -----------------------------------------------------------------------------
// Name: Publisher constructor
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
Publisher::Publisher( Master& master )
    : master_( master )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Publisher destructor
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
Publisher::~Publisher()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Publisher::Send
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
void Publisher::Send( const ASN1T_MsgsInMaster& message )
{
    master_.Send( message );
}

// -----------------------------------------------------------------------------
// Name: Publisher::BuildDinMsg
// Created: SBO 2007-01-25
// -----------------------------------------------------------------------------
DIN::DIN_BufferedMessage Publisher::BuildDinMsg()
{
    return master_.BuildDinMsg();
}
