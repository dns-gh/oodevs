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
#include "LongNameHelper.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include <boost/lexical_cast.hpp>

using namespace gui;
using namespace kernel;

// -----------------------------------------------------------------------------
// DiffusionListData
// -----------------------------------------------------------------------------

const QString DiffusionListData::separator_ = ";";
const QRegExp DiffusionListData::regexp_ ( "([0-9]+[;]{1})*[0-9]*" );

// -----------------------------------------------------------------------------
// DiffusionListFunctor_ABC
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DiffusionListFunctor_ABC::DiffusionListFunctor_ABC
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
DiffusionListFunctor_ABC::DiffusionListFunctor_ABC( const std::string name, const kernel::Entity_ABC& currentTeam )
    : name_       ( name )
    , currentTeam_( currentTeam )
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
// Name: DiffusionListFunctors::IsFromCurrentTeam
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
bool DiffusionListFunctor_ABC::IsFromCurrentTeam( const kernel::Entity_ABC& agent ) const
{
    const TacticalHierarchies& hierarchy = agent.Get< TacticalHierarchies >();
    return hierarchy.GetTop().GetId() == currentTeam_.GetId();
}

// -----------------------------------------------------------------------------
// DiffusionListReceiversExtractor
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DiffusionListReceiversExtractor::DiffusionListReceiversExtractor
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
DiffusionListReceiversExtractor::DiffusionListReceiversExtractor( const std::string name, const kernel::Entity_ABC& currentTeam, DiffusionListData::T_Entities& targets )
    : DiffusionListFunctor_ABC( name, currentTeam )
    , targets_     ( targets )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListReceiversExtractor::~DiffusionListReceiversExtractor
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
DiffusionListReceiversExtractor::~DiffusionListReceiversExtractor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListReceiversExtractor::operator()
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
void DiffusionListReceiversExtractor::operator()( const kernel::Entity_ABC& agent ) const
{
    if( !IsFromCurrentTeam( agent ) )
        return;

    DictionaryExtensions* dico = const_cast< DictionaryExtensions* >( agent.Retrieve< DictionaryExtensions >() );
    if( !dico )
        return;

    if( !dico->GetValue( "TypePC" ).empty() && dico->GetValue( "TypePC" ) != "PasCorresp" )
    {
        QString longName = longname::GetEntityLongName( agent ).c_str();
        longName = ( longName.isEmpty() ? agent.GetName() : longName );
        assert( targets_.find( longName ) == targets_.end() );
        targets_[ longName ] = &agent;
    }
}

// -----------------------------------------------------------------------------
// DiffusionListEmittersExtractor
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DiffusionListEmittersExtractor::DiffusionListEmittersExtractor
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
DiffusionListEmittersExtractor::DiffusionListEmittersExtractor( const std::string name, const kernel::Entity_ABC& currentTeam, DiffusionListData::T_Entities& targets )
    : DiffusionListFunctor_ABC( name, currentTeam )
    , targets_     ( targets )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListEmittersExtractor::~DiffusionListEmittersExtractor
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
DiffusionListEmittersExtractor::~DiffusionListEmittersExtractor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DiffusionListEmittersExtractor::operator()
// Created: ABR 2012-02-28
// -----------------------------------------------------------------------------
void DiffusionListEmittersExtractor::operator()( const kernel::Entity_ABC& agent ) const
{
    if( !IsFromCurrentTeam( agent ) )
        return;

    DictionaryExtensions* dico = const_cast< DictionaryExtensions* >( agent.Retrieve< DictionaryExtensions >() );
    if( !dico )
        return;
    ExtensionType* type = dico->GetExtensionTypes().tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    if( !type )
        return;
    AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( name_ );
    if( !attribute )
        return;
    if( attribute->IsActive( dico->GetExtensions() ) )
    {
        QString longName = longname::GetEntityLongName( agent ).c_str();
        longName = ( longName.isEmpty() ? agent.GetName() : longName );
        assert( targets_.find( longName ) == targets_.end() );
        targets_[ longName ] = &agent;
    }
}

// -----------------------------------------------------------------------------
// DiffusionListGenerator
// -----------------------------------------------------------------------------

// -----------------------------------------------------------------------------
// Name: DiffusionListGenerator::DiffusionListGenerator
// Created: ABR 2011-05-04
// -----------------------------------------------------------------------------
DiffusionListGenerator::DiffusionListGenerator( const std::string name, const kernel::Entity_ABC& currentTeam, QStringList& generatedDiffusionList )
    : DiffusionListFunctor_ABC( name, currentTeam )
    , generatedDiffusionList_( generatedDiffusionList )
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
    assert( IsFromCurrentTeam( agent ) );
    DictionaryExtensions* dico = const_cast< DictionaryExtensions* >( agent.Retrieve< DictionaryExtensions >() );
    assert( dico != 0 );
    ExtensionType* type = dico->GetExtensionTypes().tools::StringResolver< ExtensionType >::Find( "orbat-attributes" );
    assert( type != 0 );
    AttributeType* attribute = type->tools::StringResolver< AttributeType >::Find( name_ );
    assert( attribute != 0 );
    if( !attribute->IsActive( dico->GetExtensions() ) )
        return;

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
        if( entityDico && !entityDico->GetValue( "TypePC" ).empty() && entityDico->GetValue( "TypePC" ) != "PasCorresp" && entityHierarchy.GetLevel() == grandFatherHierarchy.GetLevel() )
            generatedDiffusionList_ << QString::number( entity.GetId() );
    }
}

