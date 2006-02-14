// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_pch.h"
#include "LogisticLinks.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: LogisticLinks constructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLinks::LogisticLinks( Controller& controller, const Resolver_ABC< Agent >& resolver )
    : controller_( controller )
    , resolver_( resolver )
    , tc2_( 0 )
    , maintenanceSuperior_( 0 )
    , medicalSuperior_( 0 )
    , supplySuperior_( 0 )
{
    
}
    
// -----------------------------------------------------------------------------
// Name: LogisticLinks destructor
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
LogisticLinks::~LogisticLinks()
{

}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::DoUpdate
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
template< typename T >
void LogisticLinks::DoUpdate( const T& message )
{
    if( message.m.oid_tc2Present )
		tc2_ = resolver_.Find( message.oid_tc2 );
    if( message.m.oid_maintenancePresent )
        maintenanceSuperior_ = resolver_.Find( message.oid_maintenance );
    if( message.m.oid_santePresent )
        medicalSuperior_ = resolver_.Find( message.oid_sante );
    if( message.m.oid_ravitaillementPresent )
        supplySuperior_ = resolver_.Find( message.oid_ravitaillement );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Update
// Created: AGE 2006-02-14
// -----------------------------------------------------------------------------
void LogisticLinks::Update( const ASN1T_MsgAutomateCreation& message )
{
    DoUpdate( message );
}

// -----------------------------------------------------------------------------
// Name: LogisticLinks::Update
// Created: AGE 2006-02-13
// -----------------------------------------------------------------------------
void LogisticLinks::Update( const ASN1T_MsgChangeLiensLogistiquesAck& message )
{
    DoUpdate( message );        
}
