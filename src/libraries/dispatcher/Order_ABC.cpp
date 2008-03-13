// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "dispatcher_pch.h"
#include "Order_ABC.h"
#include "Network_Def.h"
#include "MissionParameter_ABC.h"

using namespace dispatcher;

// -----------------------------------------------------------------------------
// Name: Order_ABC constructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
Order_ABC::Order_ABC( Model& model, unsigned int missionID, const ASN1T_MissionParameters& parameters )
    : model_( model )
    , missionID_ ( missionID )
    , parameters_()
{
    InitializeParameters( parameters );   
}

// -----------------------------------------------------------------------------
// Name: Order_ABC destructor
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
Order_ABC::~Order_ABC()
{
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        delete *it;
}

// -----------------------------------------------------------------------------
// Name: Order_ABC::InitializeParameters
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Order_ABC::InitializeParameters( const ASN1T_MissionParameters& parameters )
{
    for( unsigned i = 0; i < parameters.n; ++i )
        parameters_.push_back( MissionParameter_ABC::Create( model_, parameters.elem[ i ] ) );
}

// -----------------------------------------------------------------------------
// Name: Order_ABC::Send
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Order_ABC::Send( ASN1T_MissionParameters& asn ) const
{
    asn.n    = parameters_.size();
    asn.elem = asn.n ? new ASN1T_MissionParameter[ parameters_.size() ] : 0;
    unsigned i = 0;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (**it).Send( asn.elem[i++] );
}

// -----------------------------------------------------------------------------
// Name: Order_ABC::AsnDelete
// Created: NLD 2007-04-20
// -----------------------------------------------------------------------------
void Order_ABC::AsnDelete( ASN1T_MissionParameters& asn ) const
{
    unsigned i = 0;
    for( CIT_Parameters it = parameters_.begin(); it != parameters_.end(); ++it )
        (**it).AsnDelete( asn.elem[i++] );
    delete[] asn.elem;
}
