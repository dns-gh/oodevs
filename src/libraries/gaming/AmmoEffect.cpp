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
#include "meteo/Meteo.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AmmoEffect constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
AmmoEffect::AmmoEffect( const sword::StartFireEffect& message, Controller& controller, const CoordinateConverter_ABC& converter )
    : controller_( controller )
    , id_        ( message.fire_effect().id() )
    , type_      ( message.type() )
    , ellipse_   ( message.location(), converter )
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
    switch( type_ )
    {
        case sword::StartFireEffect::smoke:          glColor4f( COLOR_AMMO_EFFECT_SMOKE ); break;
        case sword::StartFireEffect::light:          glColor4f( COLOR_AMMO_EFFECT_LIGHT ); break;
        case sword::StartFireEffect::explosion:      glColor4f( COLOR_AMMO_EFFECT_EXPLOSION ); break;
        case sword::StartFireEffect::neutralization: glColor4f( COLOR_AMMO_EFFECT_NEUTRALIZATION ); break;
        default:
            ;
    }
    ellipse_.Draw( tools );
}

// -----------------------------------------------------------------------------
// Name: AmmoEffect::IsInside
// Created: HBD 2010-04-06
// -----------------------------------------------------------------------------
bool AmmoEffect::IsInside( const geometry::Point2f& point ) const
{
    return ellipse_.IsInside( point );
}

// -----------------------------------------------------------------------------
// Name: AmmoEffect::ApplyEffect
// Created: ABR 2011-07-27
// -----------------------------------------------------------------------------
void AmmoEffect::ApplyEffect( weather::Meteo& meteo ) const
{
    if( type_ == sword::StartFireEffect::light )
        meteo.Update( weather::PHY_Lighting::eclairant_ );
    else if( type_ == sword::StartFireEffect::smoke )
        meteo.Update( weather::PHY_Precipitation::smoke_ );
}
