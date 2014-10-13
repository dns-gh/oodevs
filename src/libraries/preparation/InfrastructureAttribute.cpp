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
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_gui/DictionaryUpdated.h"
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_gui/PropertiesDictionary.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/UrbanPositions_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/OptionsController.h"
#include "clients_kernel/OptionVariant.h"
#include <xeumeuleu/xml.hpp>

#define DEFAULT_THRESHOLD 30

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
// Name: InfrastructureAttribute constructor
// Created: ABR 2012-06-04
// -----------------------------------------------------------------------------
InfrastructureAttribute::InfrastructureAttribute( kernel::Controllers& controllers, kernel::UrbanObject_ABC& object,
                                                  gui::PropertiesDictionary& dictionary )
    : controllers_      ( controllers )
    , dictionary_       ( dictionary )
    , type_             ( 0 )
    , object_           ( object )
    , enabled_          ( true )
    , threshold_        ( DEFAULT_THRESHOLD )
    , position_         ( object.Get< kernel::UrbanPositions_ABC >().Barycenter() )
    , typeProperty_     ( tools::translate( "Infrastructure", "Info/Infrastructure/Type" ) )
    , enableProperty_   ( tools::translate( "Infrastructure", "Info/Infrastructure/Enable" ) )
    , thresholdProperty_( tools::translate( "Infrastructure", "Info/Infrastructure/Threshold" ) )
{
    UpdateDictionnary();
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute constructor
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
InfrastructureAttribute::InfrastructureAttribute( xml::xistream& xis, kernel::Controllers& controllers,
                                                  kernel::UrbanObject_ABC& object, gui::PropertiesDictionary& dictionary,
                                                  const kernel::ObjectTypes& objectTypes )
    : controllers_      ( controllers )
    , dictionary_       ( dictionary )
    , type_             ( 0 )
    , object_           ( object )
    , enabled_          ( true )
    , threshold_        ( DEFAULT_THRESHOLD )
    , position_         ( object.Get< kernel::UrbanPositions_ABC >().Barycenter() )
    , typeProperty_     ( tools::translate( "Infrastructure", "Info/Infrastructure/Type" ) )
    , enableProperty_   ( tools::translate( "Infrastructure", "Info/Infrastructure/Enable" ) )
    , thresholdProperty_( tools::translate( "Infrastructure", "Info/Infrastructure/Threshold" ) )
{
    std::string type;
    std::string role;
    float threshold = static_cast< float >( threshold_ ) / 100.f;
    xis >> xml::optional
            >> xml::start( "infrastructures" )
                >> xml::start( "infrastructure" )
                    >> xml::optional >> xml::attribute( "type", type )
                    >> xml::optional >> xml::attribute( "role", role )
                    >> xml::optional >> xml::attribute( "enabled", enabled_ )
                    >> xml::optional >> xml::attribute( "threshold", threshold )
                >> xml::end
            >> xml::end;
    threshold_ = static_cast< unsigned int >( 100 * threshold + 0.5f );
    if( type.empty() && !role.empty() )
        type =  role;
    type_ = objectTypes.StringResolver< kernel::InfrastructureType >::Find( type );
    if( type_ )
        object.Get< kernel::UrbanPositions_ABC >().SetInfrastructurePresent( true );
    else
        invalidType_ = type;
    UpdateDictionnary();
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute destructor
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
InfrastructureAttribute::~InfrastructureAttribute()
{
    // NOTHING
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
        xis >> xml::attribute( "enabled", enabled_ )
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
    const_cast< InfrastructureAttribute* >( this )->invalidType_.clear();
    if( type_ )
    {
        xos << xml::start( "infrastructures" )
                << xml::start( "infrastructure" )
                    << xml::attribute( "role", type_->GetName() )
                    << xml::attribute( "enabled", enabled_ )
                    << xml::attribute( "threshold", static_cast< float >( threshold_ ) / 100.f )
                << xml::end
            << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::Draw
// Created: LGY 2013-03-07
// -----------------------------------------------------------------------------
void InfrastructureAttribute::Draw( const geometry::Point2f& /*where*/, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( type_ && viewport.IsHotpointVisible() && controllers_.options_.GetOption( "Infra" ).To< bool >() )
        tools.DrawInfrastructureSymbol( type_->GetSymbol(), position_, 0.1f, 0.1f );
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
// Name: InfrastructureAttribute::GetInvalidType
// Created: JSR 2012-05-14
// -----------------------------------------------------------------------------
const std::string& InfrastructureAttribute::GetInvalidType() const
{
    return invalidType_;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::GetThreshold
// Created: CMA 2011-09-08
// -----------------------------------------------------------------------------
unsigned int InfrastructureAttribute::GetThreshold() const
{
    return threshold_;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::GetType
// Created: ABR 2012-05-25
// -----------------------------------------------------------------------------
const kernel::InfrastructureType* InfrastructureAttribute::GetType() const
{
    return type_;
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::SetType
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::SetType( kernel::InfrastructureType* infrastructure )
{
    invalidType_.clear();
    if( type_ != infrastructure )
    {
        type_ = infrastructure;
        object_.Get< kernel::UrbanPositions_ABC >().SetInfrastructurePresent( type_ != 0 );
        UpdateDictionnary();
        controllers_.controller_.Update( object_ );
    }
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::UpdateDictionnary
// Created: JSR 2012-06-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::UpdateDictionnary()
{
    if( type_ )
    {
        dictionary_.Register( object_, typeProperty_, type_, true );
        dictionary_.Register( object_, enableProperty_, enabled_ );
        dictionary_.Register( object_, thresholdProperty_, threshold_, ThresholdSetter(), false );
    }
    else
    {
        dictionary_.Remove( typeProperty_ );
        dictionary_.Remove( enableProperty_ );
        dictionary_.Remove( thresholdProperty_ );
    }
}

