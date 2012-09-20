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
#include "ResourceNetworkAttribute.h"
#include "ScoresModel.h"
#include "StaticModel.h"
#include "Stocks.h"
#include "SuccessFactorsModel.h"
#include "TacticalLine_ABC.h"
#include "TeamsModel.h"
#include "UrbanModel.h"
#include "UserProfile.h"
#include "clients_gui/LongNameHelper.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentComposition.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/AgentTypes.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/AutomatType.h"
#include "clients_kernel/CommandPostAttributes_ABC.h"
#include "clients_kernel/ComponentType.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/DotationCapacityType.h"
#include "clients_kernel/EquipmentType.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/Ghost_ABC.h"
#include "clients_kernel/Infrastructure_ABC.h"
#include "clients_kernel/Karma.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/LogisticLevel.h"
#include "clients_kernel/ObjectTypes.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "clients_kernel/Tools.h"
#include "clients_kernel/UrbanObject_ABC.h"
#include "clients_kernel/DictionaryExtensions.h"
#include "ENT/ENT_Tr_Gen.h"
#include "tools/GeneralConfig.h"
#include "tools/RealFileLoaderObserver_ABC.h"
#include "tools/SchemaWriter.h"
#pragma warning( push, 0 )
#include <boost/algorithm/string.hpp>
#pragma warning( pop )
#include <boost/lexical_cast.hpp>
#include <boost/foreach.hpp>
#include <boost/format.hpp>
#include <xeuseuleu/xsl.hpp>

