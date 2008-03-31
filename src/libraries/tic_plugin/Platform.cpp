// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "tic_plugin_pch.h"
#include "Platform.h"
#include "clients_kernel/ComponentType.h"

using namespace tic;

// -----------------------------------------------------------------------------
// Name: Platform constructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
Platform::Platform( const kernel::ComponentType& type )
    : type_      ( type )
    , state_     ( okay )
    , nSpeed_    ( 0 )
    , nDirection_( 0 )
    , nHeight_   ( 0 )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Platform destructor
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
Platform::~Platform()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Platform::Update
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void Platform::Update( const ASN1T_MsgUnitAttributes& asnMsg )
{
    if( asnMsg.m.vitessePresent )
        nSpeed_ = asnMsg.vitesse;
    if( asnMsg.m.directionPresent )
        nDirection_ = asnMsg.direction;
    if( asnMsg.m.hauteurPresent )
        nHeight_ = asnMsg.hauteur;
}

// -----------------------------------------------------------------------------
// Name: Platform::Spread
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void Platform::Spread( ASN1T_EquipmentDotations& updateMessage )
{
    if( type_.GetId() == updateMessage.type_equipement )
        Apply( updateMessage );
}

// -----------------------------------------------------------------------------
// Name: Platform::Apply
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
void Platform::Apply( ASN1T_EquipmentDotations& updateMessage )
{
       SetStatus( updateMessage.nb_dans_chaine_maintenance, destroyed )
    || SetStatus( updateMessage.nb_indisponibles,           destroyed )
    || SetStatus( updateMessage.nb_prisonniers,             destroyed )
    || SetStatus( updateMessage.nb_reparables,              broken )
    || SetStatus( updateMessage.nb_disponibles,             okay );
}

// -----------------------------------------------------------------------------
// Name: Platform::SetStatus
// Created: AGE 2008-03-31
// -----------------------------------------------------------------------------
bool Platform::SetStatus( int& number, E_State state )
{
    if( number )
    {
        --number;
        state_ = state;
        return true;
    }
    return false;
}
