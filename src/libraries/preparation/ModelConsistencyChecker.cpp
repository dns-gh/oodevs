// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "ModelConsistencyChecker.h"
#include "AgentsModel.h"
#include "FormationModel.h"
#include "GhostModel.h"
#include "LimitsModel.h"
#include "LogisticBaseStates.h"
#include "LogisticHierarchiesBase.h"
#include "Model.h"
#include "ObjectsModel.h"
#include "ProfilesModel.h"
#include "StaticModel.h"
#include "Stocks.h"
#include "FormationHierarchies.h"
#include "TacticalLine_ABC.h"
#include "TeamsModel.h"
#include "clients_gui/Tools.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Team_ABC.h"
#include "tools/Resolver.h"
#include <boost/bind.hpp>
#include <boost/foreach.hpp>

using namespace kernel;
using namespace tools;

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker constructor
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
ModelConsistencyChecker::ModelConsistencyChecker( const Model& model, const StaticModel& staticModel )
    : model_      ( model )
    , staticModel_( staticModel )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker destructor
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
ModelConsistencyChecker::~ModelConsistencyChecker()
{
    errors_.clear();
    entities_.clear();
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::GetConsistencyErrors
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
const ModelConsistencyChecker::T_ConsistencyErrors& ModelConsistencyChecker::GetConsistencyErrors() const
{
    return errors_;
}

namespace
{
    class EntityWithLongNameExtractor : private boost::noncopyable
    {
    public:
        EntityWithLongNameExtractor( std::vector< const Entity_ABC* >& entities ) : entities_( entities ) {}
        virtual ~EntityWithLongNameExtractor() {}

        virtual void operator()( const kernel::Entity_ABC& entity ) const
        {
            if( !gui::LongNameHelper::GetEntityLongName( entity ).empty() )
                entities_.push_back( &entity );
        }

    private:
        std::vector< const Entity_ABC* >& entities_;
    };

    bool CompareLongName( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 )
    {
        return gui::LongNameHelper::GetEntityLongName( entity1 ) == gui::LongNameHelper::GetEntityLongName( entity2 );
    }

    bool CompareName( const kernel::Entity_ABC& entity1, const kernel::Entity_ABC& entity2 )
    {
        return entity1.GetName() == entity2.GetName();
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckConsistency
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
bool ModelConsistencyChecker::CheckConsistency( unsigned int filters /*= eAllChecks*/ )
{
    errors_.clear();
    filters_ = filters;
    if( filters & eLongNameUniqueness )
        CheckUniqueness( eLongNameUniqueness, &CompareLongName );
    if( filters & eTeamNameUniqueness )
        CheckUniqueness( eTeamNameUniqueness, &CompareName );
    if( filters & eObjectNameUniqueness )
        CheckUniqueness( eObjectNameUniqueness, &CompareName );
    if( filters & eLimaNameUniqueness )
        CheckUniqueness( eLimaNameUniqueness, &CompareName );
    if( filters & eLimitNameUniqueness )
        CheckUniqueness( eLimitNameUniqueness, &CompareName );
    if( filters & eStockInitialization )
        CheckStockInitialization();
    if( filters & eLogisticInitialization )
        CheckLogisticInitialization();
    if( filters & eProfileUniqueness )
        CheckProfileUniqueness();
    if( filters & eProfileUnreadable || filters & eProfileUnwritable )
        CheckProfileInitialization();
    if( filters & eGhostExistence || filters & eGhostConverted )
        CheckGhosts();
	//if ( filters & eFormationWithSameLevelEmptiness )
	//	CheckFormationWithSameLevelAsParentEmptiness();
    return !errors_.empty();
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::FillEntitiesCopy
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::FillEntitiesCopy( E_ConsistencyCheck type )
{
    entities_.clear();
    switch( type )
    {
    case eLongNameUniqueness:
        {
            ExtensionType* type = staticModel_.extensions_.StringResolver< ExtensionType >::Find( "orbat-attributes" );
            if( !type )
                return;
            AttributeType* attribute = type->Find( "NomLong" );
            if( !attribute )
                return;
            EntityWithLongNameExtractor functor( entities_ );
            if( attribute->Extends( "units") )
                model_.agents_.Resolver< Agent_ABC >::Apply( functor );
            if( attribute->Extends( "automat" ) )
                model_.agents_.Resolver< Automat_ABC >::Apply( functor );
            if( attribute->Extends( "formation" ) )
                model_.formations_.Resolver< Formation_ABC >::Apply( functor );
            if( attribute->Extends( "crowd" ) )
                model_.agents_.Resolver< Population_ABC >::Apply( functor );
            if( attribute->Extends( "population" ) )
                model_.agents_.Resolver< Inhabitant_ABC >::Apply( functor );
            if( attribute->Extends( "party" ) )
                model_.teams_.Resolver< Team_ABC >::Apply( functor );
        }
        break;
    case eTeamNameUniqueness:
        {
            Iterator< const Team_ABC& > it = model_.teams_.CreateIterator();
            while( it.HasMoreElements() )
                entities_.push_back( &it.NextElement() );
        }
        break;
    case eObjectNameUniqueness:
        {
            Iterator< const Object_ABC& > it = model_.objects_.CreateIterator();
            while( it.HasMoreElements() )
                entities_.push_back( &it.NextElement() );
        }
        break;
    case eLimaNameUniqueness:
        {
            Iterator< const ::TacticalLine_ABC& > it = model_.limits_.CreateIterator();
            while( it.HasMoreElements() )
            {
                const ::TacticalLine_ABC& line = it.NextElement();
                if( !line.IsLimit() )
                    entities_.push_back( &line );
            }
        }
        break;
    case eLimitNameUniqueness:
        {
            Iterator< const ::TacticalLine_ABC& > it = model_.limits_.CreateIterator();
            while( it.HasMoreElements() )
            {
                const ::TacticalLine_ABC& line = it.NextElement();
                if( line.IsLimit() )
                    entities_.push_back( &line );
            }
        }
        break;
    case eStockInitialization:
    case eLogisticInitialization:
    case eProfileUniqueness:
    case eProfileUnreadable:
    case eProfileUnwritable:
    default:
        throw std::runtime_error( __FUNCTION__ " invalid call to FillEntitiesCopy." );
        break;
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckUniqueness
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckUniqueness( E_ConsistencyCheck type, bool ( *comparator )( const Entity_ABC&, const Entity_ABC& ) )
{
    FillEntitiesCopy( type );
    for( std::vector< const Entity_ABC* >::iterator it = entities_.begin(); it != entities_.end(); ++it )
    {
        const Entity_ABC& entity1 = **it;
        ConsistencyError error( type );
        for( std::vector< const Entity_ABC* >::iterator iter = it + 1; iter != entities_.end(); )
        {
            const Entity_ABC& entity2 = **iter;
            if( ( *comparator )( entity1, entity2 ) )
            {
                if( std::find( error.entities_.begin(), error.entities_.end(), &entity1 ) == error.entities_.end() )
                    error.entities_.push_back( &entity1 );
                error.entities_.push_back( &entity2 );
                iter = entities_.erase( iter );
            }
            else
                ++iter;
        }
        if( !error.entities_.empty() )
            errors_.push_back( error );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckStockInitialization
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckStockInitialization()
{
    Iterator< const Agent_ABC& > it = model_.GetAgentResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Agent_ABC& agent = it.NextElement();
        const Stocks* stocks = agent.Retrieve< Stocks >();
        if( agent.GetType().HasStocks() && stocks && !stocks->HasDotations() )
            AddError( eStockInitialization, &agent );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckLogisticInitialization
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckLogisticInitialization()
{
    Iterator< const Automat_ABC& > it = model_.GetAutomatResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& automat = it.NextElement();
        const LogisticBaseStates* hierarchy = static_cast< const LogisticBaseStates* >( automat.Retrieve< LogisticHierarchiesBase >() );
        if( hierarchy->GetSuperior() == 0 )
            AddError( eLogisticInitialization, &automat );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckProfileUniqueness
// Created: ABR 2011-09-27
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckProfileUniqueness()
{
    ProfilesModel::T_Units units;
    model_.profiles_.Visit( units );
    QString result;

    for( ProfilesModel::CIT_Units it = units.begin(); it != units.end(); ++it )
    {
        const ProfilesModel::T_Units::value_type& element = *it;
        if( element.second.size() > 1 )
        {
            const kernel::Entity_ABC* entity = model_.GetTeamResolver().Find( element.first );
            if( !entity )
                entity = model_.GetAutomatResolver().Find( element.first );
            if( !entity )
                entity = model_.GetPopulationResolver().Find( element.first );
            if( !entity )
                continue;
            std::string optional;
            BOOST_FOREACH( const std::string& profile, element.second )
                optional += " '" + profile + "'";
            AddError( eProfileUniqueness, entity, optional );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckProfileInitialization
// Created: ABR 2011-09-27
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckProfileInitialization()
{
    Iterator< const Automat_ABC& > it = model_.GetAutomatResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& agent = it.NextElement();
        if( !model_.profiles_.IsWriteable( agent ) )
        {
            if( filters_ & eProfileUnwritable )
                AddError( eProfileUnwritable, &agent );
            if( filters_ & eProfileUnreadable && !model_.profiles_.IsReadable( agent ) )
                AddError( eProfileUnreadable, &agent );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckGhosts
// Created: ABR 2011-10-20
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckGhosts()
{
    Iterator< const Ghost_ABC& > it = model_.ghosts_.CreateIterator();
    while( it.HasMoreElements() )
    {
        const Ghost_ABC& ghost = it.NextElement();
        if( filters_ & eGhostConverted )
        {
            if( ghost.IsConverted() )
                AddError( eGhostConverted, &ghost, ghost.GetType().ascii() );
            else if( filters_ & eGhostExistence )
                AddError( eGhostExistence, &ghost );
        }
        else
            AddError( eGhostExistence, &ghost );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckFormationWithSameLevelAsParentEmptiness
// Created: RPD 2011-11-07
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckFormationWithSameLevelAsParentEmptiness()
{
    Iterator< const Formation_ABC& > formationIterator = model_.GetFormationResolver().CreateIterator();
    while( formationIterator.HasMoreElements() )
    {
        const Formation_ABC& formation = formationIterator.NextElement();
		const FormationHierarchies* hierarchy = formation.Retrieve< FormationHierarchies >();
        if( hierarchy && hierarchy->GetSuperior() && hierarchy->CountSubordinates() != 0 )
		{
			const FormationHierarchies* parentHierarchy = hierarchy->GetSuperior()->Retrieve< FormationHierarchies >();
			if ( hierarchy->GetLevel() == parentHierarchy->GetLevel() )
			{
				AddError( eFormationWithSameLevelEmptiness, &formation );
			}
		}
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::AddError
// Created: ABR 2011-09-27
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::AddError( E_ConsistencyCheck type, const kernel::Entity_ABC* entity, const std::string& optional /*= ""*/ )
{
    assert( entity );
    ConsistencyError error ( type );
    error.entities_.push_back( entity );
    error.optional_ = optional;
    errors_.push_back( error );
}
