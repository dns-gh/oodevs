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
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationLevels constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationLevels::FormationLevels()
    : root_( 0 )
{
    root_ = new Level( "b", 0 );
    Register( root_->GetId(), root_->GetName().toStdString(), *root_ );

    root_ = AddLevel( *root_, "o" );
    root_ = AddLevel( *root_, "c" );
    root_ = AddLevel( *root_, "oo" );
    root_ = AddLevel( *root_, "ooo" );
    root_ = AddLevel( *root_, "i" );
    root_ = AddLevel( *root_, "ii" );
    root_ = AddLevel( *root_, "iii" );
    root_ = AddLevel( *root_, "x" );
    root_ = AddLevel( *root_, "xx" );
    root_ = AddLevel( *root_, "xxx" );
    root_ = AddLevel( *root_, "xxxx" );
    root_ = AddLevel( *root_, "xxxxx" );}

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
Level* FormationLevels::AddLevel( Level& root, const QString& name )
{
    Level* newLevel = new Level( name, &root );
    root.SetPrevious( *newLevel );
    Register( newLevel->GetId(), newLevel->GetName().toStdString(), *newLevel );
    return newLevel;
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
    return Find( name.toStdString() );
}

// -----------------------------------------------------------------------------
// Name: FormationLevels::Resolve
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC* FormationLevels::Resolve( unsigned int id ) const
{
    return Find( id );
}
