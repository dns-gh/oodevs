// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "astec_gaming_pch.h"
#include "AmmoEffect.h"
#include "astec_gui/GlTools_ABC.h"
#include "Controller.h"

// -----------------------------------------------------------------------------
// Name: AmmoEffect constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
AmmoEffect::AmmoEffect( const ASN1T_MsgStartFireEffect& message, Controller& controller, const CoordinateConverter_ABC& converter )
    : controller_( controller )
    , id_( message.oid_effet )
    , type_( message.type )
    , ellipse_( message.localisation, converter )
{
    controller_.Create( *this );
}

// -----------------------------------------------------------------------------
// Name: AmmoEffect destructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
AmmoEffect::~AmmoEffect()
{
    controller_.Delete( *this );
}

// -----------------------------------------------------------------------------
// Name: AmmoEffect::Draw
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void AmmoEffect::Draw( const GlTools_ABC& tools ) const
{
    if( type_ == EnumFireEffectType::fumigene )
        glColor4d( COLOR_SMOKE );
    else
        glColor4d( COLOR_LIGHT );
    ellipse_.Draw( tools );
}
