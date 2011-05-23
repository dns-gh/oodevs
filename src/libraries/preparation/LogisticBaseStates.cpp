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
#include "Dotation.h"
#include "DotationsItem.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Positions.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates constructor
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
LogisticBaseStates::LogisticBaseStates( Controller& controller, Entity_ABC& entity,
                                       const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, PropertiesDictionary& dico, bool canHaveQuotas )
    : kernel::EntityHierarchies< kernel::LogisticHierarchiesBase >( controller, entity, 0 )
    , controller_( controller )
    , resolver_( resolver )
    , item_( 0 )
    , superior_( 0 )
    , canHaveQuotas_( canHaveQuotas )
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
void LogisticBaseStates::CreateDictionary( kernel::PropertiesDictionary& dico, kernel::Entity_ABC& entity )
{    
    dico.Register( *(const kernel::LogisticHierarchiesBase*)this, tools::translate( "LogisticBaseStates", "Logistic/LogisticBase/Superior" ), superior_, *this, &LogisticBaseStates::SetSuperior );
    if( canHaveQuotas_ )
    {
        item_ = new DotationsItem( controller_, entity, dico, tools::translate( "LogisticBaseStates", "Logistic/LogisticBase/Quotas" ), *(Resolver< Dotation >*)this );
        dico.Register( entity, tools::translate( "LogisticBaseStates", "Logistic/LogisticBase/Quotas" ), item_ );    
    }
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
// Name: LogisticHierarchies::Load
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
void LogisticBaseStates::Load( xml::xistream& xis, const kernel::Entity_ABC* superior )
{
    SetSuperior( superior );
    xis >> xml::optional >> xml::start( "quotas" )
            >> xml::list( "resource", *this, &LogisticBaseStates::ReadDotation )
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
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SetSuperiorInternal
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LogisticBaseStates::SetSuperiorInternal( kernel::Entity_ABC* superior )
{
    superior_ = superior;
    kernel::EntityHierarchies< kernel::LogisticHierarchiesBase >::SetSuperiorInternal( superior );
}

// -----------------------------------------------------------------------------
// Name: MaintenanceStates::SetSuperior&
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
void LogisticBaseStates::SetSuperior( const LogisticBaseSuperior& superior )
{
    const kernel::Entity_ABC* tmp = superior;
    kernel::EntityHierarchies< kernel::LogisticHierarchiesBase >::SetSuperior( const_cast< kernel::Entity_ABC* >( tmp ) );
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::DrawLink
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void LogisticBaseStates::DrawLink( const geometry::Point2f& where, const kernel::GlTools_ABC& tools, float curve, bool displayLinks, bool displayMissings ) const
{
    if( superior_ && displayLinks )
        tools.DrawCurvedArrow( where, superior_->Get< kernel::Positions >().GetPosition(), curve );
    else if( ! superior_ && displayMissings )
        tools.DrawCircle( geometry::Point2f( where.X(), where.Y() + 150 ), 300.0 );
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
            xos << xml::start( "resource" );
            it.NextElement().SerializeAttributes( xos );
            xos << xml::end;
        }
        xos << xml::end;
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticHierarchies::SerializeLogistics
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LogisticBaseStates::SerializeLogistics( xml::xostream& xos ) const
{    
    tools::Iterator< const kernel::Entity_ABC& > children = CreateSubordinateIterator();
    if( !children.HasMoreElements() )
        return;

    xos << xml::start( GetLinkType().ascii() )
            << xml::attribute( "id", GetEntity().GetId());
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = children.NextElement();
        xos << xml::start( "subordinate" )
                << xml::attribute( "id", entity.GetId());

        //$$$ Ca me parait bien compliqué et bien foireux...
        const LogisticBaseStates* subordinateLogHierarchy = dynamic_cast< const LogisticBaseStates* >( entity.Retrieve< kernel::LogisticHierarchiesBase >() );
        if( subordinateLogHierarchy )
            subordinateLogHierarchy->SerializeQuotas( xos );
        xos << xml::end;
    }
    xos << xml::end;
}
