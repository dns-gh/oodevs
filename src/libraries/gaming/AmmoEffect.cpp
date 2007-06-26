// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_pch.h"
#include "AmmoEffect.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Controller.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AmmoEffect constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
AmmoEffect::AmmoEffect( const ASN1T_MsgStartFireEffect& message, Controller& controller, const CoordinateConverter_ABC& converter )
    : controller_( controller )
    , id_( message.effect_oid )
    , type_( message.type )
    , ellipse_( message.location, converter )
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
        glColor4f( COLOR_SMOKE );
    else
        glColor4f( COLOR_LIGHT );
    ellipse_.Draw( tools );
}
