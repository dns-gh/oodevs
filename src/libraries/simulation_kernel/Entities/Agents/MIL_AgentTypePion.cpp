// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/MIL_AgentTypePion.cpp $
// $Author: Jvt $
// $Modtime: 23/05/05 16:34 $
// $Revision: 86 $
// $Workfile: MIL_AgentTypePion.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AgentTypePion.h"
#include "MIL_AgentPion.h"
#include "Units/PHY_UnitType.h"

#include "Decision/DEC_Workspace.h"
#include "Decision/DEC_Tools.h"

#include "Entities/Specialisations/ALAT/MIL_AgentTypePionALAT.h"
#include "Entities/Specialisations/RENS/MIL_AgentTypePionRENS.h"
#include "Entities/Specialisations/ASA/MIL_AgentTypePionASA.h"
#include "Entities/Specialisations/NBC/MIL_AgentTypePionNBC.h"
#include "Entities/Specialisations/CIRCULATION/MIL_AgentTypePionCIRCULATION.h"
#include "Entities/Specialisations/MILICE/MIL_AgentTypePionMILICE.h"
#include "Entities/Specialisations/ASY/MIL_AgentTypePionASY.h"
#include "Entities/Specialisations/REFUGIE/MIL_AgentTypePionREFUGIE.h"
#include "Entities/Specialisations/LOG/TC2/MIL_AgentTypePionLOGTC2.h"
#include "Entities/Specialisations/LOG/Medical/MIL_AgentTypePionLOGMedical.h"
#include "Entities/Specialisations/LOG/Maintenance/MIL_AgentTypePionLOGMaintenance.h"
#include "Entities/Specialisations/LOG/Supply/MIL_AgentTypePionLOGSupply.h"
#include "Entities/Specialisations/LOG/Convoy/MIL_AgentTypePionLOGConvoy.h"

#include "Entities/Agents/Roles/Composantes/PHY_RolePion_Composantes.h"
#include "Entities/Agents/Roles/Humans/PHY_RolePion_Humans.h"
#include "Entities/Agents/Roles/Dotations/PHY_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Network/NET_RolePion_Dotations.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"
#include "Entities/Agents/Roles/Perception/PHY_RolePion_Perceiver.h"    
#include "Entities/Agents/Roles/Posture/PHY_RolePion_Posture.h"
#include "Entities/Agents/Roles/Location/PHY_RolePion_Location.h"
#include "Entities/Agents/Roles/Reinforcement/PHY_RolePion_Reinforcement.h"
#include "Entities/Agents/Roles/NBC/PHY_RolePion_NBC.h"
#include "Entities/Agents/Roles/Communications/PHY_RolePion_Communications.h"
#include "Entities/Agents/Roles/HumanFactors/PHY_RolePion_HumanFactors.h"
#include "Entities/Agents/Roles/Transported/PHY_RolePion_Transported.h"
#include "Entities/Agents/Roles/Surrender/PHY_RolePion_Surrender.h"
#include "Entities/Agents/Roles/Refugee/PHY_RolePion_Refugee.h"
#include "Entities/Agents/Roles/Population/PHY_RolePion_Population.h"

#include "Entities/Agents/Actions/Loading/PHY_RoleAction_Loading.h"
#include "Entities/Agents/Actions/Objects/PHY_RoleAction_Objects.h"
#include "Entities/Agents/Actions/Moving/PHY_RoleAction_Moving.h"
#include "Entities/Agents/Actions/Flying/PHY_RoleAction_Flying.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_RoleAction_DirectFiring.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_RoleAction_IndirectFiring.h"
#include "Entities/Agents/Actions/Transport/PHY_RoleAction_Transport.h"
#include "Entities/Agents/Actions/Emergency/PHY_RoleAction_FolkInfluence.h"

#include "simulation_kernel/AlgorithmsFactories.h"

#include "Decision/DEC_Representations.h"

#include "Knowledge/DEC_Knowledge_RapFor_ABC.h"
 
#include "MIL_AgentServer.h"
#include "Tools/MIL_Tools.h"
#include "tools/xmlcodecs.h"
#include <xeumeuleu/xml.h>



MIL_AgentTypePion::T_PionTypeAllocatorMap  MIL_AgentTypePion::pionTypeAllocators_;
MIL_AgentTypePion::T_PionTypeMap           MIL_AgentTypePion::pionTypes_;

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Create
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePion::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AgentTypePion( strName, xis );
}

