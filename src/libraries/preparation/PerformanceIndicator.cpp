// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#include "preparation_pch.h"
#include "PerformanceIndicator.h"
#include "Model.h"
#include "AgentsModel.h"
#include "KnowledgeGroupsModel.h"
#include "ObjectsModel.h"
#include "ProfilesModel.h"
#include "TeamsModel.h"
#include "UrbanModel.h"
#include "preparation/Agent.h"
#include "preparation/Automat.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/Automat_ABC.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Formation_ABC.h"
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/KnowledgeGroup_ABC.h"
#include "clients_kernel/Object_ABC.h"
#include "clients_kernel/ObjectType.h"
#include "clients_kernel/Population_ABC.h"
#include "clients_gui/EntityType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Team_ABC.h"
#include "tools/Loader_ABC.h"
#include "tools/ExerciseConfig.h"
#include <boost/bind.hpp>
#include <xeumeuleu/xml.hpp>
#include <xeuseuleu/xsl.hpp>

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator constructor
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
PerformanceIndicator::PerformanceIndicator( const Model& model )
    : model_( model )
    , limit_( 10.f )
    , globalFactor_( 0.001f )
    , unit_( 0.f )
    , urban_( 0.f )
    , object_( 0.f )
    , terrain_( 0.f )
    , knowledge_( 0.f )
    , unitknowledge_( 0.f )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator destructor
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
PerformanceIndicator::~PerformanceIndicator()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::Load
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
void PerformanceIndicator::Load( const tools::ExerciseConfig& config, const tools::Path& file )
{
    values_.exercise_ = config.GetExerciseName();
    values_.limit_ = static_cast< unsigned int >( limit_ );
    values_.terrainWidth_ = static_cast< unsigned int >( config.GetTerrainWidth() / 1000.f );
    values_.terrainHeight_ = static_cast< unsigned int >( config.GetTerrainHeight() / 1000.f );
    float terrainMemSize = 0.f;
    try
    {
        tools::Path detectionPath = config.GetDetectionDirectory();
        if( detectionPath.IsDirectory() )
            for( auto it = detectionPath.begin(); it != detectionPath.end(); ++it )
                if( !it->IsDirectory() && it->Extension() == ".dat" )
                    terrainMemSize += static_cast< float >( it->FileSize() );
        terrainMemSize += static_cast< float >( config.GetPathfindGraphFile().FileSize() );
        terrainMemSize += static_cast< float >( config.GetPathfindLinksFile().FileSize() );
        terrainMemSize += static_cast< float >( config.GetPathfindNodesFile().FileSize() );
        if( file.Exists() )
        {
            config.GetLoader().CheckFile( file );
            config.GetLoader().LoadFile( file, boost::bind( &PerformanceIndicator::Read, this, _1 ) );
        }
    }
    catch( ... )
    {
        // NOTHING
    }
    values_.terrainLoad_ = terrainMemSize / 1000000.f;
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::Read
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
void PerformanceIndicator::Read( xml::xistream& xis )
{
    xis >> xml::start( "parameters" )
        >> xml::attribute( "limit", limit_ )
        >> xml::attribute( "globalfactor", globalFactor_ )
        >> xml::attribute( "unit", unit_ )
        >> xml::attribute( "urban", urban_ )
        >> xml::attribute( "object", object_ )
        >> xml::attribute( "terrain", terrain_ )
        >> xml::attribute( "knowledge", knowledge_ )
        >> xml::attribute( "unitknowledge", unitknowledge_ )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::ComputeValues
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
const PerformanceIndicator::Values& PerformanceIndicator::ComputeValues()
{
    Update();
    return values_;
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::Update
// Created: MMC 2012-02-02
// -----------------------------------------------------------------------------
void PerformanceIndicator::Update()
{
    Values values;
    TeamGetInfoFunctor teamsGetInfo( values );
    ObjectGetInfoFunctor objectsGetInfo( values );
    KGGetInfoFunctor knowledgeGroupsGetInfo( values );
    model_.teams_->Apply( teamsGetInfo );
    model_.objects_->Apply( objectsGetInfo );
    model_.knowledgeGroups_->Apply( knowledgeGroupsGetInfo );
    UpdatePopulation( values );
    UpdateCrowds( values );
    values.exercise_        = values_.exercise_;
    values.terrainLoad_     = values_.terrainLoad_;
    values.terrainWidth_    = values_.terrainWidth_;
    values.terrainHeight_   = values_.terrainHeight_;
    values.limit_           = static_cast< unsigned int >( limit_ );
    values.profiles_        = model_.profiles_->GetProfilesCount();
    values.teams_           = model_.teams_->tools::Resolver< kernel::Team_ABC >::Count();
    values.units_           = model_.agents_->tools::Resolver< kernel::Agent_ABC >::Count();
    values.automats_        = model_.agents_->tools::Resolver< kernel::Automat_ABC >::Count();
    values.objects_         = model_.objects_->tools::Resolver< kernel::Object_ABC >::Count();
    values.blocs_           = model_.urban_->tools::Resolver< kernel::UrbanObject_ABC >::Count();
    values.crowds_          = model_.agents_->tools::Resolver< kernel::Population_ABC >::Count();
    values.populations_     = model_.agents_->tools::Resolver< kernel::Inhabitant_ABC >::Count();
    values.knowledgeGroups_ = model_.knowledgeGroups_->tools::Resolver< kernel::KnowledgeGroup_ABC >::Count();
    values.avgAutomatsKG_   = values.knowledgeGroups_? values.automatsKG_ / values.knowledgeGroups_ : 0;
    values.avgUnitsKG_      = values.knowledgeGroups_? values.unitsKG_ / values.knowledgeGroups_ : 0;
    values.performance_     = ComputeFormulaIndicator( values );
    values_                 = values;
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::ComputeFormulaIndicator
// Created: MMC 2012-03-06
// -----------------------------------------------------------------------------
float PerformanceIndicator::ComputeFormulaIndicator( Values& values )
{
    float units = static_cast< float >( values.units_ );
    float blocs = static_cast< float >( values.blocs_ );
    float objects = static_cast< float >( values.objects_ );
    float terrain = values.terrainLoad_;
    float knowledges = static_cast< float >( values.knowledgeGroups_ );
    if( knowledges < 1.f )
        knowledges = 1.f;
    float load  = units * ( unit_ + blocs * urban_ ) + objects * object_ + terrain * terrain_ + knowledge_ * ( knowledges + unitknowledge_ * units / knowledges );
    load *= globalFactor_;
    if( load < 0.0f )
        return 0.0f;
    return load;
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::TeamGetInfoFunctor::operator
// Created: MMC 2012-03-06
// -----------------------------------------------------------------------------
void PerformanceIndicator::TeamGetInfoFunctor::operator()( const kernel::Team_ABC& team ) const
{
    TeamData& teamData = values_.teamsDatas_[ team.GetId() ];
    teamData.name_ = team.GetName();
    Compute( teamData, team );
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::TeamGetInfoFunctor::Compute
// Created: MMC 2012-03-06
// -----------------------------------------------------------------------------
void PerformanceIndicator::TeamGetInfoFunctor::Compute( TeamData& teamData, const kernel::Entity_ABC& entity ) const
{
    const kernel::TacticalHierarchies& hierarchies = entity.Get< kernel::TacticalHierarchies >();
    tools::Iterator< const kernel::Entity_ABC& > itSub = hierarchies.CreateSubordinateIterator();
    while( itSub.HasMoreElements() )
    {
        const kernel::Entity_ABC& entity = itSub.NextElement();
        if( dynamic_cast< const kernel::Formation_ABC* >( &entity ) )
        {
            teamData.formations_++;
            Compute( teamData, entity );
        }
        else if( dynamic_cast< const kernel::Automat_ABC* >( &entity ) )
        {
            teamData.automats_++;
            const Automat* pAutomat = dynamic_cast< const Automat* >( &entity );
            if( pAutomat )
                teamData.automatTypes_[ pAutomat->Get< gui::EntityType< kernel::AutomatType > >().GetType().GetTypeName() ]++;
            Compute( teamData, entity );
        }
        else if( dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
        {
            teamData.units_++;
            const Agent* pAgent = dynamic_cast< const Agent* >( &entity );
            if( pAgent )
                teamData.unitTypes_[ pAgent->GetType().GetTypeName() ]++;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::ObjectGetInfoFunctor::operator
// Created: MMC 2012-03-06
// -----------------------------------------------------------------------------
void PerformanceIndicator::ObjectGetInfoFunctor::operator()( const kernel::Object_ABC& object ) const
{
    const kernel::TacticalHierarchies& hierarchies = object.Get< kernel::TacticalHierarchies >();
    const kernel::Team_ABC* pTeam = dynamic_cast< const kernel::Team_ABC* >( &hierarchies.GetTop() );
    if( pTeam )
    {
        TeamData& teamData = values_.teamsDatas_[ pTeam->GetId() ];
        teamData.name_ = pTeam->GetName();
        teamData.objects_++;
        teamData.objectTypes_[ object.GetType().GetType() ]++;
    }
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::KGGetInfoFunctor::operator
// Created: MMC 2012-03-06
// -----------------------------------------------------------------------------
void PerformanceIndicator::KGGetInfoFunctor::operator()( const kernel::KnowledgeGroup_ABC& kg ) const
{
    unsigned int unitCount = 0, automatCount = 0;
    tools::Iterator< const kernel::Entity_ABC& > it = kg.Get< kernel::CommunicationHierarchies >().CreateSubordinateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC* entity = &it.NextElement();
        if( entity && dynamic_cast< const kernel::Automat_ABC* >( entity ) )
        {
            ++automatCount;
            tools::Iterator< const kernel::Entity_ABC& > itSub = entity->Get< kernel::TacticalHierarchies >().CreateSubordinateIterator();
            while( itSub.HasMoreElements() )
            {
                const kernel::Entity_ABC* subEntity = &itSub.NextElement();
                if( dynamic_cast< const kernel::Agent_ABC* >( subEntity ) )
                    ++unitCount;
            }
        }
    }

    values_.maxAutomatsKG_  = std::max( automatCount, values_.maxAutomatsKG_ );
    values_.maxUnitsKG_     = std::max( unitCount, values_.maxUnitsKG_ );
    values_.automatsKG_     += automatCount;
    values_.unitsKG_        += unitCount;

    const kernel::CommunicationHierarchies& hierarchies = kg.Get< kernel::CommunicationHierarchies >();
    const kernel::Team_ABC* pTeam = dynamic_cast< const kernel::Team_ABC* >( &hierarchies.GetTop() );
    if( pTeam )
    {
        TeamData& teamData = values_.teamsDatas_[ pTeam->GetId() ];
        teamData.name_ = pTeam->GetName();
        teamData.knowledgeGroups_++;
        KnowledgeGroupData& dataKG = teamData.datasKG_[ kg.GetName().toStdString() ];
        dataKG.automats_    += automatCount;
        dataKG.units_       += unitCount;
    }
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::UpdatePopulation
// Created: MMC 2012-03-06
// -----------------------------------------------------------------------------
void PerformanceIndicator::UpdatePopulation( Values& values ) const
{
    tools::Iterator< const kernel::Inhabitant_ABC& > it = model_.GetInhabitantResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Inhabitant_ABC& entity = it.NextElement();
        const kernel::TacticalHierarchies& hierarchies = entity.Get< kernel::TacticalHierarchies >();
        const kernel::Team_ABC* pTeam = dynamic_cast< const kernel::Team_ABC* >( &hierarchies.GetTop() );
        if( pTeam )
        {
            TeamData& teamData = values.teamsDatas_[ pTeam->GetId() ];
            teamData.name_ = pTeam->GetName();
            teamData.populations_++;
        }
    }
}

// -----------------------------------------------------------------------------
// Name: PerformanceIndicator::UpdateCrowds
// Created: MMC 2012-03-06
// -----------------------------------------------------------------------------
void PerformanceIndicator::UpdateCrowds( Values& values ) const
{
    tools::Iterator< const kernel::Population_ABC& > it = model_.GetPopulationResolver().CreateIterator();
    while( it.HasMoreElements() )
    {
        const kernel::Entity_ABC* entity = static_cast< const kernel::Entity_ABC* >( &it.NextElement() );
        const kernel::TacticalHierarchies& hierarchies = entity->Get< kernel::TacticalHierarchies >();
        const kernel::Team_ABC* pTeam = dynamic_cast< const kernel::Team_ABC* >( &hierarchies.GetTop() );
        if( pTeam )
        {
            TeamData& teamData = values.teamsDatas_[ pTeam->GetId() ];
            teamData.name_ = pTeam->GetName();
            teamData.crowds_++;
        }
    }
}