using namespace kernel;
using namespace tools;

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker constructor
// Created: ABR 2011-09-22
// -----------------------------------------------------------------------------
ModelConsistencyChecker::ModelConsistencyChecker( const Model& model, const ::StaticModel& staticModel, Controllers& controllers, tools::RealFileLoaderObserver_ABC& fileLoaderObserver )
    : model_      ( model )
    , staticModel_( staticModel )
    , controllers_( controllers )
    , fileLoaderObserver_( fileLoaderObserver )
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
bool ModelConsistencyChecker::CheckConsistency()
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
    CheckLogisticSubordinates();

    CheckProfileUniqueness();
    CheckProfileInitialization();
    CheckProfileNumberOfElements();

    CheckGhosts();

    CheckCommandPosts();

    CheckKnowledgeGroups();
    CheckLongNameSize();
    CheckLoadingErrors();
    CheckScores();
    CheckSuccessFactors();
    CheckUrban();
    CheckOrbat();
    CheckFiles();
    CheckDiffusionList();

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
                for( CIT_Items safeIt = error.items_.begin(); safeIt != error.items_.end(); ++safeIt )
                    if( **safeIt == &entity1 )
                    {
                        bFound = true;
                        break;
                    }
                if( !bFound )
                    error.items_.push_back( new SafePointer< Entity_ABC >( controllers_, &entity1 ) );
                error.items_.push_back( new SafePointer< Entity_ABC >( controllers_, &entity2 ) );
                iter = entities_.erase( iter );
            }
            else
                ++iter;
        }
        if( !error.items_.empty() )
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
        else if( stocks && !stocks->GetInvalidDotations().empty() )
            for( std::vector< std::string >::const_iterator itDotation = stocks->GetInvalidDotations().begin(); itDotation != stocks->GetInvalidDotations().end(); ++itDotation )
                AddError( eStockInvalidDotation, &agent, *itDotation );
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
                            const kernel::EquipmentType& equipmentType = staticModel.objectTypes_.Resolver2< kernel::EquipmentType >::Get( agentComposition.GetType().GetId() );
                            tools::Iterator< const kernel::DotationCapacityType& > resourcesIterator = equipmentType.CreateResourcesIterator();
                            while( resourcesIterator.HasMoreElements() ) // for each resource in this equipment
                            {
                                const kernel::DotationCapacityType& dotationCapacity = resourcesIterator.NextElement();
                                const kernel::DotationType& category = staticModel.objectTypes_.kernel::Resolver2< kernel::DotationType >::Get( dotationCapacity.GetName() );
                                E_StockCategory stockCategory = tools::StockCategoryFromDotationFamily( static_cast< E_DotationFamily >( category.GetFamily() ), category.IsDType() );
                                std::string stockCategoryName = ENT_Tr::ConvertFromStockCategory( stockCategory );

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
                const EquipmentType& equipmentType = staticModel_.objectTypes_.Resolver2< EquipmentType >::Get( equipment.GetId() );
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
// Name: ModelConsistencyChecker::CheckProfileNumberOfElements
// Created: JSR 2012-06-29
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckProfileNumberOfElements()
{
    ProfilesModel::T_Profiles profiles;
    model_.profiles_.Visit( profiles );
    for( ProfilesModel::T_Profiles::const_iterator it = profiles.begin(); it != profiles.end(); ++it )
    {
        const UserProfile* profile = model_.profiles_.Find( *it );
        if( profile )
        {
            if( profile->GetAutomatAndPopulationsProfilesCount() > 12 )
                AddError( eProfileNumberTooHigh, 0, *it );
            if( profile->GetUserRole().empty() )
                AddError( eProfileNoRole, 0, *it );
        }
    }
}

namespace
{
    bool compare_ghosts ( const Ghost_ABC* ghost1, const Ghost_ABC* ghost2 )
    {
        return ghost1->GetType().toAscii().constData() < ghost2->GetType().toAscii().constData();
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckGhosts
// Created: ABR 2011-10-20
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckGhosts()
{
    Iterator< const Ghost_ABC& > it = model_.ghosts_.CreateIterator();
    std::list< const Ghost_ABC* > convertedEntities;
    while( it.HasMoreElements() )
    {
        const Ghost_ABC& ghost = it.NextElement();
        if( ghost.IsConverted() )
            convertedEntities.push_back( &ghost );
        else
            AddError( eGhostExistence, &ghost );
    }
    convertedEntities.sort( compare_ghosts );
    for ( std::list< const Ghost_ABC* >::const_iterator cit = convertedEntities.begin(); cit != convertedEntities.end(); ++cit )
        AddError( eGhostConverted, *cit, (*cit)->GetType().toAscii().constData() );
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckLongNameSize
// Created: MMC 2011-12-08
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckLongNameSize()
{
    FillEntitiesCopy( eLongNameUniqueness );

    for( std::vector< const Entity_ABC* >::iterator it = entities_.begin(); it != entities_.end(); ++it )
    {
        const Entity_ABC& entity = **it;
        const TacticalHierarchies* pTacticalHierarchies = entity.Retrieve< TacticalHierarchies >();
        if( !pTacticalHierarchies )
            continue;
        if( gui::LongNameHelper::GetEntityLongName( entity ).empty() )
            continue;

        const Diplomacies_ABC* diplo = pTacticalHierarchies->GetTop().Retrieve< Diplomacies_ABC >();
        if( diplo && !( diplo->GetKarma() == Karma::unknown_ ) )
        {
            int maxSize = diplo->GetKarma() == Karma::enemy_ ? 15 : 17;
            std::size_t curSize = gui::LongNameHelper::GetEntityLongName( entity ).size();
            if( curSize > maxSize )
                AddError( eLongNameSize, &entity, boost::str( boost::format( "%d > %d" ) % curSize % maxSize ) );
        }
    }
}

namespace
{
    bool IsCommandPost( const Entity_ABC& entity )
    {
        if( const CommandPostAttributes_ABC* pAttributes = entity.Retrieve< CommandPostAttributes_ABC >() )
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
            const Entity_ABC& entity = itChild.NextElement();
            if( IsCommandPost( entity ) )
                ++commandPostCount;
            else
            {
                const kernel::DictionaryExtensions* ext = entity.Retrieve< kernel::DictionaryExtensions>();
                if( ext && ext->IsEnabled() )
                    if( !ext->GetValue( "TypePC" ).empty() || !ext->GetValue( "TypeSIOC" ).empty() )
                        AddError( eBadCPExtensions, &entity );
            }
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
// Name: ModelConsistencyChecker::CheckLogisticSubordinates
// Created: LDC 2012-09-13
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckLogisticSubordinates()
{
    Iterator< const Automat_ABC& > it = model_.GetAutomatResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Automat_ABC& automat = it.NextElement();
        const kernel::AutomatType& type = automat.GetType();
        if( !type.HasLogistics() )
            continue;
        if( automat.GetLogisticLevel() != kernel::LogisticLevel::logistic_base_ )
            continue;
        LogisticBaseStates* hierarchy = const_cast< LogisticBaseStates* >( static_cast< const LogisticBaseStates* >( automat.Retrieve< LogisticHierarchiesBase >() ) );
        if( hierarchy->CleanBadSubordinates() )
        {
            AddError( eBadLogisticSubordinate, &automat );
            const_cast< Model& >( model_ ).SetConsistencyErrorsOnLoad();
        }
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckUrban
// Created: JSR 2012-06-28
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckUrban()
{
    std::set< std::string > unknownNetworks;
    Iterator< const kernel::UrbanObject_ABC& > it = model_.GetUrbanObjectResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::UrbanObject_ABC& object = it.NextElement();
        if( const kernel::Infrastructure_ABC* infrastructure = object.Retrieve< kernel::Infrastructure_ABC >() )
            if( !infrastructure->GetInvalidType().empty() )
                AddError( eUnknownInfrastructure, &object, infrastructure->GetInvalidType() );
        if( const ResourceNetworkAttribute* network = static_cast< const ResourceNetworkAttribute* >( object.Retrieve< kernel::ResourceNetwork_ABC >() ) )
            if( !network->GetInvalidResources().empty() )
                for( std::set< std::string >::const_iterator itResource = network->GetInvalidResources().begin(); itResource != network->GetInvalidResources().end(); ++itResource )
                    unknownNetworks.insert( *itResource );
    }
    Iterator< const kernel::Object_ABC& > itObject = model_.GetObjectResolver().CreateIterator();
    while( itObject.HasMoreElements() )
    {
        const kernel::Object_ABC& object = itObject.NextElement();
        if( const ResourceNetworkAttribute* network = static_cast< const ResourceNetworkAttribute* >( object.Retrieve< kernel::ResourceNetwork_ABC >() ) )
            if( !network->GetInvalidResources().empty() )
                for( std::set< std::string >::const_iterator itResource = network->GetInvalidResources().begin(); itResource != network->GetInvalidResources().end(); ++itResource )
                    unknownNetworks.insert( *itResource );
    }
    for( std::set< std::string >::const_iterator itNetwork = unknownNetworks.begin(); itNetwork != unknownNetworks.end(); ++ itNetwork )
        AddError( eUnknownResourceNetwork, 0, *itNetwork );
    if( model_.urban_.TakeLinkErrors() )
    {
        AddError( eDeletedUrbanBlocks, 0, "" );
        const_cast< Model& >( model_ ).SetConsistencyErrorsOnLoad();
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckOrbat
// Created: ABR 2012-02-27
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckOrbat()
{
    Iterator< const Team_ABC& > it = model_.GetTeamResolver().CreateIterator();
    if( !it.HasMoreElements() )
        AddError( eNoOrbat, 0 );
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckFiles
// Created: MMC 2012-06-01
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckFiles()
{
    std::vector< std::string > filesErrors;
    fileLoaderObserver_.GetErrors( filesErrors );
    for( int i = 0; i < filesErrors.size(); ++i )
        AddError( eOthers, 0, filesErrors[ i ] );
    fileLoaderObserver_.Purge();
}

namespace
{
    bool IsReceiver( const kernel::Entity_ABC& entity )
    {
        kernel::DictionaryExtensions* dictionary = const_cast< kernel::DictionaryExtensions* >( entity.Retrieve< kernel::DictionaryExtensions >() );
        if( !dictionary )
            return false;
        kernel::ExtensionType* type = dictionary->GetExtensionTypes().tools::StringResolver< kernel::ExtensionType >::Find( "orbat-attributes" );
        if( !type || dictionary->GetExtensions().size() == 0 )
            return false;
        if( !dictionary->GetValue( "TypePC" ).empty() && dictionary->GetValue( "TypePC" ) != "PasCorresp" )
            return true;
        return false;
    }

    bool IsTransmitter( const kernel::Entity_ABC& entity )
    {
        if( const kernel::DictionaryExtensions* dictionary = entity.Retrieve< kernel::DictionaryExtensions >() )
        {
            const std::string name = dictionary->GetExtensionTypes().GetNameByType( kernel::AttributeType::ETypeDiffusionList );
            kernel::ExtensionType* type = dictionary->GetExtensionTypes().tools::StringResolver< kernel::ExtensionType >::Find( "orbat-attributes" );
            if( !type )
                return false;
            kernel::AttributeType* attribute = type->tools::StringResolver< kernel::AttributeType >::Find( name );
            if( !attribute )
                return false;
            if( attribute->IsActive( dictionary->GetExtensions() ) )
                return true;
        }
        return false;
    }

    std::vector< std::string > GetReceivers( const kernel::Agent_ABC& transmitter )
    {
        std::vector< std::string > list;
        if( const kernel::DictionaryExtensions* extension = transmitter.Retrieve< kernel::DictionaryExtensions >() )
        {
            const std::string name = extension->GetExtensionTypes().GetNameByType( kernel::AttributeType::ETypeDiffusionList );
            const std::string value = extension->GetValue( name );
            boost::split( list, value, boost::algorithm::is_any_of( ";" ) );
        }
        return list;
    }

    bool IsValid( const kernel::Agent_ABC& transmitter, const kernel::Agent_ABC& entity, tools::Resolver_ABC< kernel::Agent_ABC >& resolver )
    {
        if( IsTransmitter( transmitter ) )
            BOOST_FOREACH( const std::string id, GetReceivers( transmitter ) )
                if( id != "" )
                    if( const kernel::Agent_ABC* agent = resolver.Find( boost::lexical_cast< unsigned int >( id ) ) )
                    {
                        if( entity.GetId() == agent->GetId() )
                            return false;
                        if( !IsValid( *agent, entity, resolver ) )
                            return false;
                    }
        return true;
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::CheckDiffusionList
// Created: LGY 2012-09-06
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::CheckDiffusionList()
{
    Iterator< const Agent_ABC& > it = model_.GetAgentResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const Agent_ABC& agent = it.NextElement();
        if( !IsValid( agent, agent, model_.GetAgentResolver() ) )
            AddError( eDiffusionList, &agent );
    }
}

// -----------------------------------------------------------------------------
// Name: ModelConsistencyChecker::AddError
// Created: ABR 2012-06-06
// -----------------------------------------------------------------------------
void ModelConsistencyChecker::AddError( E_ConsistencyCheck type, const kernel::Entity_ABC* entity, const std::string& optional /*= ""*/ )
{
    kernel::SafePointer< kernel::Entity_ABC >* safePtr = new kernel::SafePointer< kernel::Entity_ABC >( controllers_, entity );
    T_Parent::AddError( type, safePtr, optional );
}
