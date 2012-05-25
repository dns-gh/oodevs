// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "InfrastructureAttribute.h"
#include "MedicalTreatmentAttribute.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/ModeController_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include <xeumeuleu/xml.hpp>

#define DEFAULT_THRESHOLD 30

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute constructor
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
InfrastructureAttribute::InfrastructureAttribute( xml::xistream& xis, kernel::Controllers& controllers, kernel::UrbanObject_ABC& object, kernel::PropertiesDictionary& dico, const kernel::ObjectTypes& objectTypes )
    : controllers_( controllers )
    , type_       ( 0 )
    , enabled_    ( true )
    , threshold_  ( DEFAULT_THRESHOLD )
    , position_   ( object.Get< kernel::UrbanPositions_ABC >().Barycenter() )
{
    std::string type;
    xis >> xml::optional
            >> xml::start( "infrastructures" )
                >> xml::start( "infrastructure" )
                    >> xml::attribute( "type", type )
                >> xml::end
            >> xml::end;
    type_ = objectTypes.StringResolver< kernel::InfrastructureType >::Find( type );
    if( type_ )
    {
        role_ = type_->GetName();
        CreateDictionary( dico );
        //if( type_->FindCapacity( "medical" ) )
        //    object.Attach< kernel::MedicalTreatmentAttribute_ABC >( *new MedicalTreatmentAttribute( objectTypes, dico ) );
        object.Get< kernel::UrbanPositions_ABC >().SetInfrastructurePresent();
    }
    assert( controllers_.modes_ );
    controllers_.modes_->Register( *this );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute destructor
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
InfrastructureAttribute::~InfrastructureAttribute()
{
    assert( controllers_.modes_ );
    controllers_.modes_->Unregister( *this );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::Update
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::Update( xml::xistream& xis )
{
    if( type_ )
    {
        float threshold;
        xis >> xml::attribute( "role", role_ )
            >> xml::attribute( "enabled", enabled_ )
            >> xml::attribute( "threshold", threshold );
        threshold_ = static_cast< unsigned int >( 100 * threshold + 0.5f );
    }
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::Display
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::Display( kernel::Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Infrastructure", "Infrastructure" ) )
             .Display( tools::translate( "Infrastructure", "Enable:" ), enabled_ );
    displayer.Group( tools::translate( "Infrastructure", "Infrastructure" ) )
             .Display( tools::translate( "Infrastructure", "Threshold:" ), threshold_ );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::DisplayInTooltip
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::DisplayInTooltip( kernel::Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::SerializeAttributes
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    if( GetCurrentMode() == ePreparationMode_Exercise && type_ && IsOverriden() )
    {
        xos << xml::start( "infrastructure" )
                << xml::attribute( "role", type_->GetName() )
                << xml::attribute( "enabled", enabled_ )
                << xml::attribute( "threshold", static_cast< float >( threshold_ ) / 100.f )
            << xml::end;
    }
    else if( GetCurrentMode() == ePreparationMode_Terrain && type_ && enabled_ == true )
    {
        xos << xml::start( "infrastructures" )
                << xml::start( "infrastructure" )
                    << xml::attribute( "type", type_->GetName() )
                << xml::end
            << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::SetOverriden
// Created: JSR 2011-02-11
// -----------------------------------------------------------------------------
bool InfrastructureAttribute::IsOverriden() const
{
    return enabled_ != true || threshold_ != DEFAULT_THRESHOLD;
}

namespace
{
    struct ThresholdSetter
    {
        void operator()( unsigned int* pValue, unsigned int value )
        {
            *pValue = std::min( 100u, std::max( 0u, value ) );
        }
    };
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::CreateDictionary
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "Infrastructure", "Info/Infrastructure/Type" ), type_->GetName() );
    dico.Register( *this, tools::translate( "Infrastructure", "Info/Infrastructure/Enable" ), enabled_ );
    dico.Register( *this, tools::translate( "Infrastructure", "Info/Infrastructure/Threshold" ), threshold_, ThresholdSetter() );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::Draw
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::Draw( const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( type_ && viewport.IsHotpointVisible() )
        tools.DrawApp6Symbol( type_->GetSymbol(), position_, 0.1f, 0.1f );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::IsEnabled
// Created: SLG 2011-01-20
// -----------------------------------------------------------------------------
bool InfrastructureAttribute::IsEnabled() const
{
    return enabled_;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::HasValidType
// Created: JSR 2012-05-14
// -----------------------------------------------------------------------------
bool InfrastructureAttribute::HasValidType() const
{
    return type_ != 0;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::GetThreshold
// Created: CMA 2011-09-08
// -----------------------------------------------------------------------------
unsigned int InfrastructureAttribute::GetThreshold() const
{
    return threshold_;
}
