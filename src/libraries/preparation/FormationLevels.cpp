// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_pch.h"
#include "FormationLevels.h"
#include "Level.h"

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationLevels constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationLevels::FormationLevels()
    : root_( 0 )
{
    Initialize();
}
    
// -----------------------------------------------------------------------------
// Name: FormationLevels destructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationLevels::~FormationLevels()
{
    // NOTHING
}

namespace 
{
    HierarchyLevel_ABC* AddLevel( HierarchyLevel_ABC& root, const QString& name )
    {
        HierarchyLevel_ABC* newLevel = new Level( name, &root );
        root.SetNext( *newLevel );
        return newLevel;
    }
}

// -----------------------------------------------------------------------------
// Name: FormationLevels::Initialize
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
void FormationLevels::Initialize()
{
    HierarchyLevel_ABC* root = new Level( "xxxx", 0 );
    root_ = root;
    root = AddLevel( *root, "xxx" );
    root = AddLevel( *root, "xx" );
    root = AddLevel( *root, "x" );
    root = AddLevel( *root, "iii" );
    root = AddLevel( *root, "ii" );
    root = AddLevel( *root, "i" );
    root = AddLevel( *root, "ooo" );
    root = AddLevel( *root, "oo" );
    root = AddLevel( *root, "o" );
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
    const HierarchyLevel_ABC* root = root_;
    while( root )
    {
        if( root->GetName() == name )
            return root;
        root = root->GetNext();
    }
    return root;
}

// -----------------------------------------------------------------------------
// Name: FormationLevels::Resolve
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
const HierarchyLevel_ABC* FormationLevels::Resolve( unsigned int id ) const
{
    const HierarchyLevel_ABC* root = root_;
    while( root )
    {
        if( root->GetId() == id )
            return root;
        root = root->GetNext();
    }
    return root;
}
