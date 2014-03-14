// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypePion.h"

#include "MIL_AgentPion.h"
#include "MIL_AgentServer.h"
#include "RoleExtender_ABC.h"

#include "Tools/MIL_Tools.h"
#include "Tools/MIL_HumanRepartition.h"
#include "Units/PHY_UnitType.h"

#include "Decision/Brain.h"
#include "Decision/DEC_AgentFunctions.h"
#include "Decision/DEC_KnowledgeObjectFunctions.h"
#include "Decision/DEC_Representations.h"
#include "Decision/DEC_Tools.h"
#include "Decision/DEC_Workspace.h"

#include "Entities/Specialisations/LOG/TC2/MIL_AgentTypePionLOGTC2.h"
#include "Entities/Specialisations/LOG/Medical/MIL_AgentTypePionLOGMedical.h"
#include "Entities/Specialisations/LOG/Maintenance/MIL_AgentTypePionLOGMaintenance.h"
#include "Entities/Specialisations/LOG/Supply/MIL_AgentTypePionLOGSupply.h"
#include "Entities/Specialisations/LOG/Convoy/MIL_AgentTypePionLOGConvoy.h"
#include "Entities/Specialisations/Remote/MIL_AgentTypePion_Remote.h"

#include "Entities/Agents/Roles/Humans/PHY_RolePion_Humans.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Network/NET_RolePion_Dotations.h"

#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/NBC/PHY_RolePion_NBC.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.h"
#include "Entities/Agents/Roles/Population/PHY_RolePion_Population.h"
#include "Entities/Agents/Roles/Protection/PHY_RolePion_ActiveProtection.h"
#include "Entities/Agents/Roles/Illumination/PHY_RolePion_Illumination.h" // LTO
#include "Entities/Agents/Roles/Terrain/PHY_RolePion_TerrainAnalysis.h"
#include "Entities/Agents/Roles/Urban/PHY_RolePion_UrbanLocation.h"
#include "Entities/Agents/Roles/Deployment/PHY_RolePion_Deployment.h"

#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_Flying.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/Actions/CrowdTransport/PHY_RoleAction_CrowdTransport.h"
#include "Entities/Agents/Actions/Underground/PHY_RoleAction_MovingUnderground.h"
#include "Entities/Agents/Actions/Emergency/PHY_RoleAction_FolkInfluence.h"

#include "MT_Tools/MT_Logger.h"
#include "Tools/MIL_Tools.h"
#include "tools/Codec.h"

#include <boost/algorithm/string.hpp>
#include <boost/assign/list_of.hpp>
#include <functional>

namespace
{

typedef std::function< const MIL_AgentTypePion*(
        const std::string& strName, const std::string& strType,
            xml::xistream& xis ) >
        T_PionTypeAllocator;

std::map< std::string, T_PionTypeAllocator > pionTypeAllocators_;

} // namespace

