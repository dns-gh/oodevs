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

#include "MIL_pch.h"

#include "MIL_AgentTypePion.h"
#include "MIL_AgentPion.h"
#include "Units/PHY_UnitType.h"

#include "Decision/Functions/DEC_AgentFunctions.h"
#include "Decision/Functions/DEC_KnowledgeAgentFunctions.h"
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"
#include "Decision/Functions/DEC_KnowledgePopulationFunctions.h"
#include "Decision/Functions/DEC_GeometryFunctions.h"
#include "Decision/Functions/DEC_FireFunctions.h"
#include "Decision/Functions/DEC_KnowledgeFunctions.h"
#include "Decision/Functions/DEC_PathFunctions.h"
#include "Decision/Functions/DEC_PerceptionFunctions.h"
#include "Decision/Functions/DEC_MiscFunctions.h"
#include "Decision/Functions/DEC_OrdersFunctions.h"
#include "Decision/Functions/DEC_ActionFunctions.h"
#include "Decision/Functions/DEC_PathFunctions.h"
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/Functions/DEC_ObjectFunctions.h"
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
#include "Entities/Agents/Actions/ComposanteLending/PHY_ActionLendCollectionComposantes.h"
#include "Entities/Agents/Actions/ComposanteLending/PHY_ActionLendHaulerComposantes.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionConstructObject.h" 
#include "Entities/Agents/Actions/Objects/PHY_ActionPrepareObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionDestroyObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionMineObject.h"   
#include "Entities/Agents/Actions/Objects/PHY_ActionDemineObject.h" 
#include "Entities/Agents/Actions/Objects/PHY_ActionBypassObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionOccupyObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionAnimateObject.h"
#include "Entities/Agents/Actions/Objects/PHY_ActionResumeWorkObject.h"
#include "Entities/Agents/Actions/Moving/PHY_ActionMove.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Position.h"
#include "Entities/Agents/Actions/Firing/IndirectFiring/PHY_ActionIndirectFire_Knowledge.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePopulation.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePion.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyLoadable.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionUsingOnlyCarrier.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionDirectFirePionOnMajorComposantes.h"
#include "Entities/Agents/Actions/Firing/DirectFiring/PHY_ActionControlZone.h"
#include "Entities/Agents/Actions/Transport/PHY_ActionTransportLoad.h"
#include "Entities/Agents/Actions/Transport/PHY_ActionTransportUnload.h"
#include "Entities/Agents/Actions/Loading/PHY_ActionLoad.h"
#include "Entities/Agents/Actions/Loading/PHY_ActionUnload.h"
#include "Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h"

#include "Knowledge/DEC_Knowledge_RapFor_ABC.h"
 
#include "MIL_AgentServer.h"
#include "Tools/MIL_Tools.h"

