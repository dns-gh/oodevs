// Created: JVT 02-06-28
//*****************************************************************************

#include "simulation_kernel_pch.h"
#include "DEC_Workspace.h"
#include "DEC_Model.h"
#include "Entities/Populations/DEC_PopulationDecision.h"
#include "Entities/Agents/Perceptions/PHY_PerceptionView.h"
#include "Entities/Agents/Roles/Composantes/PHY_RoleInterface_Composantes.h"
#include "Entities/Orders/MIL_PionMissionType.h"
#include "Entities/Orders/MIL_AutomateMissionType.h"
#include "Entities/Orders/MIL_AutomateMission.h"
#include "Entities/Orders/MIL_PopulationMissionType.h"
#include "Entities/Orders/MIL_PopulationMission.h"
#include "Entities/Orders/MIL_PopulationOrderManager.h"
#include "Entities/Orders/MIL_FragOrderType.h"
#include "Entities/Orders/MIL_FragOrder.h"
#include "Entities/Orders/MIL_ParameterType_ABC.h"
#include "Decision/DEC_DIAFunctions.h"
#include "Decision/DEC_GeometryFunctions.h"
#include "Decision/DEC_ObjectFunctions.h"
#include "Decision/DEC_Tools.h"
#include "Knowledge/DEC_Knowledge_Agent.h"
#include "Knowledge/DEC_Knowledge_RapFor_ABC.h"
#include "MIL_AgentServer.h"
#include "tools/Codec.h"
#include "tools/Loader_ABC.h"
#include "tools/MIL_AffinitiesMap.h"
#include "tools/MIL_Config.h"
#include "tools/PhyLoader.h"
#include "tools/VersionHelper.h"
#include "tools/XmlStreamOperators.h"
#include "ENT/ENT_Tr.h"
#include <boost/algorithm/string.hpp>
#include <sys/stat.h>

