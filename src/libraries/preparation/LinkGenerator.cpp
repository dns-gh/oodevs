// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "LinkGenerator.h"
#include "LogisticHierarchiesBase.h"
#include "LogisticLevelAttritube.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "tools/Iterator.h"
#include <boost/bind.hpp>

// -----------------------------------------------------------------------------
// Name: LinkGenerator constructor
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
LinkGenerator::LinkGenerator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkGenerator destructor
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
LinkGenerator::~LinkGenerator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LinkGenerator::GenerateFromAutomat
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LinkGenerator::GenerateFromAutomat( const kernel::Entity_ABC& automat )
{
    const kernel::Entity_ABC& formation = automat.Get< kernel::TacticalHierarchies >().GetUp();
    CreateLink( formation, automat, !boost::bind( &LinkGenerator::IsLogisticBase, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: LinkGenerator::GenerateFromFormation
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LinkGenerator::GenerateFromFormation( const kernel::Entity_ABC& formation )
{
    CreateLink( formation, formation, boost::bind( &LinkGenerator::IsLogisticBase, this, _1 ) );
    const kernel::Entity_ABC& superior = formation.Get< kernel::TacticalHierarchies >().GetUp();
    if( !IsLogisticBase( superior ) )
    {
        tools::Iterator< const kernel::Entity_ABC& > children = superior.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
        while( children.HasMoreElements() )
        {
            kernel::Entity_ABC& child = const_cast< kernel::Entity_ABC& >( children.NextElement() );
            if( !LinkGenerator::IsLogisticBase( child ) && child.GetId() != formation.GetId() )
                CreateLink( child, formation, boost::bind( &LinkGenerator::IsLogisticBase, this, _1 ) );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: LinkGenerator::Generate
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LinkGenerator::Generate( const kernel::Entity_ABC& entity )
{
    tools::Iterator< const kernel::Entity_ABC& > children = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        kernel::Entity_ABC& child = const_cast< kernel::Entity_ABC& >( children.NextElement() );
        if( child.GetTypeName() == kernel::Automat_ABC::typeName_ && IsLogisticBase( child ) )
            GenerateFromAutomat( child );
        if( child.GetTypeName() == kernel::Formation_ABC::typeName_ && IsLogisticBase( child ) )
            GenerateFromFormation( child );
        Generate( child );
    }
}

// -----------------------------------------------------------------------------
// Name: LinkGenerator::CreateLink
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LinkGenerator::CreateLink( const kernel::Entity_ABC& entity, const kernel::Entity_ABC& base, boost::function< bool( const kernel::Entity_ABC& ) > fun )
{
    tools::Iterator< const kernel::Entity_ABC& > children = entity.Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
    while( children.HasMoreElements() )
    {
        kernel::Entity_ABC& child = const_cast< kernel::Entity_ABC& >( children.NextElement() );
        if( fun( child ) && child.GetId() != base.GetId() )
            SetLogisticSuperior( child, base );
        else
            CreateLink( child, base, fun );
    }
}

// -----------------------------------------------------------------------------
// Name: LinkGenerator::IsLogisticBase
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
bool LinkGenerator::IsLogisticBase( const kernel::Entity_ABC& entity ) const
{
    if( const LogisticLevelAttritube* pAttribute = entity.Retrieve< LogisticLevelAttritube >() )
        if( pAttribute->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
            return true;
    return false;
}

// -----------------------------------------------------------------------------
// Name: LinkGenerator::SetLogisticSuperior
// Created: LGY 2011-10-12
// -----------------------------------------------------------------------------
void LinkGenerator::SetLogisticSuperior( kernel::Entity_ABC& entity, const kernel::Entity_ABC& superior )
{
    if( LogisticHierarchiesBase* pHierarchy = entity.Retrieve< LogisticHierarchiesBase >() )
        pHierarchy->SetLogisticSuperior( &superior );
}
