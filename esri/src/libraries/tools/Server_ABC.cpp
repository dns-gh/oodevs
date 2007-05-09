// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tools_pch.h"
#include "Server_ABC.h"
#include "DIN/MessageService/DIN_MessageService_ABC.h"
#include "DIN/MessageService/DIN_BufferedMessage.h"
#include "DIN/DIN_Link.h"

using namespace tools;
using namespace DIN;

// -----------------------------------------------------------------------------
// Name: Server_ABC constructor
// Created: NLD 2006-09-20
// -----------------------------------------------------------------------------
Server_ABC::Server_ABC( DIN_MessageService_ABC& messageService, DIN_Link& link )
    : DIN_UserData_ABC()
    , messageService_ ( messageService )
    , link_           ( link )
{
    assert( !link_.GetUserData() );
    link_.SetUserData( this );        
    messageService_.Enable( link_ );
}

//-----------------------------------------------------------------------------
// Name: Server_ABC destructor
// Created: NLD 2002-07-12
//-----------------------------------------------------------------------------
Server_ABC::~Server_ABC()
{
    messageService_.Disable( link_ );

    assert( link_.GetUserData() == this );
    link_.SetUserData( 0 );
}

// -----------------------------------------------------------------------------
// Name: Server_ABC::BuildDinMsg
// Created: NLD 2006-09-27
// -----------------------------------------------------------------------------
DIN_BufferedMessage Server_ABC::BuildDinMsg()
{
    return DIN_BufferedMessage( messageService_ );
}

// -----------------------------------------------------------------------------
// Name: Server_ABC::GetServerFromLink
// Created: NLD 2006-09-21
// -----------------------------------------------------------------------------
// static
Server_ABC& Server_ABC::GetServerFromLink( const DIN::DIN_Link& link )
{
    DIN::DIN_UserData_ABC* pTmp = link.GetUserData();
    assert( pTmp );
    return *static_cast< Server_ABC* >( pTmp );    
}