MIL_AgentTypePion::T_PionTypeAllocatorMap  MIL_AgentTypePion::pionTypeAllocators_;
MIL_AgentTypePion::T_PionTypeMap           MIL_AgentTypePion::pionTypes_;

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Create
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const MIL_AgentTypePion* MIL_AgentTypePion::Create( const std::string& strName, MIL_InputArchive& archive )
{
    return new MIL_AgentTypePion( strName, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::Initialize( MIL_InputArchive& archive )
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

    std::set< uint > ids_;
    archive.BeginList( "Pions" );
    while( archive.NextListElement() )
    {
        archive.Section( "Unite" );
    
        std::string strName;
        std::string strType;

        archive.ReadAttribute( "nom", strName );
        archive.ReadAttribute( "type", strType );

        CIT_PionTypeAllocatorMap itPionAllocator = pionTypeAllocators_.find( strType );
        if( itPionAllocator == pionTypeAllocators_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown pion type", archive.GetContext() );

        const MIL_AgentTypePion*& pType = pionTypes_[ strName ];
        if( pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Pion type already defined", archive.GetContext() );

        pType = (*itPionAllocator->second)( strName, archive );

        if( !ids_.insert( pType->GetID() ).second )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Pion type ID already used", archive.GetContext() );
    
        archive.EndSection(); // Unite
    }
    archive.EndList(); // Pions
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
MIL_AgentTypePion::MIL_AgentTypePion( const std::string& strName, MIL_InputArchive& archive )
    : MIL_AgentType_ABC               ( strName, archive )
    , pDIAFunctionTable_              ( new DIA_FunctionTable< MIL_AgentPion >() )
    , rDistanceAvantLimas_            ( 0. )
    , rRapForIncreasePerTimeStepValue_( DEC_Knowledge_RapFor_ABC::GetRapForIncreasePerTimeStepDefaultValue() )
{
    pUnitType_ = new PHY_UnitType( archive );

    InitializeRapFor              ( archive );
    InitializeDistancesAvantPoints( archive );
    InitializeModel               ( archive );
    InitializeDiaFunctions        ();    
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_AgentTypePion::~MIL_AgentTypePion()
{
    delete pUnitType_;
    delete pDIAFunctionTable_;
}

//-----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InitializeDistancesAvantPoints
// Created: AGN 03-03-19
//-----------------------------------------------------------------------------
void MIL_AgentTypePion::InitializeDistancesAvantPoints( MIL_InputArchive& archive )
{
    if( !archive.BeginList( "DistancesAvantPoints", MIL_InputArchive::eNothing ) )
        return;

    while( archive.NextListElement() )
    {
        archive.Section( "Point" );

        std::string strTypePoint;
        archive.ReadAttribute( "type", strTypePoint );

        // Cas particulier : limas ( point n'appartenant pas à TER ! )
        if ( sCaseInsensitiveEqual()( strTypePoint, "lima" ) )
        {
            archive.Read( rDistanceAvantLimas_ );
            rDistanceAvantLimas_ = MIL_Tools::ConvertMeterToSim( rDistanceAvantLimas_ );
        }
        else
        {
            const TerrainData nType = MIL_Tools::ConvertLandType( strTypePoint );
            
            if( nType.Area() == 0xFF )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown land type", archive.GetContext() );

            if( distancesAvantPoints_.find( nType ) != distancesAvantPoints_.end() )
                throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown 'distance avant point'", archive.GetContext() );
        
            MT_Float& rDistance = distancesAvantPoints_[ nType ];
            archive.Read( rDistance );
            rDistance = MIL_Tools::ConvertMeterToSim( rDistance );
        }
        
        archive.EndSection(); // Point
    }

    archive.EndList(); // DistancesAvantPoints
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InitializeRapFor
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::InitializeRapFor( MIL_InputArchive& archive )
{
    if( !archive.Section( "RapportDeForce", MIL_InputArchive::eNothing ) )
        return;

    MT_Float rTimeTmp;
    if( archive.ReadTimeField( "TempsDeRemontee", rTimeTmp, MIL_InputArchive::eNothing ) )
    {
        rTimeTmp                         = MIL_Tools::ConvertSecondsToSim( rTimeTmp );
        rRapForIncreasePerTimeStepValue_ = DEC_Knowledge_RapFor_ABC::ComputeRapForIncreasePerTimeStepValue( rTimeTmp );
    }
    archive.EndSection(); // RapportDeForce
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InitializeModel
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::InitializeModel( MIL_InputArchive& archive )
{
    std::string strModel;
    archive.ReadField( "ModeleDecisionnel", strModel );

    pModel_ = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelPion( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown pawn model", archive.GetContext() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InitializeDiaFunctions
// Created: NLD 2004-08-19
// -----------------------------------------------------------------------------
void MIL_AgentTypePion::InitializeDiaFunctions()
{
    // Actions
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StopAction   < MIL_AgentPion                              >, "DEC_StopAction"                           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::SuspendAction< MIL_AgentPion                              >, "DEC_PauseAction"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::ResumeAction < MIL_AgentPion                              >, "DEC_ReprendAction"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionMove                             >, "DEC_StartDeplacement"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionDirectFirePopulation             >, "DEC_StartTirSurPopulation"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionDirectFirePion                   >, "DEC_StartTirDirect"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionDirectFirePionUsingOnlyLoadable  >, "DEC_StartTirDirectDebarques"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionDirectFirePionUsingOnlyCarrier   >, "DEC_StartTirDirectTransporteurs"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionDirectFirePionOnMajorComposantes >, "DEC_StartTirDirectSurComposantesMajeures" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Position            >, "DEC_StartTirIndirectSurPosition"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionIndirectFire_Knowledge           >, "DEC_StartTirIndirectSurConnaissance"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionConstructObject                  >, "DEC_StartCreerObjet"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionPrepareObject                    >, "DEC_StartPreparerObjet"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionDestroyObject                    >, "DEC_StartDetruireObjet"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionMineObject                       >, "DEC_StartValoriserObjet"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionDemineObject                     >, "DEC_StartDevaloriserObjet"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionBypassObject                     >, "DEC_StartCreerContournementObjet"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionOccupyObject                     >, "DEC_StartOccuperObjet"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionResumeWorkObject                 >, "DEC_StartReprendreTravauxObjet"           );  
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionAnimateObject                    >, "DEC_StartAnimerObjet"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionControlZone                      >, "DEC_StartControlerZone"                   );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionLoad                             >, "DEC_StartEmbarquement"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction  < PHY_ActionUnload                           >, "DEC_StartDebarquement"                    );

    // Embarquement / débarquement
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsLoaded                   , "DEC_Agent_EstEmbarque"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetLoadingTime             , "DEC_Agent_DureeEmbarquement"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetUnloadingTime           , "DEC_Agent_DureeDebarquement"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::AreHumanTransportersReady  , "DEC_Agent_TransporteursPret"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::DisableHumanTransportersNow, "DEC_LaisserTransporteursSansDelai"   );   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::RecoverHumanTransportersNow, "DEC_RecupererTransporteursSansDelai" );

    // Objects
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ObjectFunctions::ActivateObject    < MIL_AgentPion >, "DEC_ActiverObjet"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ObjectFunctions::MagicCreateObject < MIL_AgentPion >, "DEC_CreerObjetSansDelais"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ObjectFunctions::MagicDestroyObject< MIL_AgentPion >, "DEC_DetruireObjetSansDelais" );

    // Path
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PathFunctions::CreatePathToPoint    , "DEC_CreerItineraire"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PathFunctions::CreatePathToPointList, "DEC_CreerItineraireListe"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PathFunctions::DeletePath           , "DEC_DetruireItineraire"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PathFunctions::GetPathState         , "DEC_Itineraire_Etat"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PathFunctions::GetNextObjectOnPath  , "DEC_Itineraire_ProchainObjet"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PathFunctions::ExtrapolatePosition  , "DEC_Itineraire_ExtrapolerPosition" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PathFunctions::GetLastPointOfPath   , "DEC_Itineraire_DernierPoint"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PathFunctions::IsMovingOnPath       , "DEC_Itineraire_EstEnMouvementSur"  );
  
    // Perception
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::GetMaxDetectionDistance             , "DEC_Perception_DistanceMax"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableCoupDeSonde                   , "DEC_Perception_ActiverCoupsDeSonde"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableCoupDeSonde                  , "DEC_Perception_DesactiverCoupsDeSonde"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::IsPointVisible                      , "DEC_Perception_PointEstVisible"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::SetVisionModeDirection              , "DEC_Perception_VisionVerrouilleeSurDirection"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::SetVisionModePoint                  , "DEC_Perception_VisionVerrouilleeSurPoint"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::SetVisionModeNormal                 , "DEC_Perception_VisionNormale"                        );   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableRecognitionLocalisation       , "DEC_Perception_ActiverReconnaissanceLocalisation"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableRecognitionLocalisation      , "DEC_Perception_DesactiverReconnaissanceLocalisation" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableRecognitionPoint              , "DEC_Perception_ActiverReconnaissancePoint"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableRecognitionPoint             , "DEC_Perception_DesactiverReconnaissancePoint"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableRadar                         , "DEC_Perception_ActiverRadar"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableRadar                        , "DEC_Perception_DesactiverRadar"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableRadarOnLocalisation           , "DEC_Perception_ActiverRadarSurLocalisation"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableRadarOnLocalisation          , "DEC_Perception_DesactiverRadarSurLocalisation"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableObjectRecognitionLocalisation , "DEC_Perception_ActiverDetectionObjetLocalisation"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableObjectRecognitionLocalisation, "DEC_Perception_DesactiverDetectionObjetLocalisation" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableFlyingShellDetection          , "DEC_Perception_ActiverPerceptionTirsIndirect"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableFlyingShellDetection         , "DEC_Perception_DesactiverPerceptionTirsIndirect"     );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::SetStealthFactor                    , "DEC_Perception_Furtivite"                            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::DisableSensors                      , "DEC_Perception_DesactiverSenseurs"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PerceptionFunctions::EnableSensors                       , "DEC_Perception_ActiverSenseurs"                      );

    // Gestion des renforts
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::GetReinforcements  , "DEC_Renforts" );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::Reinforce          , "DEC_Renforce" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::CancelReinforcement, "DEC_AnnuleRenforcement" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::IsReinforcing      , "DEC_EstEnRenfort" );

    // Tests / Debug
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::DebugDrawPoint , "DEC_DebugAffichePoint"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::DebugDrawPoints, "DEC_DebugAffichePoints" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::Debug          , "DEC_Debug"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::Trace          , "DEC_Trace"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::SetPosture     , "DEC_PostureForTest"     ); //$$ Used for test only

    // Pion accessors    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsPC                    , "DEC_Agent_EstPC"                            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsTransported           , "DEC_Agent_EstTransporte"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsFlying                , "DEC_Agent_EstEnVol"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::SetFlyingHeight         , "DEC_Agent_HauteurDeVol"                     );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsMoving                , "DEC_Agent_EstEnMouvement"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetOperationalState     , "DEC_Agent_EtatOps"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetMajorOperationalState, "DEC_Agent_EtatOpsMajeur"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::CanConstructObject      , "DEC_Agent_PeutConstruireObjet"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::CanBypassObject         , "DEC_Agent_PeutConstruireContournementObjet" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::CanDestroyObject        , "DEC_Agent_PeutDetruireObjet"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::CanMineObject           , "DEC_Agent_PeutValoriserObjet"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::CanActivateObject       , "DEC_Agent_PeutActiverObjet"                 );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::EnableDiscreteMode      , "DEC_Agent_ActiverModeDiscret"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::DisableDiscreteMode     , "DEC_Agent_DesactiverModeDiscret"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsNeutralized           , "DEC_Agent_EstNeutralise"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsAutomateEmbraye       , "DEC_Agent_AutomateEstEmbraye"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetPosition             , "DEC_Agent_Position"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetDirection            , "DEC_Agent_Direction"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsSurrendered           , "DEC_Agent_SEstRendu"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsPrisoner              , "DEC_Agent_EstPrisonnier"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsRefugee               , "DEC_Agent_EstRefugie"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsMilitia               , "DEC_Agent_EstMilice"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsTerrorist             , "DEC_Agent_EstTerroriste"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsDead                  , "DEC_Agent_EstMort"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetPosture              , "DEC_Agent_NiveauInstallation"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetRoePopulation        , "DEC_Agent_RoePopulation"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetKilledOfficers       , "DEC_Agent_OfficiersTues"                    );
 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::TimeLeftForMoving  , "DEC_Agent_AutonomieEnDeplacement"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::TimeToMoveDistance , "DEC_Agent_TempsPourParcourirDistanceEnLigneDroite" );

    // NBC
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsContaminated                   , "DEC_Agent_EstContamine"                         );   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::IsWearingNbcProtectionSuit       , "DEC_Agent_EstEnTenueProtectionNBC"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::WearNbcProtectionSuit            , "DEC_Agent_MettreTenueProtectionNBC"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::RemoveNbcProtectionSuit          , "DEC_Agent_EnleverTenueProtectionNBC"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::QueueForDecontamination, "DEC_ConnaissanceObjet_DemandeDeDecontamination" );

    // Blackout
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::ActivateBlackout  , "DEC_Agent_PasserEnSilenceRadio" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::DeactivateBlackout, "DEC_Agent_ArreterSilenceRadio"  );

    // Misc
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions ::SetCurrentSpeedModificator, "DEC_ModulationVitesseCourante"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions ::SetMaxSpeedModificator    , "DEC_ModulationVitesseMax"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::SetElongationFactor       , "DEC_Elongation"                 );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::RelievePion               , "DEC_ReleverPion"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::CanRelievePion            , "DEC_PeutReleverPion"            );

    // Population
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::EnableSlowDownByPopulation          , "DEC_Agent_ActiverRalentissementParPopulation"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::DisableSlowDownByPopulation         , "DEC_Agent_DesactiverRalentissementParPopulation"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::EnableReloadingDurationModification , "DEC_Agent_ActiverModificationTempsRechargementParPopulation"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::DisableReloadingDurationModification, "DEC_Agent_DesactiverModificationTempsRechargementParPopulation" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::EnableInvulnerability               , "DEC_Agent_ActiverInvulnerabilitePopulation"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::DisableInvulnerability              , "DEC_Agent_DesactiverInvulnerabilitePopulation"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::ChannelPopulations                  , "DEC_Agent_CanaliserPopulation"                                  );

    // Agent knowledges accessors    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::GetOperationalState                       < MIL_AgentPion >, "DEC_ConnaissanceAgent_EtatOps"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::GetMajorOperationalState                  < MIL_AgentPion >, "DEC_ConnaissanceAgent_EtatOpsMajeur"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::GetSpeed                                  < MIL_AgentPion >, "DEC_ConnaissanceAgent_Vitesse"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::GetPosition                               < MIL_AgentPion >, "DEC_ConnaissanceAgent_Position"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsFlying                                  < MIL_AgentPion >, "DEC_ConnaissanceAgent_EstEnVol"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::GetNatureAtlas                                             , "DEC_ConnaissanceAgent_NatureAtlas"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::GetNatureWeapon                                            , "DEC_ConnaissanceAgent_NatureArme"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::GetMaxPerceptionLevelForKnowledgeGroup    < MIL_AgentPion >, "DEC_ConnaissanceAgent_NiveauPerceptionMax"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::GetDangerosity                                             , "DEC_ConnaissanceAgent_Dangerosite"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::GetDangerosityOnPion                      < MIL_AgentPion >, "DEC_ConnaissanceAgent_DangerositeSurPion"              );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::GetDangerosityOnKnowledge                 < MIL_AgentPion >, "DEC_ConnaissanceAgent_DangerositeSurConnaissance"      );   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsPerceivedByAgent                                         , "DEC_ConnaissanceAgent_EstPercuParUnite"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsPerceivedByKnowledgeGroup                                , "DEC_ConnaissanceAgent_EstPercuParGroupeDeConnaissance" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsAnEnemy                                                  , "DEC_ConnaissanceAgent_EstUnEnnemi"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsKnowledgeValid                          < MIL_AgentPion >, "DEC_ConnaissanceAgent_EstValide"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsMoving                                                   , "DEC_ConnaissanceAgent_EstEnMouvement"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsPerceivingAgent                                          , "DEC_ConnaissanceAgent_PercoitUnite"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsSurrendered                             < MIL_AgentPion >, "DEC_ConnaissanceAgent_SEstRendu"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsPrisoner                                < MIL_AgentPion >, "DEC_ConnaissanceAgent_EstPrisonnier"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsRefugee                                 < MIL_AgentPion >, "DEC_ConnaissanceAgent_EstRefugie"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsRefugeeManaged                          < MIL_AgentPion >, "DEC_ConnaissanteAgent_EstRefugiePrisEnCompte"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsMilitia                                 < MIL_AgentPion >, "DEC_ConnaissanceAgent_EstMilice"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsTerrorist                               < MIL_AgentPion >, "DEC_ConnaissanceAgent_EstTerroriste"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsDead                                    < MIL_AgentPion >, "DEC_ConnaissanceAgent_EstMort"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::Lock                                      < MIL_AgentPion >, "DEC_ConnaissanceAgent_Verrouiller"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::Unlock                                    < MIL_AgentPion >, "DEC_ConnaissanceAgent_Deverrouiller"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsInSameDirection                                          , "DEC_ConnaissanceAgent_EstDansDirection"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::IsInDetectionCone                                          , "DEC_ConnaissanceAgent_EstAPorteDeCapteurDansCone"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions::KillOfficers                                               , "DEC_ConnaissanceAgent_TuerOfficiers"                   );

    // Object knowledges accessors
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsKnowledgeValid                 < MIL_AgentPion >, "DEC_ConnaissanceObjet_EstValide"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsPrepared                       < MIL_AgentPion >, "DEC_ConnaissanceObjet_EstPrepare"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsBypassed                       < MIL_AgentPion >, "DEC_ConnaissanceObjet_EstContourne"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::GetLocalisation                  < MIL_AgentPion >, "DEC_ConnaissanceObjet_Localisation"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::GetType                          < MIL_AgentPion >, "DEC_ConnaissanceObjet_Type"                              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth       < MIL_AgentPion >, "DEC_ConnaissanceObjet_LargeurSiteFranchissement"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsSiteFranchissementBanksToFitOut< MIL_AgentPion >, "DEC_ConnaissanceObjet_BergesAAmenagerSiteFranchissement" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::Recon											 , "DEC_ConnaissanceObjet_Reconnaitre"                       );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsRecon                          < MIL_AgentPion >, "DEC_ConnaissanceObjet_EstReconnu"                        );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::CanBeOccupied 									 , "DEC_ConnaissanceObjet_PeutEtreOccupe"                    ); 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::CanBeAnimated                                     , "DEC_ConnaissanceObjet_PeutEtreAnime"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsLogisticRouteEquipped          < MIL_AgentPion >, "DEC_ConnaissanceObjet_ItineraireLogEstEquipe"            ); 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::DamageObject                                      , "DEC_ConnaissanceObjet_Degrader"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::SetExitingPopulationDensity                       , "DEC_ConnaisssanceObjet_ChangeDensitePopulationSortante"  ); 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::ResetExitingPopulationDensity                     , "DEC_ConnaisssanceObjet_ResetDensitePopulationSortante"   );

    // Population knowledges accessors
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::GetDominationState           < MIL_AgentPion >, "DEC_ConnaissancePopulation_Domination"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::Recon										 , "DEC_ConnaissancePopulation_Reconnaitre"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::IsRecon                      < MIL_AgentPion >, "DEC_ConnaissancePopulation_EstReconnu"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::IsPerceivedByAgent                            , "DEC_ConnaissancePopulation_EstPercueParUnite"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::GetDangerosity                                , "DEC_ConnaissancePopulation_Dangerosite"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::IsInZone                     < MIL_AgentPion >, "DEC_ConnaissancePopulation_EstDansZone"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::ClosestPoint                                  , "DEC_ConnaissancePopulation_PointPlusProche"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::Secure                                        , "DEC_ConnaissancePopulation_Securiser"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::SecuringPoint                                 , "DEC_ConnaissancePopulation_PointSecurisation"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::IsEnemy                                       , "DEC_ConnaissancePopulation_EstEnnemi"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::Exterminate                                   , "DEC_ConnaissancePopulation_Exterminer"                   );
    
    // Global knowledge
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetRapForLocal                                          , "DEC_RapportDeForceLocal"                            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetAgentsAttacking                                      , "DEC_Connaissances_AgentsPrenantAPartie"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetPerceivedAgents                                      , "DEC_Connaissances_AgentsPercus"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetFriendsInZone                       < MIL_AgentPion >, "DEC_Connaissances_AmisDansZone"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetPotentiallyDangerousAgents                           , "DEC_Connaissances_AgentsPotentiellementDangereux"   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetMilitiaAgents                                        , "DEC_Connaissances_Milices"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetTerroristAgents                                      , "DEC_Connaissances_Terroristes"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetRefugeeAgents                                        , "DEC_Connaissances_Refugies"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetSurrenderedAgents                                    , "DEC_Connaissances_AgentsRendus"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetObjects                             < MIL_AgentPion >, "DEC_Connaissances_Objets"                           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetObjectsColliding                                     , "DEC_Connaissances_CollisionsObjets"                 );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetClosestObject                                        , "DEC_Connaissances_ObjetLePlusProche"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetClosestFriendObject                                  , "DEC_Connaissances_ObjetAmiLePlusProche"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetPopulationsColliding                                 , "DEC_Connaissances_CollisionsPopulations"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetPopulationsAttacking                                 , "DEC_Connaissances_PopulationsPrenantAPartie"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::GetPopulations                         < MIL_AgentPion >, "DEC_Connaissances_Populations"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::ShareKnowledgesWith                    < MIL_AgentPion >, "DEC_Connaissances_PartageConnaissancesAvec"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions::ShareKnowledgesInZoneWith              < MIL_AgentPion >, "DEC_Connaissances_PartageConnaissancesDansZoneAvec" );


    // Ordres de conduite
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::WaitForOrder       < MIL_AgentPion >, "DEC_AttenteOrdre"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::StopWaitingForOrder< MIL_AgentPion >, "DEC_AnnuleAttenteOrdre" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions ::RelievePion                         , "DEC_ReleverUnite"       );

    // Limas / Mission
    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::IsNewMissionStarted       < MIL_AgentPion >, "DEC_NouvelleMission"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::FinishMission             < MIL_AgentPion >, "DEC_FinMission"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::SetMissionLimaFlag        < MIL_AgentPion >, "DEC_SetMissionLimaFlag"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GetMissionLimaFlag        < MIL_AgentPion >, "DEC_GetMissionLimaFlag"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GetLima                   < MIL_AgentPion >, "DEC_GetLima"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::AutomateSetMissionLimaFlag                 , "DEC_Automate_SetMissionLimaFlag" );

    // Etat décisionnel
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::NotifyForceRatioStateChanged                 , "DEC_Agent_ChangeEtatRapportDeForce"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::NotifyRulesOfEngagementStateChanged          , "DEC_Agent_ChangeEtatROE"                    ); 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::NotifyRulesOfEngagementPopulationStateChanged, "DEC_Agent_ChangeEtatROEPopulation"          ); 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::NotifyCloseCombatStateChanged                , "DEC_Agent_ChangeEtatCombatDeRencontre"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::NotifyOperationalStateChanged                , "DEC_Agent_ChangeEtatOperationnel"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::NotifyIndirectFireAvailabilityChanged        , "DEC_Agent_ChangeDisponibiliteAuTirIndirect" );

    // Facteurs humains
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetHumanFactorTiredness , "DEC_FacteurHumain_Fatigue"    );  
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetHumanFactorExperience, "DEC_FacteurHumain_Experience" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetHumanFactorMorale    , "DEC_FacteurHumain_Moral"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetHumanFactorWeight    , "DEC_ForceFacteursHumains"     );

    // Calculs de position
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::GetInterceptionPosition                                 , "DEC_Geometrie_PositionInterception"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions   ::GetInterceptionPoint                                    , "DEC_Agent_PositionInterception"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeChantierPosition                                 , "DEC_Geometrie_CalculerPositionChantier"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeSupportPosition                                  , "DEC_Geometrie_CalculerPositionAppui"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeAmbushPosition                                   , "DEC_Geometrie_CalculerPositionEmbuscade"                 );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeSafetyPosition                                   , "DEC_Geometrie_CalculerPositionSurete"                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeSafetyPositionWithPopulation                     , "DEC_Geometrie_CalculerPositionSureteAvecPopulation"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeSafetyPositionWithObjective                      , "DEC_Geometrie_CalculerPositionSureteAvecObjectif"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau< MIL_AgentPion> , "DEC_Geometrie_CalculePointProcheLocalisationDansFuseau"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau  < MIL_AgentPion >, "DEC_Geometrie_CalculeBarycentreLocalisationDansFuseau"   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeNearestFuseauEntryPoint                          , "DEC_Geometrie_CalculeEntreeFuseauLaPlusProche"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::SplitLocalisationInParts               < MIL_AgentPion >, "DEC_Geometrie_DecoupeLocalisation"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::SplitLocalisationInSurfaces            < MIL_AgentPion >, "DEC_Geometrie_DecoupeLocalisationSurface"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::SplitLocalisationInSections            < MIL_AgentPion >, "DEC_Geometrie_DecoupeFuseauEnTroncons"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeObstaclePosition                < MIL_AgentPion >, "DEC_Geometrie_CalculerPositionObstacle"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDestPoint                       < MIL_AgentPion >, "DEC_Geometrie_CalculePointArrivee"                       ); 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeStartPoint                      < MIL_AgentPion >, "DEC_Geometrie_CalculePointDepart"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::IsPointInFuseau                        < MIL_AgentPion >, "DEC_Geometrie_EstPointDansFuseau"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeKnowledgeAgentBarycenter                         , "DEC_Geometrie_CalculeBarycentreListeConnaissancesAgents" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeFrontestAgent                   < MIL_AgentPion >, "DEC_Geometrie_PionDevant"                                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeBackestAgent                    < MIL_AgentPion >, "DEC_Geometrie_PionDerriere"                              );    

    // Fire 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::GetMaxRangeToFireOnEnemy                 , "DEC_Tir_PorteeMaxPourTirerSurAgent"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::GetMinRangeToFireOnEnemy                 , "DEC_Tir_PorteeMinPourTirerSurAgent"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::GetMaxRangeToFireOnEnemyActualPosture    , "DEC_Tir_PorteeMaxPourTirerSurAgentPosturesReelles"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::GetMinRangeToFireOnEnemyActualPosture    , "DEC_Tir_PorteeMinPourTirerSurAgentPosturesReelles"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::GetMaxRangeToFireOnEnemyWhenUnloaded     , "DEC_Tir_PorteeMaxPourTirerDebarqueSurAgent"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::GetMaxRangeToBeFiredByEnemy              , "DEC_Tir_PorteeMaxPourEtreTireParAgent"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::GetMaxRangeToIndirectFire                , "DEC_Tir_PorteeMaxTirIndirect"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::GetMinRangeToIndirectFire                , "DEC_Tir_PorteeMinTirIndirect"                         );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::GetMaxRangeToIndirectFireWithoutAmmoCheck, "DEC_Tir_PorteeMaxTirIndirect_SansTesterEtatMunitions" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::GetMinRangeToIndirectFireWithoutAmmoCheck, "DEC_Tir_PorteeMinTirIndirect_SansTesterEtatMunitions" );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_FireFunctions::ThrowSmokeOnKnowledgeAgent               , "DEC_Tir_LancerFumigeneSurConnaissance"                 );

    // RC
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Operational< MIL_AgentPion >, "DEC_RC"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Message    < MIL_AgentPion >, "DEC_Message" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Warning    < MIL_AgentPion >, "DEC_Warning" );
    
    // Hiérarchie
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetPionsWithPC             , "DEC_Pion_PionsAvecPC"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetPionsWithoutPC          , "DEC_Pion_PionsSansPC"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetPionPC                  , "DEC_Pion_PionPC"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetPionPCOfAutomate        , "DEC_Pion_PionPCDeAutomate"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::GetPionsWithoutPCOfAutomate, "DEC_Pion_PionsDeAutomateSansPC" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AgentFunctions::ChangeAutomate             , "DEC_Pion_ChangeAutomate"        );

    // Logistique
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions  ::StartAction< PHY_ActionLendCollectionComposantes >, "DEC_StartPreterVSRAM"        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions  ::StartAction< PHY_ActionLendHaulerComposantes     >, "DEC_StartPreterRemorqueurs"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::UndoLendCollectionComposantes                     , "DEC_RecupererVSRAM"          );   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::UndoLendHaulerComposantes                         , "DEC_RecupererRemorqueurs"    );   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionGetTC2                                        , "DEC_Pion_TC2"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::PionRequestSupply                                 , "DEC_DemandeDeRavitaillement" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::ChangeDotationValueUsingTC2                       , "DEC_ChangeValeurDotations"   );
    
    // Transport / Héliportage
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::Transport_AddPion                       , "DEC_Transport_AjouterPion"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::Transport_AddPions                      , "DEC_Transport_AjouterPions"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction< PHY_ActionTransportLoad   >, "DEC_Transport_StartEmbarquer"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::StartAction< PHY_ActionTransportUnload >, "DEC_Transport_StartDebarquer"           );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::Transport_MagicLoadPion                 , "DEC_Transport_EmbarquerPionSansDelais"  );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::Transport_MagicLoadPions                , "DEC_Transport_EmbarquerPionsSansDelais" );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::Transport_MagicUnloadPion               , "DEC_Transport_DebarquerPionSansDelais"  );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::Transport_MagicUnloadPions              , "DEC_Transport_DebarquerPionsSansDelais" );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::Transport_IsFinished                    , "DEC_Transport_EstTermine"               );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::Transport_Cancel                        , "DEC_Transport_Annuler"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::Transport_IsTransporting                , "DEC_Transport_EnCoursDeTransport"       );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::CanTransportPion                        , "DEC_Agent_PeutTransporterPion"          );        

    // Prisonniers
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::TakePrisoner, "DEC_FairePrisonnier" );

    // Refugies
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ActionFunctions::OrientateRefugee, "DEC_OrienterRefugies" );

}

// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePion::InstanciatePion( MIL_Automate& automate, MIL_InputArchive& archive ) const
{
    return *new MIL_AgentPion( automate, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InstanciatePion
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePion::InstanciatePion( uint nID, MIL_InputArchive& archive ) const
{
    return *new MIL_AgentPion( *this, nID, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AgentTypePion::InstanciatePion
// Created: NLD 2005-02-08
// -----------------------------------------------------------------------------
MIL_AgentPion& MIL_AgentTypePion::InstanciatePion( uint nID, MIL_Automate& automate, const MT_Vector2D& vPosition ) const
{
    return *new MIL_AgentPion( *this, nID, automate, vPosition );
}
