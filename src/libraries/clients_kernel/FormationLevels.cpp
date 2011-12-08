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
#include "ENT/ENT_Tr.h"
#include <xeumeuleu/xml.hpp>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: FormationLevels constructor
// Created: SBO 2006-09-21
// -----------------------------------------------------------------------------
FormationLevels::FormationLevels()
    : root_( 0 )
{
    for( int i = eNatureLevel_c; i < eNbrNatureLevel; ++i )
        root_ = AddLevel( root_, ENT_Tr::ConvertFromNatureLevel( static_cast< E_NatureLevel >( i ) ).c_str() );
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
Level* FormationLevels::AddLevel( Level* root, const QString& name )
{
    Level* newLevel = new Level( name, root );
    if( root != 0 )
        root->SetPrevious( *newLevel );
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
