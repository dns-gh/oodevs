// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "Client_ABC.h"
#include "ObjectMessageService.h"
#include "DIN/MessageService/DIN_BufferedMessage.h"
#include "DIN/DIN_Link.h"

using namespace tools;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Client_ABC constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Client_ABC::Client_ABC( ObjectMessageService& messageService, DIN_Link& link )
    : DIN_UserData_ABC()
    , messageService_ ( messageService )
    , link_           ( link )
{
    assert( !link_.GetUserData() );
    link_.SetUserData( this );        

    messageService_.Enable( link_ );
}

//-----------------------------------------------------------------------------
// Name: Client_ABC destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Client_ABC::~Client_ABC()
{
    messageService_.Disable( link_ );

    assert( link_.GetUserData() == this );
    link_.SetUserData( 0 );
}

// -----------------------------------------------------------------------------
// Name: Client_ABC::Disconnect
// Created: NLD 2006-10-05
// -----------------------------------------------------------------------------
void Client_ABC::Disconnect()
{
    link_.Close( true ); 
}

// -----------------------------------------------------------------------------
// Name: Client_ABC::BuildDinMsg
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
DIN_BufferedMessage Client_ABC::BuildDinMsg()
{
    return DIN_BufferedMessage( messageService_ );
}

