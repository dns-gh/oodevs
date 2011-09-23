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
#include "LimitsModel.h"
#include "LogisticBaseStates.h"
#include "LogisticHierarchiesBase.h"
#include "Model.h"
#include "ObjectsModel.h"
#include "StaticModel.h"
#include "Stocks.h"
#include "TacticalLine_ABC.h"
#include "TeamsModel.h"

#include "clients_gui/Tools.h"
#include "clients_gui/LongNameHelper.h"

#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Entity_ABC.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_kernel/Team_ABC.h"

#include "tools/Resolver.h"
#include <boost/bind.hpp>

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
                if( std::find( error.ids_.begin(), error.ids_.end(), entity1.GetId() ) == error.ids_.end() )
                    error.ids_.push_back( entity1.GetId() );
                error.ids_.push_back( entity2.GetId() );
                iter = entities_.erase( iter );
            }
            else
                ++iter;
        }
        if( !error.ids_.empty() )
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
        if( stocks && !stocks->HasDotations() )
        {
            ConsistencyError error( eStockInitialization );
            error.ids_.push_back( agent.GetId() );
            errors_.push_back( error );
        }
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
        {
            ConsistencyError error( eLogisticInitialization );
            error.ids_.push_back( automat.GetId() );
            errors_.push_back( error );
        }
    }
}