namespace
{
    const std::string strUnits( "units" );
    const std::string strAutomats( "automats" );
    const std::string strPopulation( "populations" );
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace constructor
// Created: JVT 02-06-28
// Last modified: JVT 02-09-16
//-----------------------------------------------------------------------------
DEC_Workspace::DEC_Workspace( MIL_Config& config )
{
    MT_LOG_INFO_MSG( "Initializing decision" );
    modelTypes_[strUnits] = &pionModels_;
    modelTypes_[strAutomats] = &automateModels_;
    modelTypes_[strPopulation] = &populationModels_;
    InitializeConfig( config );
    InitializeObjectNames( config );
    InitializeDIA   ( config );
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace destructor
// Created: JVT 02-07-02
// Last modified: AGN 02-11-05
//-----------------------------------------------------------------------------
DEC_Workspace::~DEC_Workspace()
{
    for( auto it = automateModels_.begin(); it != automateModels_.end(); ++it )
        delete it->second;
    for( auto it = pionModels_.begin(); it != pionModels_.end(); ++it )
        delete it->second;
    for( auto it = populationModels_.begin(); it != populationModels_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeConfig
// Created: NLD 2004-06-14
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeConfig( MIL_Config& config )
{
    const unsigned int tickDuration = config.GetTimeStep();
    config.GetPhyLoader().LoadPhysicalFile( "decisional", boost::bind( &DEC_Workspace::LoadDecisional, this, _1, tickDuration ) );
}

namespace
{
    const PHY_PerceptionLevel* ConvertFromLevel( const std::string& level )
    {
        if( level == "never" )
            return &PHY_PerceptionLevel::notSeen_;
        if( level == "detection" )
            return &PHY_PerceptionLevel::detected_;
        if( level == "recognition" )
            return &PHY_PerceptionLevel::recognized_;
        return &PHY_PerceptionLevel::identified_;
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::LoadDecisional
// Created: LDC 2010-12-01
// -----------------------------------------------------------------------------
void DEC_Workspace::LoadDecisional( xml::xistream& xisDecisional,
        unsigned int tickDuration )
{
    unsigned int nTmp;

    xisDecisional >> xml::start( "decisional" )
                      >> xml::start( "dangerosity-modifiers" )
                          >> xml::attribute( "max-accuracy", nTmp );
    if( nTmp < 0 || nTmp > 100 )
        throw MASA_EXCEPTION( "dangerosity-modifiers: max-accuracy not in [0..100]" );
    DEC_Knowledge_Agent::rMaxDangerosityDegradationByRelevance_ = nTmp / 100.;

    xisDecisional >> xml::attribute( "max-operational-state", nTmp );
    if( nTmp < 0 || nTmp > 100 )
        throw MASA_EXCEPTION( "dangerosity-modifiers: max-operationnal-state not in [0..100]" );
    DEC_Knowledge_Agent     ::rMaxDangerosityDegradationByOpState_ = nTmp / 100.;
    PHY_RoleInterface_Composantes::rMaxDangerosityDegradationByOpState_ = nTmp / 100.;

    xisDecisional >> xml::attribute( "max-neutralized-state", nTmp );
    if( nTmp < 0 || nTmp > 100 )
        throw MASA_EXCEPTION( "dangerousity-modifiers: max-neutralized-state not in [0..100]" );
    DEC_Knowledge_Agent     ::rMaxDangerosityDegradationByNeutralizedState_ = nTmp / 100.;
    PHY_RoleInterface_Composantes::rMaxDangerosityDegradationByNeutralizedState_ = nTmp / 100.;

    xisDecisional >> xml::end
                  >> xml::start( "operational-state-weights" )
                      >> xml::attribute( "component", PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ )
                      >> xml::attribute( "major-component", PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ )
                      >> xml::attribute( "crew", PHY_ComposantePion::rOpStateWeightHumans_ )
                      >> xml::attribute( "threshold", PHY_RoleInterface_Composantes::rOpStateDecisionalThreshold_ )
                  >> xml::end;

    xisDecisional >> xml::optional >> xml::start( "urban-perception" )
                      >> xml::attribute( "view-factor", PHY_PerceptionView::nUrbanCoefficient_ )
                  >> xml::end;

    xisDecisional >> xml::start( "critical-intelligence-delay" );
    tools::ReadTimeAttribute( xisDecisional, "minimum-affinity", MIL_AffinitiesMap::interrogateDelayForMinimumAffinity_ );
    tools::ReadTimeAttribute( xisDecisional, "neutral-affinity", MIL_AffinitiesMap::interrogateDelayForNeutralAffinity_ );
    tools::ReadTimeAttribute( xisDecisional, "maximum-affinity", MIL_AffinitiesMap::interrogateDelayForMaximumAffinity_ );
    xisDecisional >> xml::end;
    
    DEC_GeometryFunctions::ReadRecoAndSearchSpeeds( xisDecisional, tickDuration );

    if( PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ < 0 || PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ > 1 )
        throw MASA_EXCEPTION( "operational-state-weights: component not in [0..1]" );
    if( PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ < 0 || PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ > 1 )
        throw MASA_EXCEPTION( "operational-state-weights: major-component not in [0..1]" );
    if( PHY_ComposantePion::rOpStateWeightHumans_ < 0 || PHY_ComposantePion::rOpStateWeightHumans_ > 1 )
        throw MASA_EXCEPTION( "operational-state-weights: crew not in [0..1]" );

    if( PHY_RoleInterface_Composantes::rOpStateWeightMajorComposante_ + PHY_RoleInterface_Composantes::rOpStateWeightNonMajorComposante_ != 1. )
        throw MASA_EXCEPTION( "Sum of 'Decisionnel::EtatOps::PoidsComposantesMajeures', 'PoidsComposantesMajeures' and 'PoidsPersonnel' != 1" ); // $$$$ ABL 2007-07-25: error context

    DEC_Knowledge_RapFor_ABC::Initialize( xisDecisional, tickDuration );

    std::string maxHostilePerceptionLevel;
    xisDecisional >> xml::optional >> xml::start( "perception" )
                    >> xml::attribute( "detect-destroyed-units", DEC_Knowledge_Agent::detectDestroyedUnits_ )
                    >> xml::optional >> xml::attribute( "max-level", maxHostilePerceptionLevel )
                      >> xml::list( "availability",
                        [&]( xml::xistream& xis )
                        {
                            const auto type = ENT_Tr::ConvertToPerceptionType( xis.attribute< std::string >( "type" ) );
                            if( type != eNbrPerceptionType )
                                DEC_Knowledge_Agent::perceptionInfoAvailability_[ type ] =
                                    ConvertFromLevel( xis.attribute< std::string >( "level" ) );
                        } )
                  >> xml::end;
    DEC_Knowledge_Agent::maxHostilePerceptionLevel_ = ConvertFromLevel( maxHostilePerceptionLevel );

    xisDecisional >> xml::end;
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIA
// Created: JVT 02-07-08
// Last modified: JVT 03-11-24
//-----------------------------------------------------------------------------
void DEC_Workspace::InitializeDIA( MIL_Config& config )
{
    //DIA4
    config.GetLoader().LoadFile( config.GetDecisionalFile(), boost::bind( &DEC_Workspace::LoadDIA, this, boost::ref( config ), _1 ) );
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeDIA
// Created: JVT 02-07-08
// Last modified: JVT 03-11-24
//-----------------------------------------------------------------------------
void DEC_Workspace::LoadDIA( MIL_Config& config, xml::xistream& xis )
{
    std::string decisionalVersion;
    xis >> xml::start( "decisional" )
        >> xml::optional >> xml::attribute( "model-version", decisionalVersion );

    const std::string runtimeVersion = tools::AppProjectVersion();
    if( !tools::CheckVersion( decisionalVersion, runtimeVersion, 2u ) )
        throw MASA_EXCEPTION( "Decisional model version (" + decisionalVersion + ") does not match runtime version (" + runtimeVersion + ")" );

    std::map< std::string, tools::Path > strSourcePaths;
    xis >> xml::start( "RepertoiresSources" )
            >> xml::list( "RepertoireSources" , *this, &DEC_Workspace::RegisterSourcePath, config, strSourcePaths )
        >> xml::end;

    MIL_ParameterType_ABC::Initialize();

    InitializeMissions( config );
    InitializeModels  ( config, strSourcePaths );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::RegisterSourcePath
// Created: MGD 2010-03-17
// -----------------------------------------------------------------------------
void DEC_Workspace::RegisterSourcePath( xml::xistream& xis, MIL_Config& config, std::map< std::string, tools::Path >& paths )
{
    std::string nameSpace;
    tools::Path dir;
    xis >> xml::attribute( "namespace", nameSpace )
        >> xml::attribute( "directory", dir );

    paths.insert( std::pair< std::string, tools::Path >( nameSpace, config.BuildDecisionalChildFile( dir ) ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeMissions
// Created: NLD 2006-11-24
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeMissions( MIL_Config& config )
{
    config.GetPhyLoader().LoadPhysicalFile( "missions", boost::bind( &DEC_Workspace::LoadMissions, this, _1 ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::LoadMissions
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void DEC_Workspace::LoadMissions( xml::xistream& xisMission )
{
    MIL_PionMissionType      ::Initialize( xisMission );
    MIL_AutomateMissionType  ::Initialize( xisMission );
    MIL_PopulationMissionType::Initialize( xisMission );
    MIL_FragOrderType        ::Initialize( xisMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeModels
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeModels( MIL_Config& config, const std::map< std::string, tools::Path >& strSourcePaths )
{
    config.GetPhyLoader().LoadPhysicalFile( "models", boost::bind(
        &DEC_Workspace::LoadModels, this, _1, boost::cref( strSourcePaths ), config.GetIntegrationDir() ) );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::LoadModels
// Created: LDC 2010-12-02
// -----------------------------------------------------------------------------
void DEC_Workspace::LoadModels( xml::xistream& xisModels, const std::map< std::string, tools::Path >& strSourcePaths,
                                const tools::Path& integrationDir )
{
    xisModels >> xml::start( "models" );

    // Pions
    MT_LOG_INFO_MSG( "Initializing unit DIA models" );
    xisModels >> xml::start( "units" )
                >> xml::list( "unit", *this, &DEC_Workspace::ReadModel, strSourcePaths, strUnits,
                              MIL_PionMissionType::MissionNames(), integrationDir )
              >> xml::end;

    // Automates
    MT_LOG_INFO_MSG( "Initializing automat DIA models" );
    xisModels >> xml::start( "automats" )
                  >> xml::list( "automat", *this, &DEC_Workspace::ReadModel, strSourcePaths, strAutomats,
                                MIL_AutomateMissionType::MissionNames(), integrationDir )
              >> xml::end;

    // Populations
    MT_LOG_INFO_MSG( "Initializing population DIA models" );
    xisModels >> xml::start( "crowd" )
                  >> xml::list( "crowd", *this, &DEC_Workspace::ReadModel, strSourcePaths, strPopulation,
                                MIL_PopulationMissionType::MissionNames(), integrationDir )
              >> xml::end;

    xisModels >> xml::end; // models
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::ReadModel
// Created: RDS 2008-05-21
// -----------------------------------------------------------------------------
void DEC_Workspace::ReadModel( xml::xistream& xis, const std::map< std::string, tools::Path >& strSourcePaths,
                               const std::string& strEntityType, const T_MissionTypeNameMap& missionTypes,
                               const tools::Path& integrationDir )
{
    std::string strName;
    xis >> xml::attribute( "name", strName );

    T_ModelMap* pModels = modelTypes_[strEntityType];
    if( !pModels )
        throw MASA_EXCEPTION( xis.context() + "Unknown model type" );
    const DEC_Model_ABC*& pModel = (*pModels)[ strName ];
    if( pModel )
        throw MASA_EXCEPTION( xis.context() + "Duplicate model name" );

    bool isMasalife = false;
    xis >> xml::attribute( "masalife", isMasalife );
    //Key is net.masagroup for masalife, none for others.
    std::string key = isMasalife ? "net.masagroup" : "none";

    const std::map< std::string, tools::Path >::const_iterator itFind = strSourcePaths.find( key );
    if( itFind == strSourcePaths.end() )
        throw MASA_EXCEPTION( xis.context() + "Model corresponds to an unknown namespace" );

    pModel = new DEC_Model( strName, xis, itFind->second, missionTypes, isMasalife, integrationDir );
}

//-----------------------------------------------------------------------------
// Name: DEC_Workspace::GetTime
// Created: JVT 02-06-28
//-----------------------------------------------------------------------------
float DEC_Workspace::GetTime() const
{
    return (float)MIL_AgentServer::GetWorkspace().GetSimTime();
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::InitializeObjectNames
// Created: LDC 2009-05-25
// -----------------------------------------------------------------------------
void DEC_Workspace::InitializeObjectNames( MIL_Config& config )
{
    config.GetPhyLoader().LoadPhysicalFile( "object-names", &DEC_ObjectFunctions::RegisterObjectNames );
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindModelPion
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
const DEC_Model_ABC* DEC_Workspace::FindModelPion( const std::string& strModelName ) const
{
    CIT_ModelMap it = pionModels_.find( strModelName );
    if( it == pionModels_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindModelAutomate
// Created: NLD 2004-09-03
// -----------------------------------------------------------------------------
const DEC_Model_ABC* DEC_Workspace::FindModelAutomate( const std::string& strModelName ) const
{
    CIT_ModelMap it = automateModels_.find( strModelName );
    if( it == automateModels_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: DEC_Workspace::FindModelPopulation
// Created: NLD 2005-09-28
// -----------------------------------------------------------------------------
const DEC_Model_ABC* DEC_Workspace::FindModelPopulation( const std::string& strModelName  ) const
{
    CIT_ModelMap it = populationModels_.find( strModelName );
    if( it == populationModels_.end() )
        return 0;
    return it->second;
}
