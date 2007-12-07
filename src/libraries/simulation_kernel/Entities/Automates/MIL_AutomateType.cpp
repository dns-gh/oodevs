// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/MIL_AutomateType.cpp $
// $Author: Jvt $
// $Modtime: 23/05/05 16:35 $
// $Revision: 47 $
// $Workfile: MIL_AutomateType.cpp $
//
// *****************************************************************************

#include "simulation_kernel_pch.h"

#include "MIL_AutomateType.h"

#include "MIL_Automate.h"

#include "Entities/Agents/MIL_AgentTypePion.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Decision/DEC_Workspace.h"
#include "Decision/DEC_Tools.h"
#include "Decision/Functions/DEC_AutomateFunctions.h"
#include "Decision/Functions/DEC_OrdersFunctions.h"
#include "Decision/Functions/DEC_GeometryFunctions.h"
#include "Decision/Functions/DEC_KnowledgeFunctions.h"
#include "Decision/Functions/DEC_KnowledgeAgentFunctions.h"
#include "Decision/Functions/DEC_KnowledgeObjectFunctions.h"
#include "Decision/Functions/DEC_KnowledgePopulationFunctions.h"
#include "Decision/Functions/DEC_MiscFunctions.h"
#include "Decision/Functions/DEC_PathFunctions.h"
#include "Decision/Functions/DEC_LogisticFunctions.h"
#include "Decision/Functions/DEC_ObjectFunctions.h"
#include "Decision/Functions/DEC_IntelligenceFunctions.h"
#include "MIL_AgentServer.h"
#include "Entities/Specialisations/ASA/MIL_AutomateTypeASA.h"
#include "Entities/Specialisations/LOG/MIL_AutomateTypeLOG.h"
#include "Entities/Specialisations/REFUGIE/MIL_AutomateTypeREFUGIE.h"
#include "Knowledge/DEC_Knowledge_RapFor_ABC.h"
#include "tools/xmlcodecs.h"
#include "xeumeuleu/xml.h"

using namespace xml;

MIL_AutomateType::T_AutomateTypeAllocatorMap  MIL_AutomateType::automateTypeAllocators_;
MIL_AutomateType::T_AutomateTypeMap           MIL_AutomateType::automateTypes_;

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::Create
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateType::Create( const std::string& strName, xml::xistream& xis )
{
    return new MIL_AutomateType( strName, xis );
}

