// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "DiffusionListFunctors.h"

#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "DiffusionListHierarchy.h"
#include <boost/lexical_cast.hpp>

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// DiffusionListFunctor_ABC
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DiffusionListFunctor_ABC::DiffusionListFunctor_ABC
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
DiffusionListFunctor_ABC::DiffusionListFunctor_ABC( const std::string name )
    : name_( name )
{
    assert( !name_.empty() );
}

// -----------------------------------------------------------------------------
// Name: DiffusionListFunctor_ABC::~DiffusionListFunctor_ABC
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
DiffusionListFunctor_ABC::~DiffusionListFunctor_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// DiffusionListClearer
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DiffusionListClearer::DiffusionListClearer
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
DiffusionListClearer::DiffusionListClearer( const std::string name )
    : DiffusionListFunctor_ABC( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListClearer::~DiffusionListClearer
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
DiffusionListClearer::~DiffusionListClearer()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListClearer::operator()
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
void DiffusionListClearer::operator()( const kernel::Entity_ABC& agent ) const
{
    DictionaryExtensions* dico = const_cast< DictionaryExtensions* >( agent.Retrieve< DictionaryExtensions >() );
    if( !dico )
        return;
    dico->SetValue( name_, "" );
}

// -----------------------------------------------------------------------------
// DiffusionListGenerator
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DiffusionListGenerator::DiffusionListGenerator
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
DiffusionListGenerator::DiffusionListGenerator( const std::string name )
    : DiffusionListFunctor_ABC( name )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListGenerator::~DiffusionListGenerator
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
DiffusionListGenerator::~DiffusionListGenerator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListGenerator::operator()
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
void DiffusionListGenerator::operator()( const Entity_ABC& agent ) const
{
    DictionaryExtensions* dico = const_cast< DictionaryExtensions* >( agent.Retrieve< DictionaryExtensions >() );
    if( !dico )
        return;
    ExtensionType* type = dico->GetExtensionTypes().tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( !type )
        return;
    AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( name_ );
    if( !attribute )
        return;
    if( attribute->IsActive( dico->GetExtensions() ) && dico->GetValue( name_ ).empty() )
    {
        std::string diffusion;
        const TacticalHierarchies& hierarchy = agent.Get< TacticalHierarchies >();
        const Entity_ABC* father = hierarchy.GetSuperior();
        assert( father );
        const TacticalHierarchies& fatherHierarchy = father->Get< TacticalHierarchies >();
        const Entity_ABC* grandFather = fatherHierarchy.GetSuperior();
        assert( grandFather );
        const TacticalHierarchies& grandFatherHierarchy = grandFather->Get< TacticalHierarchies >();
        tools::Iterator< const Entity_ABC& > it = grandFatherHierarchy.CreateSubordinateIterator();
        while( it.HasMoreElements() )
        {
            const Entity_ABC& entity = it.NextElement();

            DictionaryExtensions* entityDico = const_cast< DictionaryExtensions* >( entity.Retrieve< DictionaryExtensions >() );
            const TacticalHierarchies& entityHierarchy = entity.Get< TacticalHierarchies >();
            if( entityDico && attribute->IsActive( entityDico->GetExtensions() ) && entityHierarchy.GetLevel() == fatherHierarchy.GetLevel() )
            {
                diffusion += ( diffusion.empty() ) ? "" : DiffusionListHierarchy::diffusionSeparator_;
                diffusion += boost::lexical_cast< std::string >( entity.GetId() );
            }
        }
        dico->SetValue( name_, diffusion );
    }
}