struct MIL_AgentTypePion::LoadingWrapper
{
    void ReadUnit( xml::xistream& xis )
    {
        MIL_AgentTypePion::ReadUnit( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing agent types" );

    pionTypeAllocators_[ "Pion INF"                    ] = &MIL_AgentTypePion               ::Create; 
    pionTypeAllocators_[ "Pion ASA"                    ] = &MIL_AgentTypePionASA            ::Create; 
    pionTypeAllocators_[ "Pion ALAT"                   ] = &MIL_AgentTypePionALAT           ::Create; 
    pionTypeAllocators_[ "Pion RENS"                   ] = &MIL_AgentTypePionRENS           ::Create; 
    pionTypeAllocators_[ "Pion NBC"                    ] = &MIL_AgentTypePionNBC            ::Create; 
    pionTypeAllocators_[ "Pion ABC"                    ] = &MIL_AgentTypePion               ::Create;
    pionTypeAllocators_[ "Pion GEN"                    ] = &MIL_AgentTypePion               ::Create;
    pionTypeAllocators_[ "Pion ASS"                    ] = &MIL_AgentTypePion               ::Create; 
    pionTypeAllocators_[ "Pion TRANS"                  ] = &MIL_AgentTypePion               ::Create; 
    pionTypeAllocators_[ "Pion LOG TC2"                ] = &MIL_AgentTypePionLOGTC2         ::Create;     
    pionTypeAllocators_[ "Pion LOG BLD Sante"          ] = &MIL_AgentTypePionLOGMedical     ::Create; 
    pionTypeAllocators_[ "Pion LOG BLD Maintenance"    ] = &MIL_AgentTypePionLOGMaintenance ::Create; 
    pionTypeAllocators_[ "Pion LOG BLD Ravitaillement" ] = &MIL_AgentTypePionLOGSupply      ::Create; 
    pionTypeAllocators_[ "Pion LOG BLT Sante"          ] = &MIL_AgentTypePionLOGMedical     ::Create; 
    pionTypeAllocators_[ "Pion LOG BLT Maintenance"    ] = &MIL_AgentTypePionLOGMaintenance ::Create; 
    pionTypeAllocators_[ "Pion LOG BLT Ravitaillement" ] = &MIL_AgentTypePionLOGSupply      ::Create; 
    pionTypeAllocators_[ "Pion LOG Convoi"             ] = &MIL_AgentTypePionLOGConvoy      ::Create; 
    pionTypeAllocators_[ "Pion CIRCULATION"            ] = &MIL_AgentTypePionCIRCULATION    ::Create; 
    pionTypeAllocators_[ "Pion JOINT"                  ] = &MIL_AgentTypePion               ::Create; 
    pionTypeAllocators_[ "Pion MILICE"                 ] = &MIL_AgentTypePionMILICE         ::Create; 
    pionTypeAllocators_[ "Pion ASY"                    ] = &MIL_AgentTypePionASY            ::Create;
    pionTypeAllocators_[ "Pion REFUGIE"                ] = &MIL_AgentTypePionREFUGIE        ::Create; 
    pionTypeAllocators_[ "Pion Emergency"              ] = &MIL_AgentTypePion               ::Create; 
    
    LoadingWrapper loader;
    
    xis >> xml::start( "units" )
            >> xml::list( "unit", loader, &LoadingWrapper::ReadUnit )
        >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::ReadUnit
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::ReadUnit( xml::xistream& xis )
{   
    std::set< uint > ids_;
    std::string strName;
    std::string strType;

    xis >> xml::attribute( "name", strName )
        >> xml::attribute( "type", strType );

    CIT_PionTypeAllocatorMap itPionAllocator = pionTypeAllocators_.find( strType );
    if( itPionAllocator == pionTypeAllocators_.end() )
        xis.error( "Unknown pion type" );

    const MIL_AgentTypePion*& pType = pionTypes_[ strName ];
    if( pType )
        xis.error( "Pion type already defined" );

    pType = (*itPionAllocator->second)( strName, xis );

    if( !ids_.insert( pType->GetID() ).second )
        xis.error( "Pion type ID already used" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::Terminate()
{
    for( CIT_PionTypeMap it = pionTypes_.begin(); it != pionTypes_.end(); ++it )
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
MIL_AgentTypePion::MIL_AgentTypePion( const std::string& strName, xml::xistream& xis )
    : MIL_AgentType_ABC               ( strName, xis )
    , pUnitType_                      ( new PHY_UnitType( xis ) )
    , rDistanceAvantLimas_            ( 0. )
    , rRapForIncreasePerTimeStepValue_( DEC_Knowledge_RapFor_ABC::GetRapForIncreasePerTimeStepDefaultValue() )
{
    InitializeRapFor              ( xis );
    InitializeDistancesAvantPoints( xis );
    InitializeModel               ( xis );
    InitializeDiaFunctions        ();    
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion constructor
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
MIL_AgentTypePion::MIL_AgentTypePion( const DEC_Model_ABC* pModel )
    : pModel_   ( pModel )
    , pUnitType_( 0 )
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
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InitializeDistancesAvantPoints
// Created: AGN 03-03-19
//-----------------------------------------------------------------------------
void MIL_AgentTypePion::InitializeDistancesAvantPoints( xml::xistream& xis )
{
    xis >> xml::optional()
            >> xml::start( "distance-before-points" )
                >> xml::list( "distance-before-point", *this, &MIL_AgentTypePion::ReadPoint )
            >> xml::end();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::ReadPoint
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::ReadPoint( xml::xistream& xis )
{
    std::string strTypePoint;
    xis >> xml::attribute( "type", strTypePoint );

    // Cas particulier : limas ( point n'appartenant pas à TER ! )
    if ( sCaseInsensitiveEqual()( strTypePoint, "lima" ) )
    {
        xis >> xml::attribute( "value", rDistanceAvantLimas_ );
        rDistanceAvantLimas_ = MIL_Tools::ConvertMeterToSim( rDistanceAvantLimas_ );
    }
    else
    {
        const TerrainData nType = MIL_Tools::ConvertLandType( strTypePoint );
        
        if( nType.Area() == 0xFF )
            xis.error( "Unknown land type" );

        if( distancesAvantPoints_.find( nType ) != distancesAvantPoints_.end() )
            xis.error( "Unknown 'distance avant point'" );

        MT_Float& rDistance = distancesAvantPoints_[ nType ];
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
    MT_Float rTimeTmp;
    tools::ReadTimeAttribute( xis, "feedback-time", rTimeTmp );
    rTimeTmp                         = MIL_Tools::ConvertSecondsToSim( rTimeTmp );
    rRapForIncreasePerTimeStepValue_ = DEC_Knowledge_RapFor_ABC::ComputeRapForIncreasePerTimeStepValue( rTimeTmp );
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
        xis.error( "Unknown pawn model" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InitializeDiaFunctions
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::InitializeDiaFunctions()
{
}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePion::InstanciatePion( uint nID, MIL_Automate& automate, xml::xistream& xis ) const
{
    return new MIL_AgentPion( *this, nID, automate, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion* MIL_AgentTypePion::InstanciatePion( uint nID, MIL_Automate& automate ) const
{
    return new MIL_AgentPion( *this, nID, automate );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::RegisterRoles
// Created: MGD 2009-08-13
// @TODO REPLACE BY XML in factory
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::RegisterRoles( MIL_AgentPion& pion, AlgorithmsFactories& algorithmsFactories ) const
{
    pion.RegisterRole< NET_RolePion_Dotations         >( pion );
    pion.RegisterRole< PHY_RolePion_Reinforcement     >( pion );
    pion.RegisterRole< PHY_RolePion_Posture           >( pion, *algorithmsFactories.postureComputerFactory_ );
    pion.RegisterRole< PHY_RolePion_Location          >( pion, *algorithmsFactories.locationComputerFactory_ );
    pion.RegisterRole< PHY_RolePion_Dotations         >( pion, *algorithmsFactories.consumptionComputerFactory_ );
    pion.RegisterRole< PHY_RolePion_Humans            >( pion );
    pion.RegisterRole< PHY_RolePion_Composantes       >( pion );
    pion.RegisterRole< PHY_RolePion_Perceiver         >( pion );
    pion.RegisterRole< PHY_RolePion_NBC               >( pion );
    pion.RegisterRole< PHY_RolePion_Communications    >( pion );
    pion.RegisterRole< PHY_RolePion_HumanFactors      >();
    pion.RegisterRole< PHY_RolePion_Transported       >( pion );
    pion.RegisterRole< PHY_RolePion_Surrender         >( pion );
    pion.RegisterRole< PHY_RolePion_Refugee           >( pion );
    pion.RegisterRole< PHY_RolePion_Population        >( pion );
    pion.RegisterRole< PHY_RoleAction_Loading         >( pion);
    pion.RegisterRole< PHY_RoleAction_Transport       >( pion );
    pion.RegisterRole< PHY_RoleAction_Moving          >( pion );
    pion.RegisterRole< PHY_RoleAction_Objects         >( pion );
    pion.RegisterRole< firing::PHY_RoleAction_DirectFiring    >( pion, *algorithmsFactories.weaponAvailabilityComputerFactory_, *algorithmsFactories.composantesAbleToBeFiredComputerFactory_ );
    pion.RegisterRole< firing::PHY_RoleAction_IndirectFiring  >( pion, *algorithmsFactories.weaponAvailabilityComputerFactory_ );
    pion.RegisterRole< DEC_RolePion_Decision          >( pion );
    pion.RegisterRole< PHY_RoleAction_FolkInfluence   >();
    pion.RegisterRole< DEC_Representations            >();

    if( pion.CanFly() )
        pion.RegisterRole< PHY_RoleAction_Flying >( pion, *algorithmsFactories.moveComputerFactory_ );
    else
        pion.RegisterRole< PHY_RoleAction_InterfaceFlying >();
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::RegisterFunctions
// Created: LDC 2009-04-23
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::RegisterFunctions( directia::Brain& /*brain*/, MIL_AgentPion& /*agent*/ ) const
{

}