MIL_AgentTypePion::T_PionTypeMap           MIL_AgentTypePion::pionTypes_;

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Create
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePion::Create( const std::string& strName, const std::string& strType, xml::xistream& xis )
{
    return new MIL_AgentTypePion( strName, strType, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing agent types" );
    const std::vector< std::string > basicTypes = boost::assign::list_of
        ( "Pion ABC" )
        ( "Pion ALAT" )
        ( "Pion ASA" )
        ( "Pion ASS" )
        ( "Pion Civilian" )
        ( "Pion Emergency" )
        ( "Pion GEN" )
        ( "Pion INF" )
        ( "Pion JOINT" )
        ( "Pion Journalist" )
        ( "Pion Notable" )
        ( "Pion RENS" )
        ( "Pion TRANS" )
        ;
    for( auto it = basicTypes.begin(); it != basicTypes.end(); ++it )
        pionTypeAllocators_[ *it ] = &MIL_AgentTypePion::Create;

    pionTypeAllocators_[ "Pion NBC" ] =
        []( const std::string& name, const std::string& type, xml::xistream& xis )
            -> const MIL_AgentTypePion*
        {
            auto t = new MIL_AgentTypePion( name, type, xis );
            t->SetBrainFunctions( boost::assign::list_of
                    ( "DEC_DecontaminerZone" )
                    ( "DEC_Agent_SeDecontaminer" ));
            t->SetNBC( true );
            return t;
        };
    pionTypeAllocators_[ "Pion ASY" ] =
        []( const std::string& name, const std::string& type, xml::xistream& xis )
            -> const MIL_AgentTypePion*
        {
            auto t = new MIL_AgentTypePion( name, type, xis );
            t->SetTerrorist( true );
            return t;
        };
    pionTypeAllocators_[ "Pion CIRCULATION" ] = 
        []( const std::string& name, const std::string& type, xml::xistream& xis )
            -> const MIL_AgentTypePion*
        {
            auto t = new MIL_AgentTypePion( name, type, xis );
            t->SetBrainFunctions( boost::assign::list_of
                    ( "DEC_Circulation_EquiperItineraireLogistique" ));
            return t;
        };
    pionTypeAllocators_[ "Pion MILICE" ] =
        []( const std::string& name, const std::string& type, xml::xistream& xis )
            -> const MIL_AgentTypePion*
        {
            auto t = new MIL_AgentTypePion( name, type, xis );
            t->SetMilitia( true );
            return t;
        };
    pionTypeAllocators_[ "Pion REFUGIE" ] =
        []( const std::string& name, const std::string& type, xml::xistream& xis )
            -> const MIL_AgentTypePion*
        {
            auto t = new MIL_AgentTypePion( name, type, xis );
            t->SetRefugee( true );
            return t;
        };
        
    pionTypeAllocators_[ "Pion LOG TC2"                ] = &MIL_AgentTypePionLOGTC2         ::Create;
    pionTypeAllocators_[ "Pion LOG BLD Sante"          ] = &MIL_AgentTypePionLOGMedical     ::Create;
    pionTypeAllocators_[ "Pion LOG BLD Maintenance"    ] = &MIL_AgentTypePionLOGMaintenance ::Create;
    pionTypeAllocators_[ "Pion LOG BLD Ravitaillement" ] = &MIL_AgentTypePionLOGSupply      ::Create;
    pionTypeAllocators_[ "Pion LOG BLT Sante"          ] = &MIL_AgentTypePionLOGMedical     ::Create;
    pionTypeAllocators_[ "Pion LOG BLT Maintenance"    ] = &MIL_AgentTypePionLOGMaintenance ::Create;
    pionTypeAllocators_[ "Pion LOG BLT Ravitaillement" ] = &MIL_AgentTypePionLOGSupply      ::Create;
    pionTypeAllocators_[ "Pion LOG Convoi"             ] = &MIL_AgentTypePionLOGConvoy      ::Create;
    pionTypeAllocators_[ "Pion Organization"           ] = &MIL_AgentTypePionLOGTC2         ::Create;
    pionTypeAllocators_[ "Pion Remote"                 ] = &MIL_AgentTypePion_Remote        ::Create;

    xis >> xml::start( "units" )
            >> xml::list( "unit", &MIL_AgentTypePion::ReadUnit )
        >> xml::end;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::ReadUnit
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::ReadUnit( xml::xistream& xis )
{
    std::set< unsigned int > ids_;
    std::string strName;
    std::string strType;
    xis >> xml::attribute( "name", strName )
        >> xml::attribute( "type", strType );
    const auto itPionAllocator = pionTypeAllocators_.find( strType );
    if( itPionAllocator == pionTypeAllocators_.end() )
        throw MASA_EXCEPTION( xis.context() + "Unknown pion type" );
    const MIL_AgentTypePion*& pType = pionTypes_[ strName ];
    if( pType )
        throw MASA_EXCEPTION( xis.context() + "Pion type already defined" );
    try
    {
        pType = itPionAllocator->second( strName, strType, xis );
    }
    catch( const std::exception& e )
    {
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( e ) );
        pType = 0;
    }

    if( pType )
    {
        if( !ids_.insert( pType->GetID() ).second )
            throw MASA_EXCEPTION( xis.context() + "Pion type ID already used" );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::Terminate()
{
    for( auto it = pionTypes_.begin(); it != pionTypes_.end(); ++it )
        delete it->second;
    pionTypes_.clear();
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_AgentTypePion::MIL_AgentTypePion( const std::string& strName, const std::string& strType, xml::xistream& xis )
    : MIL_AgentType_ABC               ( strName, strType, xis )
    , pUnitType_                      ( new PHY_UnitType( xis ) )
    , rDistanceAvantLimas_            ( 0. )
    , rFeedbackTime_                  ( std::numeric_limits< double >::infinity() )
    , pHumanRepartition_              ( new MIL_HumanRepartition( xis ) )
    , isNBC_                          ( false )
{
    InitializeRapFor              ( xis );
    InitializeDistancesAvantPoints( xis );
    InitializeModel               ( xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion constructor
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
MIL_AgentTypePion::MIL_AgentTypePion( const DEC_Model_ABC* pModel )
    : pModel_   ( pModel )
    , pUnitType_( 0 )
    , rDistanceAvantLimas_( 0. )
    , rFeedbackTime_( 0. )
    , pHumanRepartition_( new MIL_HumanRepartition() )
    , isNBC_( false )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_AgentTypePion::~MIL_AgentTypePion()
{
    delete pUnitType_;
    delete pHumanRepartition_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InitializeDistancesAvantPoints
// Created: AGN 03-03-19
//-----------------------------------------------------------------------------
void MIL_AgentTypePion::InitializeDistancesAvantPoints( xml::xistream& xis )
{
    xis >> xml::optional
        >> xml::start( "distance-before-points" )
            >> xml::list( "distance-before-point", *this, &MIL_AgentTypePion::ReadPoint )
        >> xml::end;
}

namespace
{
    const TerrainData KeypointToTerrainData( const E_KeyPoint keypoint )
    {
        switch( keypoint )
        {
            case eKeyPointForest:
                return TerrainData::Forest();
            case eKeyPointBridge:
                return TerrainData::Bridge();
            case eKeyPointCrossroads:
                return TerrainData::Crossroad();
            case eKeyPointUrban:
                return TerrainData::Urban();
            default:
                throw MASA_EXCEPTION( "Unknown keypoint value." );
        }
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::ReadPoint
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::ReadPoint( xml::xistream& xis )
{
    std::string strTypePoint;
    xis >> xml::attribute( "type", strTypePoint );
    if( boost::iequals( strTypePoint, "lima" ) )
    {
        xis >> xml::attribute( "value", rDistanceAvantLimas_ );
        rDistanceAvantLimas_ = MIL_Tools::ConvertMeterToSim( rDistanceAvantLimas_ );
    }
    else
    {
        const TerrainData nType = KeypointToTerrainData( MIL_Tools::ConvertLandType( strTypePoint ) );
        if( distancesAvantPoints_.find( nType ) != distancesAvantPoints_.end() )
            throw MASA_EXCEPTION( xis.context() + "Unknown 'distance avant point'" );
        double& rDistance = distancesAvantPoints_[ nType ];
        xis >> xml::attribute( "value", rDistance );
        rDistance = MIL_Tools::ConvertMeterToSim( rDistance );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InitializeRapFor
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::InitializeRapFor( xml::xistream& xis )
{
    xis >> xml::list( "force-ratio", *this, &MIL_AgentTypePion::ReadFeedback );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::ReadFeedback
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::ReadFeedback( xml::xistream& xis )
{
    double rTimeTmp;
    tools::ReadTimeAttribute( xis, "feedback-time", rTimeTmp );
    rFeedbackTime_ = MIL_Tools::ConvertSecondsToSim( rTimeTmp );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InitializeModel
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::InitializeModel( xml::xistream& xis )
{
    std::string strModel;
    xis >> xml::attribute( "decisional-model", strModel );
    pModel_ = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelPion( strModel );
    if( !pModel_ )
        throw MASA_EXCEPTION( xis.context() + "Unknown pawn model " + strModel );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePion::InstanciatePion( MissionController_ABC& controller,
                                                   MIL_Automate& automate,
                                                   xml::xistream& xis ) const
{
    return new MIL_AgentPion( *this, controller, automate, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::RegisterRoles
// Created: MGD 2009-08-13
// @TODO REPLACE BY XML in factory
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::RegisterRoles( MIL_AgentPion& pion, sword::RoleExtender_ABC* ext ) const
{
    const bool bIsAutonomous = pion.IsAutonomous();
    pion.RegisterRole( *new network::NET_RolePion_Dotations( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Reinforcement( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Posture( pion ) );
    pion.RegisterRole( *new PHY_RolePion_UrbanLocation( pion ) );
    pion.RegisterRole( *new dotation::PHY_RolePion_Dotations( pion ) );
    pion.RegisterRole( *new human::PHY_RolePion_Humans( pion ) );
    pion.RegisterRole( *new PHY_RolePion_ActiveProtection( pion ) );
    pion.RegisterRole( *new nbc::PHY_RolePion_NBC( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Communications( pion, bIsAutonomous ) );
    pion.RegisterRole( *new PHY_RolePion_HumanFactors( pion ) );
    pion.RegisterRole( *new transport::PHY_RolePion_Transported( pion ) );
    pion.RegisterRole( *new surrender::PHY_RolePion_Surrender( pion ) );
    pion.RegisterRole( *new refugee::PHY_RolePion_Refugee( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Population( pion ) );
    pion.RegisterRole( *new transport::PHY_RoleAction_Transport( pion ) );
    pion.RegisterRole( *new crowdtransport::PHY_RoleAction_CrowdTransport( pion ) );
    pion.RegisterRole( *new PHY_RoleAction_Objects( pion ) );
    pion.RegisterRole( *new firing::PHY_RoleAction_DirectFiring( pion ) );
    pion.RegisterRole( *new firing::PHY_RoleAction_IndirectFiring( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Illumination() ); // LTO
    pion.RegisterRole( *new PHY_RoleAction_FolkInfluence() );
    pion.RegisterRole( *new DEC_Representations() );
    pion.RegisterRole( *new PHY_RolePion_TerrainAnalysis( pion ) );
    pion.RegisterRole( *new PHY_RoleAction_MovingUnderground( pion ) );
    pion.RegisterRole( *new PHY_RolePion_Deployment( pion ) );
    if( pion.GetType().GetUnitType().CanFly() )
        pion.RegisterRole( *new PHY_RoleAction_Flying( pion ) );
    else
        pion.RegisterRole( *new PHY_RoleAction_InterfaceFlying() );
    if( ext )
        ext->RegisterRoles( pion );
}

namespace
{

void RegisterFunction( const std::string& name,  sword::Brain& brain,
        MIL_Agent_ABC& agent )
{
    if( name == "DEC_ContaminerZone" )
        brain.RegisterFunction( name.c_str(),
            boost::function< void( const TER_Localisation* ) >(
                boost::bind( &DEC_KnowledgeObjectFunctions::DecontaminateZone,
                    boost::cref( agent ), _1 ) ) );
    else if( name == "DEC_Agent_SeDecontaminer" )
        brain.RegisterFunction( name.c_str(),
            boost::bind( &DEC_AgentFunctions::SelfDecontaminate, boost::ref( agent ) ) );
    else if( name == "DEC_Circulation_EquiperItineraireLogistique" ) 
        brain.RegisterFunction( name.c_str(),
            &DEC_KnowledgeObjectFunctions::EquipLogisticRoute );
    else
        throw MASA_EXCEPTION(
                "cannot register unknown unit decisional method: " + name );
}

} // namespace

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::RegisterFunctions( sword::Brain& brain, MIL_Agent_ABC& agent ) const
{
    for( auto it = functions_.begin(); it != functions_.end(); ++it )
        RegisterFunction( *it, brain, agent );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Find
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePion::Find( const std::string& strName )
{
    CIT_PionTypeMap it = pionTypes_.find( strName );
    return it == pionTypes_.end() ? 0 : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Find
// Created: JVT 2005-04-14
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePion::Find( unsigned int nID )
{
    for( auto it = pionTypes_.begin(); it != pionTypes_.end(); ++it )
        if( it->second->GetID() == nID )
            return it->second;
    return 0;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetUnitType
// Created: NLD 2004-08-12
// -----------------------------------------------------------------------------
const PHY_UnitType& MIL_AgentTypePion::GetUnitType() const
{
    assert( pUnitType_ );
    return *pUnitType_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetModel
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
const DEC_Model_ABC& MIL_AgentTypePion::GetModel() const
{
    assert( pModel_ );
    return *pModel_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetDistanceAvantPoint
// Created: NLD 2004-09-06
// -----------------------------------------------------------------------------
double MIL_AgentTypePion::GetDistanceAvantPoint( const TerrainData& nType ) const
{
    CIT_DistanceAvantPointMap it = distancesAvantPoints_.find( nType );
    return it == distancesAvantPoints_.end() ? 0. : it->second;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetDistanceAvantLima
// Created: JVT 2005-05-10
// -----------------------------------------------------------------------------
double MIL_AgentTypePion::GetDistanceAvantLima() const
{
    return rDistanceAvantLimas_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetFeedbackTime
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
double MIL_AgentTypePion::GetFeedbackTime() const
{
    return rFeedbackTime_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::GetHumanRepartition
// Created: MMC 2011-10-10
// -----------------------------------------------------------------------------
const MIL_HumanRepartition& MIL_AgentTypePion::GetHumanRepartition() const
{
    return *pHumanRepartition_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::operator==
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
bool MIL_AgentTypePion::operator==( const MIL_AgentTypePion& rhs ) const
{
    return this == &rhs; //$$ A changer quand IDs
}

void MIL_AgentTypePion::SetBrainFunctions( const std::vector< std::string >& names )
{
    functions_ = names;
}

void MIL_AgentTypePion::SetNBC( bool isNBC )
{
    isNBC_ = isNBC;
}

bool MIL_AgentTypePion::IsNBC() const
{
    return isNBC_;
}
