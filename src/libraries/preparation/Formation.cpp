// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Formation.h"
#include "FormationHierarchies.h"
#include "tools/IdManager.h"
#include "LogisticBaseStates.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/AggregatedTools.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
Formation::Formation( kernel::Controller& controller, E_NatureLevel level, tools::IdManager& idManager )
    : EntityImplementation< Formation_ABC >( controller, idManager.GetNextId(), "" )
    , level_( level )
{
    AddExtension( *this );
    name_ = tools::translate( "Formation", "Formation [%L1]" ).arg( id_ );
}

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Formation::Formation( xml::xistream& xis, Controller& controller, tools::IdManager& idManager )
    : EntityImplementation< Formation_ABC >( controller, 0, "" )
{
    std::string level, name;
    xis >> xml::attribute( "id", ( int& ) id_ )
        >> xml::attribute( "level", level )
        >> xml::attribute( "name", name );
    level_ = ENT_Tr::ConvertToNatureLevel( level );
    name_  = name.empty() ? tools::translate( "Formation", "Formation [%L1]" ).arg( id_ ) : name.c_str();

    idManager.Lock( id_ );
    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: Formation destructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
Formation::~Formation()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Formation::GetName
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
QString Formation::GetName() const
{
    return QString( "%1%2%3" ).arg( ENT_Tr::ConvertFromNatureLevel( level_ ).c_str() ).arg( name_.isEmpty() ? "" : " - " ).arg( name_ );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetBaseName
// Created: MMC 2012-01-11
// -----------------------------------------------------------------------------
QString Formation::GetBasicName() const
{
    return name_;
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
E_NatureLevel Formation::GetLevel() const
{
    return level_;
}

// -----------------------------------------------------------------------------
// Name: Formation::InitializeSymbol
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
void Formation::InitializeSymbol() const
{
    const kernel::TacticalHierarchies& hierarchies = Get< kernel::TacticalHierarchies >();
    const std::string symbol = hierarchies.GetSymbol();
    const std::string level = hierarchies.GetLevel();
    if( symbolPath_ == symbol && levelPath_ == level )
        return;
    symbolPath_ = symbol;
    levelPath_ = level;
    kernel::App6Symbol::SetKarma( symbolPath_, hierarchies.GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
}

// -----------------------------------------------------------------------------
// Name: Formation::Draw
// Created: LGY 2011-03-04
// -----------------------------------------------------------------------------
void Formation::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    if( !IsAggregated() && HasAggregatedSubordinate( *this ) && viewport.IsVisible( where ) )
    {
        InitializeSymbol();
        tools.DrawApp6SymbolFixedSize( symbolPath_, where, -2.f, 0 );
        tools.DrawApp6SymbolFixedSize( levelPath_, where, -2.f, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: Formation::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void Formation::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Formation::Rename
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Formation::Rename( const QString& name )
{
    const QString prefix = QString( "%1 - ").arg( ENT_Tr::ConvertFromNatureLevel( level_ ).c_str() );
    gui::EntityImplementation< kernel::Formation_ABC >::Rename( name.startsWith( prefix ) ? name.right( name.length() - prefix.length() ) : name );
}

// -----------------------------------------------------------------------------
// Name: Formation::SetLevelName
// Created: MMC 2012-01-05
// -----------------------------------------------------------------------------
void Formation::SetLevel( E_NatureLevel level )
{
    kernel::TacticalHierarchies& hierarchies = Get< kernel::TacticalHierarchies >();
    FormationHierarchies* pFormationHierachies = dynamic_cast< FormationHierarchies* >( &hierarchies );
    if( pFormationHierachies )
    {
        level_ = level;
        Rename( name_ );
        pFormationHierachies->SetLevel( level_ );
        InitializeSymbol();
    }
}

// -----------------------------------------------------------------------------
// Name: Formation::SerializeAttributes
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void Formation::SerializeAttributes( xml::xostream& xos ) const
{
    gui::EntityImplementation< kernel::Formation_ABC >::SerializeAttributes( xos );
    xos << xml::attribute( "level", ENT_Tr::ConvertFromNatureLevel( level_ ) );
}
