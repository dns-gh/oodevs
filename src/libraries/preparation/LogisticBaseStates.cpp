// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LogisticBaseStates.h"
#include "clients_kernel/Tools.h"
#include "Dotation.h"
#include "DotationsItem.h"

#include "clients_gui/GlTools_ABC.h"
#include "clients_gui/LogisticBase.h"
#include "clients_gui/LogisticHelpers.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Controllers.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Positions.h"
#include "MT_Tools/MT_Logger.h"

#include <xeumeuleu/xml.hpp>

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates constructor
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
LogisticBaseStates::LogisticBaseStates( kernel::Controller& controller, kernel::Entity_ABC& entity,
                                        const tools::Resolver_ABC< kernel::DotationType, std::string >& resolver, gui::PropertiesDictionary& dico, bool canHaveQuotas )
    : gui::EntityHierarchies< gui::LogisticHierarchiesBase >( controller, entity, 0 )
    , controller_   ( controller )
    , entity_       ( entity )
    , resolver_     ( resolver )
    , item_         ( 0 )
    , superior_     ( 0 )
    , canHaveQuotas_( canHaveQuotas )
{
    CreateDictionary( dico, entity );
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates destructor
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
LogisticBaseStates::~LogisticBaseStates()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::CreateDictionary
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
void LogisticBaseStates::CreateDictionary( gui::PropertiesDictionary& dico, kernel::Entity_ABC& entity )
{
    dico.RegisterExtension( entity, static_cast< const gui::LogisticHierarchiesBase* >( this ), tools::translate( "LogisticBaseStates", "Logistic/LogisticBase/Superior" ), superior_, *this, &LogisticBaseStates::SetLogisticSuperior );
    if( canHaveQuotas_ )
    {
        item_ = new DotationsItem( controller_, entity, dico, tools::translate( "LogisticBaseStates", "Logistic/LogisticBase/Quotas" ), *static_cast< Resolver< Dotation >* >( this ), false );
        dico.Register( entity, tools::translate( "LogisticBaseStates", "Logistic/LogisticBase/Edit Quotas" ), item_ );
    }
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::Draw
// Created: AHC 2010-09-29
// -----------------------------------------------------------------------------
void LogisticBaseStates::Draw( const geometry::Point2f& , const gui::Viewport_ABC& viewport, gui::GlTools_ABC& tools ) const
{
    const bool displayLinks   = tools.ShouldDisplay( "LogisticLinks" );
    const bool displayMissing = tools.ShouldDisplay( "MissingLogisticLinks" ) && viewport.IsHotpointVisible();
    if( ! displayLinks && ! displayMissing )
        return;

    glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
    glLineWidth( 3.f );
    glColor4f( COLOR_YELLOW );
    DrawLink( logistic_helpers::GetLogisticPosition( entity_ ), tools, 0.5f, displayLinks, displayMissing );
    glPopAttrib();
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::ClearDotations
// Created: MMC 2012-03-23
// -----------------------------------------------------------------------------
void LogisticBaseStates::ClearDotations()
{
    item_->Clear();
    item_->Update();
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::SetDotation
// Created: MMC 2012-03-23
// -----------------------------------------------------------------------------
void LogisticBaseStates::SetDotation( const kernel::DotationType& type, unsigned int quantity )
{
    if( !item_ )
        return;

    Dotation* dotation = tools::Resolver< Dotation >::Find( type.GetId() );
    if( !dotation )
    {
        if( quantity > 0 )
        {
            dotation = new Dotation( type, quantity );
            item_->AddDotation( *dotation );
            tools::Resolver< Dotation >::Register( type.GetId(), *dotation );
        }
    }
    else
    {
        dotation->quantity_ = quantity;
        if( quantity == 0 )
        {
            item_->RemoveDotation( type );
            tools::Resolver< Dotation >::Remove( type.GetId() );
        }
    }
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::Load
// Created: SBO 2006-11-16
// -----------------------------------------------------------------------------
void LogisticBaseStates::Load( xml::xistream& xis, const kernel::Entity_ABC* superior )
{
    SetLogisticSuperior( superior );
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
    if( item_ )
    {
        Dotation* dotation = new Dotation( xis, resolver_ );
        item_->AddDotation( *dotation );
        tools::Resolver< Dotation >::Register( dotation->type_.GetId(), *dotation );
        controller_.Update( *this );
    }
    else
        MT_LOG_ERROR_MSG( "Defining quotas on automat which doesn't allow them." );
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::SetSuperiorInternal
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LogisticBaseStates::SetSuperiorInternal( kernel::Entity_ABC* superior )
{
    superior_ = superior;
    gui::EntityHierarchies< gui::LogisticHierarchiesBase >::SetSuperiorInternal( superior );
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::SetLogisticSuperior
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LogisticBaseStates::SetLogisticSuperior( const kernel::LogisticBaseSuperior& superior )
{
    const kernel::Entity_ABC* tmp = superior;
    gui::EntityHierarchies< gui::LogisticHierarchiesBase >::SetSuperior( const_cast< kernel::Entity_ABC* >( tmp ) );
    controller_.Update( *this );
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::const
// Created: LGY 2012-03-22
// -----------------------------------------------------------------------------
bool LogisticBaseStates::HasMissingLogisticLinks() const
{
    if( entity_.GetTypeName() == kernel::Automat_ABC::typeName_ && !superior_ )
        return true;
    if( const gui::LogisticBase* attribute = entity_.Retrieve< gui::LogisticBase >() )
        return attribute->IsBase() && !superior_;
    return false;
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::DrawLink
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void LogisticBaseStates::DrawLink( const geometry::Point2f& where, const gui::GlTools_ABC& tools, float curve, bool displayLinks, bool displayMissings ) const
{
    if( superior_ && displayLinks )
    {
        const kernel::Entity_ABC* superior = superior_;
        tools.DrawCurvedArrow( where, logistic_helpers::GetLogisticPosition( *superior ), curve );
    }
    else if( HasMissingLogisticLinks() && displayMissings )
        tools.DrawCircle( geometry::Point2f( where.X(), where.Y() + 150 ), 300.0 );
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::SerializeQuotas
// Created: AHC 2010-10-06
// -----------------------------------------------------------------------------
void LogisticBaseStates::SerializeQuotas( xml::xostream& xos ) const
{
    if( !HasQuotas() )
        return;

    xos << xml::start( "quotas" );
    tools::Iterator< const Dotation& > it = tools::Resolver< Dotation >::CreateIterator();
    while( it.HasMoreElements() )
    {
        const Dotation& dotation = it.NextElement();
        if( dotation.quantity_ > 0 )
        {
            xos << xml::start( "resource" );
            dotation.SerializeAttributes( xos );
            xos << xml::end;
        }
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::CleanBadSubordinates
// Created: LDC 2012-09-13
// -----------------------------------------------------------------------------
bool LogisticBaseStates::CleanBadSubordinates()
{
    tools::Iterator< const kernel::Entity_ABC& > it = CreateSubordinateIterator();
    std::vector< const kernel::Entity_ABC* > badEntities;
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = it.NextElement();
        const kernel::Formation_ABC* formation = dynamic_cast< const kernel::Formation_ABC* >( &entity );
        if( formation && !formation->Get< gui::LogisticBase >().IsBase() )
            badEntities.push_back( &entity );
    }
    for( std::vector< const kernel::Entity_ABC* >::const_iterator it = badEntities.begin(); it != badEntities.end(); ++it )
    {
        LogisticBaseStates* subordinateHierarchy = dynamic_cast< LogisticBaseStates* >( const_cast< gui::LogisticHierarchiesBase* >( (*it)->Retrieve< gui::LogisticHierarchiesBase >() ) );
        if( subordinateHierarchy )
            subordinateHierarchy->ChangeSuperior( 0 );
        else
            RemoveSubordinate( **it );
    }
    return !badEntities.empty();
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::SerializeLogistics
// Created: AGE 2006-11-21
// -----------------------------------------------------------------------------
void LogisticBaseStates::SerializeLogistics( xml::xostream& xos ) const
{
    tools::Iterator< const kernel::Entity_ABC& > children = CreateSubordinateIterator();
    if( !children.HasMoreElements() || dynamic_cast< const kernel::Ghost_ABC* >( &GetEntity() ) )
        return;

    xos << xml::start( GetLinkType().toStdString() )
            << xml::attribute( "id", GetEntity().GetId());
    while( children.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = children.NextElement();
        if( dynamic_cast< const kernel::Ghost_ABC* >( &entity ) )
            continue;
        xos << xml::start( "subordinate" )
                << xml::attribute( "id", entity.GetId());

        //$$$ Ca me parait bien compliqu� et bien foireux... // $$$$ ABR 2012-06-25: Ca l'est... si pas de superieur les quotas ne sont pas enregistr�s -> pertes d'information.
        const LogisticBaseStates* subordinateLogHierarchy = dynamic_cast< const LogisticBaseStates* >( entity.Retrieve< gui::LogisticHierarchiesBase >() );
        if( subordinateLogHierarchy )
            subordinateLogHierarchy->SerializeQuotas( xos );
        xos << xml::end;
    }
    xos << xml::end;
}

// -----------------------------------------------------------------------------
// Name: LogisticBaseStates::HasQuotas
// Created: MMC 2012-03-23
// -----------------------------------------------------------------------------
bool LogisticBaseStates::HasQuotas() const
{
    tools::Iterator< const Dotation& > it = tools::Resolver< Dotation >::CreateIterator();
    while( it.HasMoreElements() )
        if( it.NextElement().quantity_ > 0 )
            return true;
    return false;
}