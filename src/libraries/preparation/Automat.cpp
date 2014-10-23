// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "Automat.h"
#include "tools/IdManager.h"
#include "clients_gui/GLOptions.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/Viewport_ABC.h"
#include "clients_kernel/App6Symbol.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/Controller.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_gui/EntityType.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/Positions.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Tools.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::Automat( const AutomatType& type, Controller& controller, tools::IdManager& idManager, const QString& name )
    : EntityImplementation< Automat_ABC >( controller, idManager.GetNextId(), "" )
    , type_( type )
{
    name_ = name.isEmpty() ? type.GetName().c_str() : name;
    AddExtension( *this );
}

// -----------------------------------------------------------------------------
// Name: Automat constructor
// Created: SBO 2006-10-09
// -----------------------------------------------------------------------------
Automat::Automat( xml::xistream& xis, Controller& controller, tools::IdManager& idManager, const AutomatType& type )
    : EntityImplementation< Automat_ABC >( controller, xis.attribute< unsigned long >( "id" ), xis.attribute< std::string >( "name" ).c_str() )
    , type_ ( type )
{
    AddExtension( *this );
    idManager.Lock( id_ );
}

// -----------------------------------------------------------------------------
// Name: Automat destructor
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
Automat::~Automat()
{
    Destroy();
}

// -----------------------------------------------------------------------------
// Name: Automat::Draw
// Created: AGE 2006-10-06
// -----------------------------------------------------------------------------
void Automat::Draw( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& view ) const
{
    if( !IsAnAggregatedSubordinate() && view.GetOptions().IsAggregated( *this ) && viewport.IsVisible( where ) )
    {
        InitializeSymbol();
        view.DrawApp6SymbolFixedSize( symbol_, where, -1.5f, 0 );
        view.DrawApp6SymbolFixedSize( level_, where, -1.5f, 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: Automat::Pick
// Created: LGY 2013-02-20
// -----------------------------------------------------------------------------
void Automat::Pick( const geometry::Point2f& where, const gui::Viewport_ABC& viewport, gui::GLView_ABC& tools ) const
{
    Draw( where, viewport, tools );
}

// -----------------------------------------------------------------------------
// Name: Automat::InitializeSymbol
// Created: AGE 2006-10-25
// -----------------------------------------------------------------------------
void Automat::InitializeSymbol() const
{
    const kernel::TacticalHierarchies& hierarchies = Get< kernel::TacticalHierarchies >();
    const std::string symbol = hierarchies.GetSymbol();
    const std::string level = hierarchies.GetLevel();
    if( symbol_ == symbol && level_ == level )
        return;
    symbol_ = symbol;
    level_ = level;
    kernel::App6Symbol::SetKarma( symbol_, hierarchies.GetTop().Get< kernel::Diplomacies_ABC >().GetKarma() );
}

// -----------------------------------------------------------------------------
// Name: Automat::GetType
// Created: AHC 2010-10-07
// -----------------------------------------------------------------------------
const AutomatType& Automat::GetType() const
{
    return type_;
}
