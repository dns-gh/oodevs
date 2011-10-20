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
#include "IdManager.h"
#include "LogisticBaseStates.h"
#include "clients_gui/Tools.h"
#include "clients_kernel/FormationLevels.h"
#include "clients_kernel/ActionController.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/HierarchyLevel_ABC.h"
#include "clients_kernel/Viewport_ABC.h"
#include "clients_kernel/GlTools_ABC.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/PropertiesDictionary.h"
#include "LogisticLevelAttritube.h"
#include "clients_kernel/App6Symbol.h"
#include "Tools.h"
#include <xeumeuleu/xml.hpp>
#include <QtGui/qmessagebox.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2006-09-19
// -----------------------------------------------------------------------------
Formation::Formation( kernel::Controller& controller, const HierarchyLevel_ABC& level, IdManager& idManager )
    : EntityImplementation< Formation_ABC >( controller, idManager.GetNextId(), "" )
    , level_( &level )
{
    RegisterSelf( *this );
    name_ = tools::translate( "Formation", "Formation [%1]" ).arg( id_ );
    CreateDictionary( controller );
}

// -----------------------------------------------------------------------------
// Name: Formation constructor
// Created: SBO 2006-10-05
// -----------------------------------------------------------------------------
Formation::Formation( xml::xistream& xis, Controller& controller, const FormationLevels& levels, IdManager& idManager )
    : EntityImplementation< Formation_ABC >( controller, 0, "" )
{
    std::string level, name;
    xis >> xml::attribute( "id", ( int& ) id_ )
        >> xml::attribute( "level", level )
        >> xml::attribute( "name", name );
    level_ = levels.Resolve( level.c_str() );
    name_  = name.empty() ? tools::translate( "Formation", "Formation [%1]" ).arg( id_ ) : name.c_str();

    xis >> xml::optional >> xml::attribute( "nature", nature_ )
        >> xml::optional >> xml::attribute( "color", color_ );

    idManager.Lock( id_ );
    RegisterSelf( *this );
    CreateDictionary( controller );
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
    return level_->GetName() + ( name_.isEmpty() ? "" : " - " + name_ );
}

// -----------------------------------------------------------------------------
// Name: Formation::GetLevel
// Created: SBO 2006-09-20
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC& Formation::GetLevel() const
{
    return *level_;
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
void Formation::Draw( const geometry::Point2f& where, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const
{
    if( !IsAggregated( *this ) && HasAggregatedSubordinate() && viewport.IsVisible( where ) )
    {
        InitializeSymbol();
        tools.DrawApp6Symbol( symbolPath_, where, 4 );
        tools.DrawApp6Symbol( levelPath_, where, 4 );
    }
}

// -----------------------------------------------------------------------------
// Name: Formation::Rename
// Created: SBO 2006-10-10
// -----------------------------------------------------------------------------
void Formation::Rename( const QString& name )
{
    const QString prefix = level_->GetName() + " - ";
    name_ = name.startsWith( prefix ) ? name.right( name.length() - prefix.length() ) : name;
    Touch();
}

// -----------------------------------------------------------------------------
// Name: Formation::SerializeAttributes
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void Formation::SerializeAttributes( xml::xostream& xos ) const
{
    xos << xml::attribute( "id", long( id_ ) )
        << xml::attribute( "name", name_.ascii() )
        << xml::attribute( "level", level_->GetName().ascii() );
    if (nature_.length() > 0)
        xos << xml::attribute( "nature", nature_ );
    if (color_.length() > 0)
        xos << xml::attribute( "color", color_ );
}

// -----------------------------------------------------------------------------
// Name: Formation::CreateDictionary
// Created: SBO 2006-11-17
// -----------------------------------------------------------------------------
void Formation::CreateDictionary( kernel::Controller& controller )
{
    PropertiesDictionary& dictionary = *new PropertiesDictionary( controller );
    Attach( dictionary );
    const Formation& constSelf = *this;
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Formation", "Info/Identifier" ), constSelf.id_ );
    dictionary.Register( *(const Entity_ABC*)this, tools::translate( "Formation", "Info/Name" ), name_, *this, &Formation::Rename );
}

// -----------------------------------------------------------------------------
// Name: Formation::SerializeAttributes
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
const kernel::LogisticLevel& Formation::GetLogisticLevel() const
{
    return Get< LogisticLevelAttritube >().GetLogisticLevel();
}

// -----------------------------------------------------------------------------
// Name: Formation::IsAggregated
// Created: LGY 2011-03-10
// -----------------------------------------------------------------------------
bool Formation::IsAggregated( const kernel::Entity_ABC& entity ) const
{
    if( const kernel::Positions* positions = entity.Retrieve< kernel::Positions >() )
        return positions->IsAggregated();
    return false;
}

// -----------------------------------------------------------------------------
// Name: Formation::HasAggregatedSubordinate
// Created: LGY 2011-03-10
// -----------------------------------------------------------------------------
bool Formation::HasAggregatedSubordinate() const
{
    tools::Iterator< const kernel::Entity_ABC& > it = Get< TacticalHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
        return IsAggregated( it.NextElement() );
    return false;
}
