// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "FormationLevels.h"
#include "Level.h"
#include "SymbolFactory.h"
#include <xeumeuleu/xml.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationLevels constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationLevels::FormationLevels()
    : root_( 0 )
{
    SymbolFactory factory;
    Initialize( factory );
}
    
// -----------------------------------------------------------------------------
// Name: FormationLevels destructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationLevels::~FormationLevels()
{
    DeleteAll();
}

// -----------------------------------------------------------------------------
// Name: FormationLevels::AddLevel
// Created: AGE 2006-10-19
// -----------------------------------------------------------------------------
Level* FormationLevels::AddLevel( SymbolFactory& factory, Level& root, const QString& name )
{
    Level* newLevel = new Level( factory, name, &root );
    root.SetPrevious( *newLevel );
    Register( newLevel->GetId(), newLevel->GetName(), *newLevel );
    return newLevel;
}

// -----------------------------------------------------------------------------
// Name: FormationLevels::Initialize
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void FormationLevels::Initialize( SymbolFactory& factory )
{
    root_ = new Level( factory, "o", 0 );
    Register( root_->GetId(), root_->GetName(), *root_ );

    root_ = AddLevel( factory, *root_, "oo" );
    root_ = AddLevel( factory, *root_, "ooo" );
    root_ = AddLevel( factory, *root_, "i" );
    root_ = AddLevel( factory, *root_, "ii" );
    root_ = AddLevel( factory, *root_, "iii" );
    root_ = AddLevel( factory, *root_, "x" );
    root_ = AddLevel( factory, *root_, "xx" );
    root_ = AddLevel( factory, *root_, "xxx" );
    root_ = AddLevel( factory, *root_, "xxxx" );
    root_ = AddLevel( factory, *root_, "xxxxx" );
}

// -----------------------------------------------------------------------------
// Name: FormationLevels::GetRoot
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC* FormationLevels::GetRoot() const
{
    return root_;
}

// -----------------------------------------------------------------------------
// Name: FormationLevels::Resolve
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC* FormationLevels::Resolve( const QString& name ) const
{
    return Find( name );
}

// -----------------------------------------------------------------------------
// Name: FormationLevels::Resolve
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC* FormationLevels::Resolve( unsigned int id ) const
{
    return Find( id );
}
