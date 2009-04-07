// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Representations.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"

// -----------------------------------------------------------------------------
// Name: DEC_Representations constructor
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
DEC_Representations::DEC_Representations( MT_RoleContainer& roleContainer )
: MT_Role_ABC( roleContainer )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations destructor
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
DEC_Representations::~DEC_Representations()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::GetCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
const T_ObjectVector& DEC_Representations::GetCategory( const std::string& name )
{
    return representations_[name];
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Representations::AddToCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::AddToCategory( const std::string& name, DIA_TypedObject* pObject )
{
    representations_[name].push_back( pObject );
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::RemoveFromCategory
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::RemoveFromCategory( const std::string& name, DIA_TypedObject* pObject )
{
    T_ObjectVector& representation = representations_[name];
    for( IT_ObjectVector it = representation.begin(); it != representation.end(); ++it )
    {
        if( *it == pObject )
        {
            representation.erase( it );
            break;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::DeleteRepresentation
// Created: LDC 2009-04-03
// -----------------------------------------------------------------------------
void DEC_Representations::DeleteRepresentation( DIA_TypedObject* pObject )
{
    for( IT_Representations it = representations_.begin(); it != representations_.end(); ++it )
    {
        for( IT_ObjectVector oit = it->second.begin(); oit != it->second.end(); ++oit )
        {
            if( *oit == pObject )
            {
                it->second.erase( oit );
                delete pObject;
                return;
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Representations::RemoveAllReferencesOf
// Created: LDC 2009-04-07
// -----------------------------------------------------------------------------
void DEC_Representations::RemoveAllReferencesOf( const DIA_TypedObject& referenced )
{
    DEC_RolePion_Decision& role = GetRole<DEC_RolePion_Decision>();
    role.RemoveAllReferencesOf( referenced, role.GetContext() );
}
