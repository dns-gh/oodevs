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
#include "CommandPostAttributes.h"
#include "Dotation.h"
#include "FormationModel.h"
#include "GhostModel.h"
#include "LimitsModel.h"
#include "LogisticAttribute.h"
#include "LogisticBaseStates.h"
#include "LogisticLevelAttritube.h"
#include "Model.h"
#include "ObjectsModel.h"
#include "ProfilesModel.h"
#include "ScoresModel.h"
#include "StaticModel.h"
#include "Stocks.h"
#include "SuccessFactorsModel.h"
#include "TacticalLine_ABC.h"
#include "TeamsModel.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/DotationCapacityType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/LogisticSupplyClass.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "ENT/ENT_Tr_Gen.h"
#include "tools/GeneralConfig.h"
#include "tools/SchemaWriter.h"
#include <boost/foreach.hpp>
#include <xeuseuleu/xsl.hpp>

using namespace kernel;
using namespace tools;

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker constructor
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
ModelConsistencyChecker::ModelConsistencyChecker( const Model& model, const ::StaticModel& staticModel, Controllers& controllers )
    : model_      ( model )
    , staticModel_( staticModel )
    , controllers_( controllers )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker destructor
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
ModelConsistencyChecker::~ModelConsistencyChecker()
{
    ClearErrors();
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
        explicit EntityWithLongNameExtractor( std::vector< const Entity_ABC* >& entities )
            : entities_( entities )
        {
            // NOTHING
        }

        virtual ~EntityWithLongNameExtractor()
        {
            // NOTHING
        }

        virtual void operator()( const Entity_ABC& entity ) const
        {
            if( !gui::LongNameHelper::GetEntityLongName( entity ).empty() )
                entities_.push_back( &entity );
        }

    private:
        std::vector< const Entity_ABC* >& entities_;
    };

    bool CompareLongName( const Entity_ABC& entity1, const Entity_ABC& entity2 )
    {
        return gui::LongNameHelper::GetEntityLongName( entity1 ) == gui::LongNameHelper::GetEntityLongName( entity2 );
    }

    bool CompareName( const Entity_ABC& entity1, const Entity_ABC& entity2 )
    {
        return entity1.GetName() == entity2.GetName();
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckConsistency
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
bool ModelConsistencyChecker::CheckConsistency( bool( *IsError )( E_ConsistencyCheck type ) )
{
    ClearErrors();

    CheckUniqueness( eLongNameUniqueness, &CompareLongName );
    CheckUniqueness( eTeamNameUniqueness, &CompareName );
    CheckUniqueness( eObjectNameUniqueness, &CompareName );
    CheckUniqueness( eLimaNameUniqueness, &CompareName );
    CheckUniqueness( eLimitNameUniqueness, &CompareName );

    CheckStockInitialization();
    CheckMaxStockExceeded();
    CheckMissingStock();
    CheckLogisticInitialization();
    CheckLogisticBase();
    CheckLogisticFormation();

    CheckProfileUniqueness();
    CheckProfileInitialization();

    CheckGhosts();

    CheckCommandPosts();

    CheckKnowledgeGroups();
    CheckLoadingErrors();
    CheckScores();
    CheckSuccessFactors();

    bool isValid = true;
    for( IT_ConsistencyErrors it = errors_.begin(); it != errors_.end(); ++it )
        if( IsError( it->type_ ) )
        {
            isValid = false;
            break;
        }
    const_cast< Model& >( model_ ).SetExerciseValidity( isValid );

    return !errors_.empty();
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::ClearErrors
// Created: JSR 2011-12-06
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::ClearErrors()
{
    for( IT_ConsistencyErrors it = errors_.begin(); it != errors_.end(); ++it )
        for( IT_SafeEntities entityIt = it->entities_.begin(); entityIt != it->entities_.end(); ++entityIt )
            delete *entityIt;
    errors_.clear();
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
            if( attribute->Extends( "unit") )
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
                bool bFound = false;
                for( CIT_SafeEntities safeIt = error.entities_.begin(); safeIt != error.entities_.end(); ++safeIt )
                    if( **safeIt == &entity1 )
                    {
                        bFound = true;
                        break;
                    }
                if( !bFound )
                    error.entities_.push_back( new SafePointer< Entity_ABC >( controllers_, &entity1 ) );
                error.entities_.push_back( new SafePointer< Entity_ABC >( controllers_, &entity2 ) );
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
        if( agent.GetType().HasStocks() && ( !stocks || ( stocks && !stocks->HasDotations() ) ) )
            AddError( eStockInitialization, &agent );
    }
}

namespace
{
    void CompareConsumptionAndStock( const LogisticHierarchiesBase& pLogHierarchy, const TacticalHierarchies& tacticalHierarchy, std::vector< std::string >& missingStocks, const ::StaticModel& staticModel )
    {
        tools::Iterator< const kernel::Entity_ABC& > logChildIt = pLogHierarchy.CreateSubordinateIterator();

        while( logChildIt.HasMoreElements() ) // for each log subordinate of the tc2 automat
        {
            const kernel::Entity_ABC& logChild = logChildIt.NextElement();
            if( const Automat_ABC* pLogChildAutomat = dynamic_cast< const Automat_ABC* >( &logChild ) ) // if it's an automat
            {
                const kernel::TacticalHierarchies* pTacticalHierarchies = logChild.Retrieve< kernel::TacticalHierarchies >();
                tools::Iterator< const kernel::Entity_ABC& > child = pTacticalHierarchies->CreateSubordinateIterator();
                while( child.HasMoreElements() ) // for each tactical subordinate in this automat
                {
                    const kernel::Entity_ABC& entity = child.NextElement();
                    if( const Agent_ABC* pLogChildAgent = dynamic_cast< const Agent_ABC* >( &entity ) ) // if it's an agent
                    {
                        kernel::AgentType& agentType = staticModel.types_.tools::Resolver< kernel::AgentType >::Get( pLogChildAgent->GetType().GetId() );
                        tools::Iterator< const kernel::AgentComposition& > agentCompositionIterator = agentType.CreateIterator();
                        while( agentCompositionIterator.HasMoreElements() ) // for each equipment in this agent
                        {
                            const kernel::AgentComposition& agentComposition = agentCompositionIterator.NextElement();
                            const kernel::EquipmentType& equipmentType = staticModel.objectTypes_.tools::Resolver< kernel::EquipmentType >::Get( agentComposition.GetType().GetId() );
                            tools::Iterator< const kernel::DotationCapacityType& > resourcesIterator = equipmentType.CreateResourcesIterator();
                            while( resourcesIterator.HasMoreElements() ) // for each resource in this equipment
                            {
                                const kernel::DotationCapacityType& dotationCapacity = resourcesIterator.NextElement();
                                const kernel::DotationType& category = staticModel.objectTypes_.kernel::Resolver2< kernel::DotationType >::Get( dotationCapacity.GetName() );
                                std::string stockCategoryName = category.GetLogisticSupplyClass().GetName();

                                if( agentComposition.GetCount() > 0 &&
                                    dotationCapacity.GetNormalizedConsumption() > 0. &&
                                    std::find( missingStocks.begin(), missingStocks.end(), stockCategoryName ) == missingStocks.end() ) // if this resource is needed and not already missing
                                {
                                    bool missing = true;
                                    tools::Iterator< const Entity_ABC& > tacticalChildIt = tacticalHierarchy.CreateSubordinateIterator();
                                    while( tacticalChildIt.HasMoreElements() ) // for each tactical subordinate of the tc2 automat
                                    {
                                        const kernel::Entity_ABC& tacticalChild = tacticalChildIt.NextElement();
                                        if( const Agent_ABC* tacticalChildAgent = dynamic_cast< const Agent_ABC* >( &tacticalChild ) ) // if it's an agent
                                        {
                                            const Stocks* stocks = tacticalChildAgent->Retrieve< Stocks >();
                                            if( stocks && stocks->HasDotations() && stocks->HasDotationType( category ) ) // if it has stocks and enough of this dotation type
                                                missing = false;
                                        }
                                    }
                                    if( missing ) // if needed resource is missing on the tc2 automat
                                        missingStocks.push_back( stockCategoryName );
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckMissingStock
// Created: ABR 2012-05-03
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckMissingStock()
{
    Iterator< const Automat_ABC& > it = model_.GetAutomatResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& automat = it.NextElement();
        const LogisticLevelAttritube* attribute = automat.Retrieve< LogisticLevelAttritube >();
        if( attribute->GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
        {
            std::vector< std::string > missingStocks;
            CompareConsumptionAndStock( automat.Get< LogisticHierarchiesBase >(), automat.Get< TacticalHierarchies >(), missingStocks, staticModel_ );
            if( !missingStocks.empty() )
            {
                std::string optional;
                for( std::vector< std::string >::const_iterator it = missingStocks.begin(); it != missingStocks.end(); ++it )
                    optional += ( optional.empty() ) ? *it : ( ", " + *it );
                AddError( eStockMissing, &automat, optional );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckMaxStockExceeded
// Created: JSR 2012-01-03
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckMaxStockExceeded()
{
    Iterator< const Agent_ABC& > it = model_.GetAgentResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Agent_ABC& agent = it.NextElement();
        const Stocks* stocks = agent.Retrieve< Stocks >();
        if( stocks && stocks->HasDotations() )
        {
            typedef std::map< std::string, std::pair< double, double > > T_StockCapacities;
            typedef T_StockCapacities::const_iterator                  CIT_StockCapacities;
            T_StockCapacities maxCapacities;
            Iterator< const AgentComposition& > itComposition = agent.GetType().CreateIterator();
            while( itComposition.HasMoreElements() )
            {
                const AgentComposition& agentComposition = itComposition.NextElement();
                const ComponentType& equipment = agentComposition.GetType();
                const EquipmentType& equipmentType = staticModel_.objectTypes_.Resolver< EquipmentType >::Get( equipment.GetId() );
                if( const EquipmentType::CarryingSupplyFunction* carrying = equipmentType.GetLogSupplyFunctionCarrying() )
                {
                    double nEquipments = static_cast< double >( agentComposition.GetCount() );
                    maxCapacities[ carrying->stockNature_ ].first += nEquipments * carrying->stockWeightCapacity_;
                    maxCapacities[ carrying->stockNature_ ].second += nEquipments * carrying->stockVolumeCapacity_;
                }
            }

            T_StockCapacities currentCapacities;
            Iterator< const Dotation& > itDotation = stocks->CreateIterator();
            while( itDotation.HasMoreElements() )
            {
                const Dotation& dotation = itDotation.NextElement();
                currentCapacities[ dotation.type_.GetNature() ].first += dotation.quantity_ * dotation.type_.GetUnitWeight();
                currentCapacities[ dotation.type_.GetNature() ].second += dotation.quantity_ * dotation.type_.GetUnitVolume();
            }

            for( CIT_StockCapacities it = currentCapacities.begin(); it != currentCapacities.end(); ++it )
            {
                CIT_StockCapacities maxIt = maxCapacities.find( it->first );
                if( maxIt == maxCapacities.end() || it->second.first > maxIt->second.first || it->second.second > maxIt->second.second )
                    AddError( eStockMaxExceeded, &agent, it->first );
            }
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
        const Entity_ABC* superior = hierarchy->GetSuperior();
        if( !superior )
            AddError( eLogisticInitialization, &automat );
        if( superior == &automat )
        {
            const LogisticBaseSuperior nullReference;
            const_cast< LogisticBaseStates* >( hierarchy )->SetLogisticSuperior( nullReference );
            AddError( eLogisticInitialization, &automat );
        }
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
    for( ProfilesModel::CIT_Units it = units.begin(); it != units.end(); ++it )
    {
        const ProfilesModel::T_Units::value_type& element = *it;
        if( element.second.size() > 1 )
        {
            const Entity_ABC* entity = model_.GetTeamResolver().Find( element.first );
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
            AddError( eProfileUnwritable, &agent );
            if( !model_.profiles_.IsReadable( agent ) )
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
        if( ghost.IsConverted() )
            AddError( eGhostConverted, &ghost, ghost.GetType().ascii() );
        else
            AddError( eGhostExistence, &ghost );
    }
}

namespace
{
    bool IsCommandPost( const Entity_ABC& entity )
    {
        if( const CommandPostAttributes* pAttributes = entity.Retrieve< CommandPostAttributes >() )
            return pAttributes->IsCommandPost();
        return false;
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckCommandPosts
// Created: JSR 2012-01-06
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckCommandPosts()
{
    Iterator< const Automat_ABC& > it = model_.GetAutomatResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& automat = it.NextElement();
        const TacticalHierarchies& hierarchies = automat.Get< TacticalHierarchies >();
        tools::Iterator< const Entity_ABC& > itChild = hierarchies.CreateSubordinateIterator();
        unsigned int commandPostCount = 0;
        while( itChild.HasMoreElements() )
        {
            if( IsCommandPost( itChild.NextElement() ) )
                ++commandPostCount;
        }
        if( commandPostCount == 0 )
            AddError( eNoCommandPost, &automat );
        else if( commandPostCount > 1 )
            AddError( eSeveralCommandPost, &automat );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckKnowledgeGroups
// Created: JSR 2012-01-06
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckKnowledgeGroups()
{
    // TODO � v�rifier
    Iterator< const Team_ABC& > it = model_.GetTeamResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Team_ABC& team = it.NextElement();
        const CommunicationHierarchies& hierarchies = team.Get< CommunicationHierarchies >();
        tools::Iterator< const Entity_ABC& > itSub = hierarchies.CreateSubordinateIterator();
        while( itSub.HasMoreElements() )
        {
            const Entity_ABC* entity = &itSub.NextElement();
            if( !dynamic_cast< const KnowledgeGroup_ABC* >( entity ) )
                AddError( eNoKnowledgeGroup, entity );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckScores
// Created: JSR 2012-01-06
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckScores()
{
    try
    {
        const tools::SchemaWriter schemaWriter;
        xml::xostringstream xos;
        model_.scores_.Serialize( xos, schemaWriter );
    }
    catch( std::exception& e )
    {
        AddError( eScoreError, 0, std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckSuccessFactors
// Created: JSR 2012-01-06
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckSuccessFactors()
{
    try
    {
        const tools::SchemaWriter schemaWriter;
        xml::xostringstream xos;
        model_.successFactors_.Serialize( xos, schemaWriter );
        xsl::xstringtransform xst( tools::GeneralConfig::BuildResourceChildFile( "SuccessFactors.xsl" ) );
        xml::xistringstream xis( xos.str() );
        xst << xis;
    }
    catch( std::exception& e )
    {
        AddError( eSuccessFactorError, 0, std::string( e.what() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckLoadingErrors
// Created: JSR 2012-01-04
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckLoadingErrors()
{
    Model::T_LoadingErrors errors = model_.GetLoadingErrors();
    for( Model::T_LoadingErrors::const_iterator it = errors.begin(); it != errors.end(); ++it )
        AddError( it->first, 0, it->second );
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckLogisticBase
// Created: JSR 2012-01-09
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckLogisticBase()
{
    Iterator< const Object_ABC& > it = model_.GetObjectResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Object_ABC& obj = it.NextElement();
        const LogisticAttribute* attribute = obj.Retrieve< LogisticAttribute >();
        if( attribute )
        {
            if( !attribute->HasValidLogisticBase() )
                AddError( eNoLogisticBase, &obj );
            else
            {
                const Entity_ABC* logisticBase = attribute->GetLogisticBase();
                if( logisticBase )
                {
                    const TacticalHierarchies* logHierarchy = static_cast< const TacticalHierarchies* >( logisticBase->Retrieve< TacticalHierarchies >() );
                    const TacticalHierarchies* objHierarchy = static_cast< const TacticalHierarchies* >( obj.Retrieve< TacticalHierarchies >() );
                    if( logHierarchy && objHierarchy && &logHierarchy->GetTop() != &objHierarchy->GetTop() )
                        AddError( eLogisticBaseNotSameTeam, &obj );
                }
            }
        }
    }
}

namespace
{
    bool IsLogisticBaseFormation( const Formation_ABC& formation )
    {
        if( formation.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
            return true;
        const TacticalHierarchies* hierarchy = static_cast< const TacticalHierarchies* >( formation.Retrieve< TacticalHierarchies >() );
        if( !hierarchy )
            return false;
        const Entity_ABC* superior = hierarchy->GetSuperior();
        if( !superior )
            return false;
        const Formation_ABC* pParentFormation = dynamic_cast< const Formation_ABC* >( superior );
        if( !pParentFormation )
            return false;
        return IsLogisticBaseFormation( *pParentFormation );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckLogisticFormation
// Created: MMC 2012-04-18
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckLogisticFormation()
{
    Iterator< const Automat_ABC& > it = model_.GetAutomatResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& automat = it.NextElement();
        const kernel::AutomatType& type = automat.GetType();
        if( !type.HasLogistics() )
            continue;
        if( automat.GetLogisticLevel() == kernel::LogisticLevel::logistic_base_ )
            continue;
        const TacticalHierarchies* hierarchy = static_cast< const TacticalHierarchies* >( automat.Retrieve< TacticalHierarchies >() );
        if( hierarchy && hierarchy->GetSuperior() )
        {
            const Formation_ABC* pParentFormation = dynamic_cast< const Formation_ABC* >( hierarchy->GetSuperior() );
            if( pParentFormation && IsLogisticBaseFormation( *pParentFormation ) )
                continue;
        }
        AddError( eNoLogisticFormation, &automat );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::AddError
// Created: ABR 2011-09-27
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::AddError( E_ConsistencyCheck type, const Entity_ABC* entity, const std::string& optional /*= ""*/ )
{
    ConsistencyError error( type );
    error.entities_.push_back( new SafePointer< Entity_ABC >( controllers_, entity ) );
    error.optional_ = optional;
    errors_.push_back( error );
}
