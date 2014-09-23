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
#include "Simulation.h"
#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/SoundEvent.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Profile_ABC.h"
#include "meteo/Meteo.h"
#include "meteo/PHY_Lighting.h"
#include "meteo/PHY_Precipitation.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: AmmoEffect constructor
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
AmmoEffect::AmmoEffect( const sword::StartFireEffect& message, Controller& controller, const CoordinateConverter_ABC& converter, const tools::Resolver_ABC< kernel::Agent_ABC >& agents, kernel::Profile_ABC& profile )
    : controller_( controller )
    , agents_( agents )
    , id_( message.fire_effect().id() )
    , type_( message.type() )
    , ellipse_( message.location(), converter )
    , profile_( profile )
{
    controller_.Create( *this );
    PlaySoundEffect();
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
// Name: AmmoEffect::PlaySoundEffect
// Created: NPT 2013-07-05
// -----------------------------------------------------------------------------
void AmmoEffect::PlaySoundEffect() const
{
    if( IsVisible( profile_ ) )
    {
        switch( type_ )
        {
            case sword::StartFireEffect::smoke:             controller_.Update( gui::SoundEvent( 0, "indirectsmoke", gui::SoundEvent::eSingleShot ) ); break;
            case sword::StartFireEffect::light:             controller_.Update( gui::SoundEvent( 0, "indirectillumination", gui::SoundEvent::eSingleShot ) ); break;
            case sword::StartFireEffect::explosion:         controller_.Update( gui::SoundEvent( 0, "indirectexplosive", gui::SoundEvent::eSingleShot ) ); break;
            case sword::StartFireEffect::neutralization:    controller_.Update( gui::SoundEvent( 0, "indirecteffect", gui::SoundEvent::eSingleShot ) ); break;
        default:
            ;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: AmmoEffect::IsVisible
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
bool AmmoEffect::IsVisible( const kernel::Profile_ABC& profile ) const
{
    if( profile.IsSupervision() )
        return true;
    for( auto it = perceivers_.begin(); it != perceivers_.end(); ++it )
    {
        const kernel::Entity_ABC* entity = agents_.Find( *it );
        if( entity && profile.IsVisible( *entity ) )
            return true;
    }
    return false;
}

// -----------------------------------------------------------------------------
// Name: AmmoEffect::Draw
// Created: AGE 2006-04-04
// -----------------------------------------------------------------------------
void AmmoEffect::Draw( const gui::GlTools_ABC& tools, const kernel::Profile_ABC& profile ) const
{
    if( IsVisible( profile ) )
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
        meteo.SetLighting( weather::PHY_Lighting::eclairant_ );
    else if( type_ == sword::StartFireEffect::smoke )
        meteo.SetPrecipitation( weather::PHY_Precipitation::smoke_ );
}

// -----------------------------------------------------------------------------
// Name: AmmoEffect::SetPerceivers
// Created: JSR 2013-04-26
// -----------------------------------------------------------------------------
void AmmoEffect::SetPerceivers( const ::google::protobuf::RepeatedPtrField< ::sword::Id >& perceivers )
{
    perceivers_.clear();
    for( auto it = perceivers.begin(); it != perceivers.end(); ++it )
        perceivers_.push_back( it->id() );
    PlaySoundEffect();
}
