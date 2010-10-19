// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LogisticBaseStates.h"
#include "Tools.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Viewport_ABC.h"


using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates constructor
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
LogisticBaseStates::LogisticBaseStates( Controller& controller, Entity_ABC& entity, 
                                       const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, PropertiesDictionary& dico )
    : ::LogisticHierarchies< LogisticBaseSuperior, kernel::LogisticBaseHierarchies >( controller, entity )
    , controller_( controller )
    , resolver_( resolver )
    , item_( 0 )
{
    CreateDictionary( dico, entity );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates destructor
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
LogisticBaseStates::~LogisticBaseStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::CreateDictionary
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
void LogisticBaseStates::CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& )
{
    dico.Register( *(const kernel::LogisticBaseHierarchies*)this, tools::translate( "LogisticBaseStates", "Logistic/LogisticBase/Superior" ), tc2_, *this, &LogisticBaseStates::SetSuperior );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::SetSuperior
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
void LogisticBaseStates::SetSuperior( const LogisticBaseSuperior& formation )
{
    ::LogisticHierarchies< LogisticBaseSuperior, kernel::LogisticBaseHierarchies >::SetSuperior( formation );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::Draw
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
void LogisticBaseStates::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    const bool displayLinks   = tools.ShouldDisplay( "LogisticLinks" );
    const bool displayMissing = tools.ShouldDisplay( "MissingLogisticLinks" ) && viewport.IsHotpointVisible();
    if( ! displayLinks && ! displayMissing )
        return;

    const kernel::Formation_ABC& formation = static_cast< const kernel::Formation_ABC& >( GetEntity() );

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );
    glColor4f( COLOR_MAROON );
    DrawLink( where, tools, 0.5f, displayLinks, displayMissing && formation.GetLogisticLevel()!=kernel::LogisticLevel::none_);
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::Load
// Created: AHC 2010-10-06
// -----------------------------------------------------------------------------
void LogisticBaseStates::Load( xml::xistream& xis )
{
    xis >> xml::optional >> xml::start( "quotas" )
            >> xml::list( "dotation", *this, &LogisticBaseStates::ReadDotation )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::ReadDotation
// Created: AHC 2010-10-06
// -----------------------------------------------------------------------------
void LogisticBaseStates::ReadDotation( xml::xistream& xis )
{
    Dotation* dotation = new Dotation( xis, resolver_ );
    item_->AddDotation( *dotation );
    tools::Resolver< Dotation >::Register( dotation->type_->GetId(), *dotation );
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::SerializeQuotas
// Created: AHC 2010-10-06
// -----------------------------------------------------------------------------
void LogisticBaseStates::SerializeQuotas( xml::xostream& xos ) const
{
    tools::Iterator< const Dotation& > it = tools::Resolver< Dotation >::CreateIterator();
    if( it.HasMoreElements() )
    {
        xos << xml::start( "quotas" );
        while( it.HasMoreElements() )
        {
            xos << xml::start( "dotation" );
            it.NextElement().SerializeAttributes( xos );
            xos << xml::end;
        }
        xos << xml::end;
    }
}
