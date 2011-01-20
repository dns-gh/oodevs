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
#include "clients_kernel/Displayer_ABC.h"
#include "clients_kernel/InfrastructureType.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_gui/TerrainObjectProxy.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute constructor
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
InfrastructureAttribute::InfrastructureAttribute( const gui::TerrainObjectProxy& object, const kernel::InfrastructureType& infrastructureType, kernel::PropertiesDictionary& dico )
    : enabled_  ( true )
    , threshold_ ( 30 )
    , type_ ( infrastructureType )
    , object_( object )
{
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute constructor
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
InfrastructureAttribute::InfrastructureAttribute( const gui::TerrainObjectProxy& object, xml::xistream& xis, const kernel::InfrastructureType& infrastructureType, kernel::PropertiesDictionary& dico )
    : enabled_  ( true )
    , threshold_ ( 30 )
    , type_ ( infrastructureType )
    , object_( object )
{
    xis >> xml::attribute( "enable", enabled_ )
        >> xml::attribute( "threshold", threshold_ );
    CreateDictionary( dico );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute destructor
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
InfrastructureAttribute::~InfrastructureAttribute()
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::Update
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::Update( xml::xistream& xis )
{
    xis >> xml::attribute( "role", role_ )
        >> xml::attribute( "enabled", enabled_ )
        >> xml::attribute( "threshold", threshold_ );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::Display
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::Display( Displayer_ABC& displayer ) const
{
    displayer.Group( tools::translate( "Infrastructure", "Infrastructure" ) )
             .Display( tools::translate( "Infrastructure", "Enable:" ), enabled_ );
    displayer.Group( tools::translate( "Infrastructure", "Infrastructure" ) )
             .Display( tools::translate( "Infrastructure", "Threshold:" ), threshold_ );
}

// -----------------------------------------------------------------------------
// Name: ConstructionAttribute::DisplayInTooltip
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::DisplayInTooltip( Displayer_ABC& displayer ) const
{
    Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::SerializeAttributes
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::start( "infrastructure" )
        << xml::attribute( "role", role_ )
        << xml::attribute( "enabled", enabled_ )
        << xml::attribute( "threshold", threshold_ );
    xos << xml::end;

}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::CreateDictionary
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::CreateDictionary( kernel::PropertiesDictionary& dico )
{
    dico.Register( *this, tools::translate( "Infrastructure", "Info/Infrastructure/Type" ), type_.GetName() );
    dico.Register( *this, tools::translate( "Infrastructure", "Info/Infrastructure/Enable" ), enabled_ );
    dico.Register( *this, tools::translate( "Infrastructure", "Info/Infrastructure/Threshold" ), threshold_ );
}

// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::Draw
// Created: SLG 2011-01-11
// -----------------------------------------------------------------------------
void InfrastructureAttribute::Draw( const kernel::Viewport_ABC& /*viewport*/, const kernel::GlTools_ABC& tools ) const
{
    tools.DrawApp6Symbol( type_.GetSymbol(), object_.Barycenter() , 0.1f, 1.f );
}


// -----------------------------------------------------------------------------
// Name: InfrastructureAttribute::IsEnabled
// Created: SLG 2011-01-20
// -----------------------------------------------------------------------------
bool InfrastructureAttribute::IsEnabled() const
{
    return enabled_;
}