struct MIL_AutomateType::LoadingWrapper
{
    void ReadAutomat( xml::xistream& xis )
    {
        MIL_AutomateType::ReadAutomat( xis );
    }
};

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AutomateType::Initialize( xml::xistream& xis )
{
    MT_LOG_INFO_MSG( "Initializing automate types" );

    automateTypeAllocators_[ "Automate INF"                    ] = &MIL_AutomateType       ::Create; 
    automateTypeAllocators_[ "Automate ASA"                    ] = &MIL_AutomateTypeASA    ::Create; 
    automateTypeAllocators_[ "Automate ALAT"                   ] = &MIL_AutomateType       ::Create; 
    automateTypeAllocators_[ "Automate RENS"                   ] = &MIL_AutomateType       ::Create; 
    automateTypeAllocators_[ "Automate NBC"                    ] = &MIL_AutomateType       ::Create; 
    automateTypeAllocators_[ "Automate ABC"                    ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate GEN"                    ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate ASS"                    ] = &MIL_AutomateType       ::Create; 
    automateTypeAllocators_[ "Automate TRANS"                  ] = &MIL_AutomateType       ::Create; 
    automateTypeAllocators_[ "Automate LOG TC2"                ] = &MIL_AutomateTypeLOG    ::Create; 
    automateTypeAllocators_[ "Automate LOG BLD Sante"          ] = &MIL_AutomateTypeLOG    ::Create; 
    automateTypeAllocators_[ "Automate LOG BLD Maintenance"    ] = &MIL_AutomateTypeLOG    ::Create; 
    automateTypeAllocators_[ "Automate LOG BLD Ravitaillement" ] = &MIL_AutomateTypeLOG    ::Create; 
    automateTypeAllocators_[ "Automate LOG BLT Sante"          ] = &MIL_AutomateTypeLOG    ::Create; 
    automateTypeAllocators_[ "Automate LOG BLT Maintenance"    ] = &MIL_AutomateTypeLOG    ::Create; 
    automateTypeAllocators_[ "Automate LOG BLT Ravitaillement" ] = &MIL_AutomateTypeLOG    ::Create; 
    automateTypeAllocators_[ "Automate JOINT"                  ] = &MIL_AutomateType       ::Create; 
    automateTypeAllocators_[ "Automate CIRCULATION"            ] = &MIL_AutomateType       ::Create; 
    automateTypeAllocators_[ "Automate REFUGIE"                ] = &MIL_AutomateTypeREFUGIE::Create; 
    automateTypeAllocators_[ "Automate MILICE"                 ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate ASY"                    ] = &MIL_AutomateType       ::Create;
    automateTypeAllocators_[ "Automate Emergency"              ] = &MIL_AutomateType       ::Create;
    
    LoadingWrapper loader;

    xis >> start( "automats" )
            >> list( "automat", loader, &LoadingWrapper::ReadAutomat )
        >> end();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::ReadAutomat
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_AutomateType::ReadAutomat( xml::xistream& xis )
{
    std::set< uint > ids_;
    std::string strName;
    std::string strType;

    xis >> attribute( "name", strName )
        >> attribute( "type", strType );

    CIT_AutomateTypeAllocatorMap itAutomateAllocator = automateTypeAllocators_.find( strType );
    if( itAutomateAllocator == automateTypeAllocators_.end() )
        xis.error( "Unknown automate type" );

    const MIL_AutomateType*& pType = automateTypes_[ strName ];
    if( pType )
        xis.error( "Automate type already defined" );

    pType = (*itAutomateAllocator->second)( strName, xis );

    if( !ids_.insert( pType->GetID() ).second )
        xis.error( "Automate type ID already used" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::Terminate
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AutomateType::Terminate()
{
    for( CIT_AutomateTypeMap it = automateTypes_.begin(); it != automateTypes_.end(); ++it )
        delete it->second;
    automateTypes_.clear();
}

// =============================================================================
// INIT
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType constructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_AutomateType::MIL_AutomateType( const std::string& strName, xml::xistream& xis )
    : nID_                            ( 0 )
    , strName_                        ( strName )
    , pDIAFunctionTable_              ( new DIA_FunctionTable< MIL_Automate >() )
    , pTypePC_                        ( 0 )
    , pModel_                         ( 0 )
    , rRapForIncreasePerTimeStepValue_( DEC_Knowledge_RapFor_ABC::GetRapForIncreasePerTimeStepDefaultValue() )
{
    xis >> attribute( "id", nID_ )
        >> list( "unit", *this, &MIL_AutomateType::ReadUnit );
    if( !pTypePC_ )
        xis.error( "No command-post defined for automat type: " + strName_ );

    InitializeRapFor      ( xis );
    InitializeModel       ( xis );
    InitializeDiaFunctions();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType destructor
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
MIL_AutomateType::~MIL_AutomateType()
{
    delete pDIAFunctionTable_;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::ReadUnit
// Created: ABL 2007-07-23
// -----------------------------------------------------------------------------
void MIL_AutomateType::ReadUnit( xml::xistream& xis )
{
    std::string strPionType;
    xis >> attribute( "type", strPionType );

    const MIL_AgentTypePion* pType = MIL_AgentTypePion::Find( strPionType );
    if( !pType )
        xis.error( "Unknown pawn type" );

    if( composition_.find( pType ) != composition_.end() )
        xis.error( "Pawn type already defined in composition" );

    composition_[ pType ].nMax_ = std::numeric_limits< uint >::max();
    composition_[ pType ].nMin_ = 0;

    xis >> optional() >> attribute( "min-occurs", composition_[ pType ].nMin_ )
        >> optional() >> attribute( "max-occurs", composition_[ pType ].nMax_ );

    bool isCommandPost = false;
    xis >> optional() >> attribute( "command-post", isCommandPost );
    if( isCommandPost )
        if( !pTypePC_ )
            pTypePC_ = pType;
        else
            xis.error( "Multiple command-post defined in automat type: " + strName_ ); 
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InitializeModel
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_AutomateType::InitializeModel( xml::xistream& xis )
{
    std::string strModel;
    xis >> attribute( "decisional-model", strModel );
    pModel_ = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelAutomate( strModel );
    if( !pModel_ )
        xis.error( "Unknown automata model" );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InitializeRapFor
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void MIL_AutomateType::InitializeRapFor( xml::xistream& xis )
{
    MT_Float rTimeTmp;
    if( tools::ReadTimeAttribute( xis, "force-ratio-feedback-time", rTimeTmp ) )
    {
        rTimeTmp                         = MIL_Tools::ConvertSecondsToSim( rTimeTmp );
        rRapForIncreasePerTimeStepValue_ = DEC_Knowledge_RapFor_ABC::ComputeRapForIncreasePerTimeStepValue( rTimeTmp );
    }
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::CheckComposition
// Created: NLD 2004-09-10
// Modified: JVT 2005-02-01
// -----------------------------------------------------------------------------
bool MIL_AutomateType::CheckComposition( const MIL_Automate& automate ) const
{
    typedef std::map< const MIL_AgentTypePion*, uint > T_CounterMap;
    typedef T_CounterMap::const_iterator               CIT_CounterMap;
    
    T_CounterMap currentComposition;
    const MIL_Automate::T_PionVector& pions = automate.GetPions();
    
    for( MIL_Automate::CIT_PionVector it = pions.begin(); it != pions.end(); ++it )
    {
        const MIL_AgentPion& pion = **it;

        if( pion == automate.GetPionPC() )
            continue;

        ++currentComposition[ &pion.GetType() ];
        if( composition_.find( &pion.GetType() ) == composition_.end() )
            return false;
    }
    
    for( CIT_CompositionMap it = composition_.begin(); it != composition_.end(); ++it )
    {
        const sCompositionBounds& bounds   = it->second;
        const uint&               nRealNbr = currentComposition[ it->first ];
        
        if ( bounds.nMin_ > nRealNbr || bounds.nMax_ < nRealNbr )
            return false;
    }
    
    return true;
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InitializeDiaFunctions
// Created: NLD 2004-10-14
// -----------------------------------------------------------------------------
void MIL_AutomateType::InitializeDiaFunctions()
{
    // Accessors
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionsWithPC         , "DEC_Automate_PionsAvecPC"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionsWithoutPC      , "DEC_Automate_PionsSansPC"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionPC              , "DEC_Automate_PionPC"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionPCOfAutomate    , "DEC_Automate_PionPCDeAutomate"     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetAutomates           , "DEC_Automate_AutomatesSubordonnes" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetParentAutomate      , "DEC_AutomateSuperieur"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsParentAutomateEngaged, "DEC_AutomateSuperieur_EstEmbraye"  );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC, "DEC_Automate_PionsDeAutomateSansPC" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionsOfAutomateWithPC   , "DEC_Automate_PionsDeAutomateAvecPC" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionPCOfAutomate        , "DEC_Automate_PionPCDeAutomate" );

    // State
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::NotifyForceRatioStateChanged       , "DEC_Automate_ChangeEtatRapportDeForce"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::NotifyRulesOfEngagementStateChanged, "DEC_Automate_ChangeEtatROE"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::NotifyCloseCombatStateChanged      , "DEC_Automate_ChangeEtatCombatDeRencontre" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::NotifyOperationalStateChanged      , "DEC_Automate_ChangeEtatOperationnel"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsEngaged                          , "DEC_Automate_EstEmbraye"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsSurrendered                      , "DEC_Automate_SEstRendu"                   );
       
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::DebrayeAutomate, "DEC_Automate_Debrayer" ); 
    
    // Debug
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::Trace          , "DEC_Trace"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::DecisionalState, "DEC_DecisionalState"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::Debug          , "DEC_Debug"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::DebugDrawPoint , "DEC_DebugAffichePoint"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::DebugDrawPoints, "DEC_DebugAffichePoints" );

    // Objets
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ObjectFunctions::ActivateObject< MIL_Automate >, "DEC_ActiverObjet" );

    // Connaissance
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::Lock                                  < MIL_Automate >, "DEC_ConnaissanceAgent_Verrouiller"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::Unlock                                < MIL_Automate >, "DEC_ConnaissanceAgent_Deverrouiller"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetPosition                           < MIL_Automate >, "DEC_ConnaissanceAgent_Position"                             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsFlying                              < MIL_Automate >, "DEC_ConnaissanceAgent_EstEnVol"                             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetSpeed                              < MIL_Automate >, "DEC_ConnaissanceAgent_Vitesse"                              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsKnowledgeValid                      < MIL_Automate >, "DEC_ConnaissanceAgent_EstValide"                            );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsRefugee                             < MIL_Automate >, "DEC_ConnaissanceAgent_EstRefugie"                           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsRefugeeManaged                      < MIL_Automate >, "DEC_ConnaissanteAgent_EstRefugiePrisEnCompte"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsMilitia                             < MIL_Automate >, "DEC_ConnaissanceAgent_EstMilice"                            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsTerrorist                           < MIL_Automate >, "DEC_ConnaissanceAgent_EstTerroriste"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsDead                                < MIL_Automate >, "DEC_ConnaissanceAgent_EstMort"                              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsSurrendered                         < MIL_Automate >, "DEC_ConnaissanceAgent_SEstRendu"                            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsPrisoner                            < MIL_Automate >, "DEC_ConnaissanceAgent_EstPrisonnier"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetDangerosityOnPion                  < MIL_Automate >, "DEC_ConnaissanceAgent_DangerositeSurPion"                   );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetDangerosityOnKnowledge             < MIL_Automate >, "DEC_ConnaissanceAgent_DangerositeSurConnaissance"           );   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetOperationalState                   < MIL_Automate >, "DEC_ConnaissanceAgent_EtatOps"                              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetMajorOperationalState              < MIL_Automate >, "DEC_ConnaissanceAgent_EtatOpsMajeur"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetMaxPerceptionLevelForKnowledgeGroup< MIL_Automate >, "DEC_ConnaissanceAgent_NiveauPerceptionMax"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsKnowledgeValid                      < MIL_Automate >, "DEC_ConnaissanceObjet_EstValide"                            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsReservedObstacleActivated           < MIL_Automate >, "DEC_ConnaissanceObjet_EstObstacleDeManoeuvreActif"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsReservedObstacle                    < MIL_Automate >, "DEC_ConnaissanceObjet_EstObstacleDeManoeuvre"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsBypassed                            < MIL_Automate >, "DEC_ConnaissanceObjet_EstContourne"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsConstructed                         < MIL_Automate >, "DEC_ConnaissanceObjet_EstConstruit"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::GetLocalisation                       < MIL_Automate >, "DEC_ConnaissanceObjet_Localisation"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::GetType                               < MIL_Automate >, "DEC_ConnaissanceObjet_Type"                                 );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth            < MIL_Automate >, "DEC_ConnaissanceObjet_LargeurSiteFranchissement"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsSiteFranchissementBanksToFitOut     < MIL_Automate >, "DEC_ConnaissanceObjet_BergesAAmenagerSiteFranchissement"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsRecon                               < MIL_Automate >, "DEC_ConnaissanceObjet_EstReconnu"                           );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsLogisticRouteEquipped               < MIL_Automate >, "DEC_ConnaissanceObjet_ItineraireLogEstEquipe"               ); 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::ShareKnowledgesWith                   < MIL_Automate >, "DEC_Connaissances_PartageConnaissancesAvec"                 );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::ShareKnowledgesInZoneWith             < MIL_Automate >, "DEC_Connaissances_PartageConnaissancesDansZoneAvec"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetObjects                            < MIL_Automate >, "DEC_Connaissances_Objets"                                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetObjectsInCircle                    < MIL_Automate >, "DEC_Connaissances_ObjetsDansCercle"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetObjectsInZone                      < MIL_Automate >, "DEC_Connaissances_ObjetsDansZone"                           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetObjectsInFuseau                    < MIL_Automate >, "DEC_Connaissances_ObjetsDansFuseau"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetFriendsInZone                      < MIL_Automate >, "DEC_Connaissances_UnitesAmiesDansZone"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetLivingEnemiesPerceivedByPion       < MIL_Automate >, "DEC_Connaissances_UnitesEnnemiesVivantesPercuesParPion"     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetDangerousEnemiesInZoneOfPion       < MIL_Automate >, "DEC_Connaissances_UnitesEnnemiesDangereusesDansZoneDePion"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetRapForGlobal                                       , "DEC_RapportDeForceGlobal"                                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetPopulations                        < MIL_Automate >, "DEC_Connaissances_Populations"                              );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::ComputeUnloadedEnemiesRatio           < MIL_Automate >, "DEC_Connaissances_PourcentageEnnemisDebarquesDansZone"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::ComputeLoadedEnemiesRatio             < MIL_Automate >, "DEC_Connaissances_PourcentageEnnemisEmbarquesDansZone"      );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::SortAccordingToUnloadedEnemies        < MIL_Automate >, "DEC_Connaissances_TrierZonesSelonPresenceEnnemisDebarques"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::SortAccordingToLoadedEnemies          < MIL_Automate >, "DEC_Connaissances_TrierZonesSelonPresenceEnnemisEmbarques"  );

    // Intelligence
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_IntelligenceFunctions   ::ComputeUnloadedEnemiesRatio                           , "DEC_Rens_PourcentageEnnemisDebarquesDansZone"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_IntelligenceFunctions   ::ComputeLoadedEnemiesRatio                             , "DEC_Rens_PourcentageEnnemisEmbarquesDansZone"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_IntelligenceFunctions   ::SortAccordingToUnloadedEnemies                        , "DEC_Rens_TrierZonesSelonPresenceEnnemisDebarques"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_IntelligenceFunctions   ::SortAccordingToLoadedEnemies                          , "DEC_Rens_TrierZonesSelonPresenceEnnemisEmbarques"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_IntelligenceFunctions   ::IsEnemyOnFlank                                        , "DEC_Rens_EnnemiSurFlanc"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_IntelligenceFunctions   ::IsFriendOnFlank                                       , "DEC_Rens_AmiSurFlanc"     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_IntelligenceFunctions   ::ComputeCoverDirection                                 , "DEC_Rens_CalculerDirectionCouverture" );

    // RCS
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Operational< MIL_Automate >, "DEC_RC"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Message    < MIL_Automate >, "DEC_Message" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Warning    < MIL_Automate >, "DEC_Warning" );

    // Geometry       
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau           < MIL_Automate >, "DEC_Geometrie_CalculerBarycentreLocalisationDansFuseau"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::SplitLocalisationInParts                        < MIL_Automate >, "DEC_Geometrie_DecoupeLocalisation"                                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::SplitLocalisationInSections                     < MIL_Automate >, "DEC_Geometrie_DecoupeFuseauEnTroncons"                               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeObstaclePosition                         < MIL_Automate >, "DEC_Geometrie_CalculerPositionObstacle"                              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDestPoint                                < MIL_Automate >, "DEC_Geometrie_CalculerPointArrivee"                                  ); 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeStartPoint                               < MIL_Automate >, "DEC_Geometrie_CalculerPointDepart"                                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::IsPointInFuseau                                 < MIL_Automate >, "DEC_Geometrie_EstPointDansFuseau"                                    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputePointBeforeLima                          < MIL_Automate >, "DEC_Geometrie_CalculerPositionParRapportALima"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputePointsBeforeLima                                         , "DEC_Geometrie_CalculerPositionsParRapportALima"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeLocalisationPointsForPionsInFuseau                       , "DEC_Geometrie_PositionsParRapportALocalisation"                      );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::StartComputingFrontAndBackLines                                 , "DEC_Geometrie_StartCalculLignesAvantEtArriere"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::StopComputingFrontAndBackLines                                  , "DEC_Geometrie_StopCalculLignesAvantEtArriere"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDistanceFromFrontLine                                    , "DEC_Geometrie_CalculerDistanceLigneAvant"                            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDistanceFromBackLine                                     , "DEC_Geometrie_CalculerDistanceLigneArriere"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDistancePointFromFrontLine                               , "DEC_Geometrie_CalculerDistancePointLigneAvant"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDistancePointFromBackLine                                , "DEC_Geometrie_CalculerDistancePointLigneArriere"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDestPointForPion                                         , "DEC_Geometrie_CalculerPointArriveePourPion"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau         < MIL_Automate >, "DEC_Geometrie_CalculerPointProcheLocalisationDansFuseau"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeNearestUnclippedLocalisationPointInFuseau< MIL_Automate >, "DEC_Geometrie_CalculerPointProcheLocalisationNonClippeeDansFuseau"   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeFrontestAgent                            < MIL_Automate >, "DEC_Geometrie_PionDevant"                                            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeBackestAgent                             < MIL_Automate >, "DEC_Geometrie_PionDerriere"                                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::SortFuseauxAccordingToSchedule                  < MIL_Automate >, "DEC_Geometrie_TrierFuseauxSelonHoraire"                              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDelayFromSchedule                        < MIL_Automate >, "DEC_Geometrie_CalculerRetard"                                        );

    // Orders
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::IsNewMissionStarted               < MIL_Automate >, "DEC_NouvelleMission"                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::FinishMission                     < MIL_Automate >, "DEC_FinMission"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GetLima                           < MIL_Automate >, "DEC_GetLima"                        );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GetNextScheduledLima              < MIL_Automate >, "DEC_ProchaineLimaHoraireNonFlagee"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GetNextScheduledElement           < MIL_Automate >, "DEC_ProchainElementHoraireNonFlage" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GetFuseau                         < MIL_Automate >, "DEC_Fuseau"                         );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::AutomateSetMissionLimaFlag                        , "DEC_SetMissionLimaFlag"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GetMissionLimaFlag                < MIL_Automate >, "DEC_GetMissionLimaFlag"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::AutomateSetMissionLimaScheduleFlag                , "DEC_SetMissionLimaFlagHoraire"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GetMissionLimaScheduleFlag        < MIL_Automate >, "DEC_GetMissionLimaFlagHoraire"      );

    // MRT / conduite
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::MRT_CreatePionMission        , "DEC_MRT_CreerMissionPion"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::MRT_Validate                 , "DEC_MRT_Valide"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::MRT_AffectFuseaux            , "DEC_MRT_AffecteFuseaux"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::CDT_CreatePionMission        , "DEC_CDT_CreerMissionPion"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::CDT_GivePionMission          , "DEC_CDT_DonnerMissionPion"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::CreateAutomateMission        , "DEC_CreerMissionAutomate"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GiveAutomateMission          , "DEC_DonnerMissionAutomate"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::AssignFuseauToAutomateMission, "DEC_AssignerFuseauAMissionAutomate" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::SplitFuseau                  , "DEC_DecouperFuseau"                 );

    // Pion management
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::PionChangeAutomate , "DEC_Pion_ChangeAutomate" );
    
    // Accesseurs sur les pions   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsPionFlying                               , "DEC_Automate_PionEstEnVol"                                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsPionTransported                          , "DEC_Automate_PionEstTransporte"                                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsPionContaminated                         , "DEC_Automate_PionEstContamine"                                 );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsPionNBCProtected                         , "DEC_Automate_PionEstEnTenueProtectionNBC"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsPionMoving                               , "DEC_Automate_PionEstEnMouvement"                               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsPionNeutralized                          , "DEC_Automate_PionEstNeutralise"                                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionOperationalState                    , "DEC_Automate_PionEtatOps"                                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionMajorOperationalState               , "DEC_Automate_PionEtatOpsMajeur"                                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionPosition                            , "DEC_Automate_PionPosition"                                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionDirection                           , "DEC_Automate_PionDirection"                                    );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::MakePionRelievePion                        , "DEC_Automate_PionRelevePion"                                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::CanPionRelievePion                         , "DEC_Automate_PionPeutReleverPion"                              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsPointInPionFuseau                        , "DEC_Automate_EstPointDansFuseauPion"                           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::ComputePointBeforeLimaForPion              , "DEC_Automate_CalculerPositionParRapportALimaPourPion"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau, "DEC_Automate_CalculerPointProcheLocalisationDansFuseauPourPion");
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::CanPionConstructObject                     , "DEC_Automate_PionPeutConstruireObjet"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::CanPionBypassObject                        , "DEC_Automate_PionPeutConstruireContournementObjet"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::CanPionDestroyObject                       , "DEC_Automate_PionPeutDetruireObjet"                            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::CanPionMineObject                          , "DEC_Automate_PionPeutValoriserObjet"                           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::CanPionActivateObject                      , "DEC_Automate_PionPeutActiverObjet"                             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::PionTimeLeftForMoving                      , "DEC_Automate_PionAutonomieEnDeplacement"                       );   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::PionTimeToMoveDistance                     , "DEC_Automate_PionTempsPourParcourirDistanceEnLigneDroite"      );   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::CanPionTransportPion                       , "DEC_Automate_PionPeutTransporterPion"                          );
    
    // Logistique
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateGetTC2       , "DEC_Automate_TC2"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_LogisticFunctions::AutomateRequestSupply, "DEC_DemandeDeRavitaillement" );

    // Objects
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ObjectFunctions::MagicCreateObject < MIL_Automate >, "DEC_CreerObjetSansDelais"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ObjectFunctions::MagicDestroyObject< MIL_Automate >, "DEC_DetruireObjetSansDelais" );

    // Populations
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::GetDominationState< MIL_Automate >, "DEC_ConnaissancePopulation_Domination"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::IsInZone          < MIL_Automate >, "DEC_ConnaissancePopulation_EstDansZone" );
}


// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InstanciateAutomate
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateType::InstanciateAutomate( uint nID, MIL_Formation& parent, xml::xistream& xis ) const
{
    return *new MIL_Automate( *this, nID, parent, xis );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InstanciateAutomate
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateType::InstanciateAutomate( uint nID, MIL_Automate& parent, xml::xistream& xis ) const
{
    return *new MIL_Automate( *this, nID, parent, xis );
}


