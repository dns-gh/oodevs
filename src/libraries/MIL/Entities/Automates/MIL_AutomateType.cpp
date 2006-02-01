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

#include "MIL_pch.h"

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
#include "MIL_AgentServer.h"

#include "Entities/Specialisations/ASA/MIL_AutomateTypeASA.h"
#include "Entities/Specialisations/LOG/MIL_AutomateTypeLOG.h"
#include "Entities/Specialisations/REFUGIE/MIL_AutomateTypeREFUGIE.h"

#include "Knowledge/DEC_Knowledge_RapFor_ABC.h"

MIL_AutomateType::T_AutomateTypeAllocatorMap  MIL_AutomateType::automateTypeAllocators_;
MIL_AutomateType::T_AutomateTypeMap           MIL_AutomateType::automateTypes_;

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::Create
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
const MIL_AutomateType* MIL_AutomateType::Create( const std::string& strName, MIL_InputArchive& archive )
{
    return new MIL_AutomateType( strName, archive );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::Initialize
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AutomateType::Initialize( MIL_InputArchive& archive )
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
    
    std::set< uint > ids_;
    archive.BeginList( "Automates" );
    while( archive.NextListElement() )
    {
        archive.Section( "Unite" );

        std::string strName;
        std::string strType;

        archive.ReadAttribute( "nom", strName );
        archive.ReadAttribute( "type", strType );

        CIT_AutomateTypeAllocatorMap itAutomateAllocator = automateTypeAllocators_.find( strType );
        if( itAutomateAllocator == automateTypeAllocators_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown automate type", archive.GetContext() );

        const MIL_AutomateType*& pType = automateTypes_[ strName ];
        if( pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automate type already defined", archive.GetContext() );

        pType = (*itAutomateAllocator->second)( strName, archive );

        if( !ids_.insert( pType->GetID() ).second )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Automate type ID already used", archive.GetContext() );

        archive.EndSection(); // Unite
    }
    archive.EndList(); // Automates
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
MIL_AutomateType::MIL_AutomateType( const std::string& strName, MIL_InputArchive& archive )
    : nID_                            ( 0 )
    , strName_                        ( strName )
    , pDIAFunctionTable_              ( new DIA_FunctionTable< MIL_Automate >() )
    , pTypePC_                        ( 0 )
    , pModel_                         ( 0 )
    , rRapForIncreasePerTimeStepValue_( DEC_Knowledge_RapFor_ABC::GetRapForIncreasePerTimeStepDefaultValue() )
{
    archive.ReadField( "MosID", nID_ );

    // Automate
    archive.Section( "Automate" );

    InitializeComposition ( archive );
    InitializeRapFor      ( archive );
    InitializeModel       ( archive );
    InitializeDiaFunctions();

    archive.EndSection(); // Automate

    // PC
    archive.Section( "PionPC" );
    std::string strPCType;
    archive.ReadAttribute( "type", strPCType );    
    pTypePC_ = MIL_AgentTypePion::FindPionType( strPCType );
    if( !pTypePC_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown PC type", archive.GetContext() );

    archive.EndSection(); // PionPC
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
// Name: MIL_AutomateType::ParseComposition
// Created: JVT 2005-02-01
// -----------------------------------------------------------------------------
bool MIL_AutomateType::ParseComposition( std::string strBuf, sCompositionBounds& result )
{   // parser très pourri de + ou * ou n..m ou n
    uint i = 0;
    
    if ( strBuf[ i ] == '+' )
    {
        if ( strBuf.size() != 1 )
            return false;
        result.nMin_ = 1;
        result.nMax_ = std::numeric_limits< uint >::max();
        return true;
    }
    
    if ( strBuf[ i ] == '*' )
    {
        if ( strBuf.size() != 1 )
            return false;
        result.nMin_ = 0;
        result.nMax_ = std::numeric_limits< uint >::max();
        return true;
    }
    
    for ( result.nMin_ = 0; i < strBuf.size(); ++i )
        if ( strBuf[ i ] >= '0' && strBuf[ i ] <= '9' )
        {
            result.nMin_ *= 10;
            result.nMin_ += strBuf[ i ] - '0';
        }
        else
            break;
    
    if ( i == strBuf.size() )
    {
        result.nMax_ = result.nMin_;
        return true;
    }    
    
    if ( i + 2 >= strBuf.size() || strBuf[ i++ ] != '.' || strBuf[ i++ ] != '.' )
        return false;

    for ( result.nMax_ = 0; i < strBuf.size(); ++i )
        if ( strBuf[ i ] >= '0' && strBuf[ i ] <= '9' )
        {
            result.nMax_ *= 10;
            result.nMax_ += strBuf[ i ] - '0';
        }
        else
            break;
    
    if ( result.nMin_ > result.nMax_ )
        std::swap( result.nMin_, result.nMax_ );
    
    return i == strBuf.size();
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InitializeComposition
// Created: NLD 2004-08-09
// -----------------------------------------------------------------------------
void MIL_AutomateType::InitializeComposition( MIL_InputArchive& archive )
{
    archive.BeginList( "Constitution" );

    while( archive.NextListElement() )
    {
        archive.Section( "Pion" );

        std::string strPionType;
        archive.ReadAttribute( "nom", strPionType );

        const MIL_AgentTypePion* pType = MIL_AgentTypePion::FindPionType( strPionType );
        if( !pType )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown pawn type", archive.GetContext() );

        if( composition_.find( pType ) != composition_.end() )
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Pawn type already defined in composition", archive.GetContext() );

        std::string strBuf;
        archive.Read( strBuf );
    
        if ( !ParseComposition( strBuf, composition_[ pType ] ) )    
            throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown format for composition (should be + or * or n..m or n !)", archive.GetContext() );
        
        archive.EndSection(); // Pion
    }
    archive.EndList(); // Constitution
}


// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InitializeModel
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
void MIL_AutomateType::InitializeModel( MIL_InputArchive& archive )
{
    std::string strModel;
    archive.ReadField( "ModeleDecisionnel", strModel );

    pModel_ = MIL_AgentServer::GetWorkspace().GetWorkspaceDIA().FindModelAutomate( strModel );
    if( !pModel_ )
        throw MT_ScipioException( __FUNCTION__, __FILE__, __LINE__, "Unknown automata model", archive.GetContext() );
}

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InitializeRapFor
// Created: NLD 2004-11-25
// -----------------------------------------------------------------------------
void MIL_AutomateType::InitializeRapFor( MIL_InputArchive& archive )
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
    
    for ( MIL_Automate::CIT_PionVector itPion = pions.begin(); itPion != pions.end(); ++itPion )
    {
        const MIL_AgentPion& pion = **itPion;

        if( pion == automate.GetPionPC() )
            continue;

        ++currentComposition[ &pion.GetType() ];
        if ( composition_.find( &pion.GetType() ) == composition_.end() )
            return false;
    }
    
    for ( CIT_CompositionMap it = composition_.begin(); it != composition_.end(); ++it )
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
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionsWithPC      , "DEC_Automate_PionsAvecPC"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionsWithoutPC   , "DEC_Automate_PionsSansPC"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionPC           , "DEC_Automate_PionPC"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionPCOfAutomate , "DEC_Automate_PionPCDeAutomate" );

    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionsOfAutomateWithoutPC, "DEC_Automate_PionsDeAutomateAvecPC" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionsOfAutomateWithPC   , "DEC_Automate_PionsDeAutomateSansPC" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::GetPionPCOfAutomate        , "DEC_Automate_PionPCDeAutomate" );

    // State
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::NotifyForceRatioStateChanged       , "DEC_Automate_ChangeEtatRapportDeForce"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::NotifyRulesOfEngagementStateChanged, "DEC_Automate_ChangeEtatROE"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::NotifyCloseCombatStateChanged      , "DEC_Automate_ChangeEtatCombatDeRencontre" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::NotifyOperationalStateChanged      , "DEC_Automate_ChangeEtatOperationnel"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsEmbraye                          , "DEC_Automate_EstEmbraye"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsSurrendered                      , "DEC_Automate_SEstRendu"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsPrisoner                         , "DEC_Automate_EstPrisonnier"               );
       
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::DebrayeAutomate, "DEC_Automate_Debrayer" ); 
    
    // Debug
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::Trace          , "DEC_Trace"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::Debug          , "DEC_Debug"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::DebugDrawPoint , "DEC_DebugAffichePoint"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::DebugDrawPoints, "DEC_DebugAffichePoints" );

    // Objets
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ObjectFunctions::ActivateObject< MIL_Automate >, "DEC_ActiverObjet" );

    // Connaissance
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::Lock                                  < MIL_Automate >, "DEC_ConnaissanceAgent_Verrouiller"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::Unlock                                < MIL_Automate >, "DEC_ConnaissanceAgent_Deverrouiller"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetPosition                           < MIL_Automate >, "DEC_ConnaissanceAgent_Position"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsFlying                              < MIL_Automate >, "DEC_ConnaissanceAgent_EstEnVol"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetSpeed                              < MIL_Automate >, "DEC_ConnaissanceAgent_Vitesse"                           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsKnowledgeValid                      < MIL_Automate >, "DEC_ConnaissanceAgent_EstValide"                         );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsRefugee                             < MIL_Automate >, "DEC_ConnaissanceAgent_EstRefugie"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsRefugeeManaged                      < MIL_Automate >, "DEC_ConnaissanteAgent_EstRefugiePrisEnCompte"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsMilita                              < MIL_Automate >, "DEC_ConnaissanceAgent_EstMilice"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsDead                                < MIL_Automate >, "DEC_ConnaissanceAgent_EstMort"                           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsSurrendered                         < MIL_Automate >, "DEC_ConnaissanceAgent_SEstRendu"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::IsPrisoner                            < MIL_Automate >, "DEC_ConnaissanceAgent_EstPrisonnier"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetDangerosityOnPion                  < MIL_Automate >, "DEC_ConnaissanceAgent_DangerositeSurPion"                );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetDangerosityOnKnowledge             < MIL_Automate >, "DEC_ConnaissanceAgent_DangerositeSurConnaissance"        );   
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetOperationalState                   < MIL_Automate >, "DEC_ConnaissanceAgent_EtatOps"                           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetMajorOperationalState              < MIL_Automate >, "DEC_ConnaissanceAgent_EtatOpsMajeur"                     );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeAgentFunctions ::GetMaxPerceptionLevelForKnowledgeGroup< MIL_Automate >, "DEC_ConnaissanceAgent_NiveauPerceptionMax"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsKnowledgeValid                      < MIL_Automate >, "DEC_ConnaissanceObjet_EstValide"                         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsPrepared                            < MIL_Automate >, "DEC_ConnaissanceObjet_EstPrepare"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsBypassed                            < MIL_Automate >, "DEC_ConnaissanceObjet_EstContourne"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::GetLocalisation                       < MIL_Automate >, "DEC_ConnaissanceObjet_Localisation"                      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::GetType                               < MIL_Automate >, "DEC_ConnaissanceObjet_Type"                              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::GetSiteFranchissementWidth            < MIL_Automate >, "DEC_ConnaissanceObjet_LargeurSiteFranchissement"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsSiteFranchissementBanksToFitOut     < MIL_Automate >, "DEC_ConnaissanceObjet_BergesAAmenagerSiteFranchissement" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsRecon                               < MIL_Automate >, "DEC_ConnaissanceObjet_EstReconnu"                        );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeObjectFunctions::IsLogisticRouteEquipped               < MIL_Automate >, "DEC_ConnaissanceObjet_ItineraireLogEstEquipe"            ); 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::ShareKnowledgesWith                   < MIL_Automate >, "DEC_Connaissances_PartageConnaissancesAvec"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::ShareKnowledgesInZoneWith             < MIL_Automate >, "DEC_Connaissances_PartageConnaissancesDansZoneAvec"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetObjects                            < MIL_Automate >, "DEC_Connaissances_Objets"                                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetFriendsInZone                      < MIL_Automate >, "DEC_Connaissances_AmisDansZone"                          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetRapForGlobal                                       , "DEC_RapportDeForceGlobal"                                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgeFunctions      ::GetPopulations                   < MIL_Automate >, "DEC_Connaissances_Populations"                           );
    
    // RCS
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Operational< MIL_Automate >, "DEC_RC"      );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Message    < MIL_Automate >, "DEC_Message" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_MiscFunctions::RC_Warning    < MIL_Automate >, "DEC_Warning" );

    // Geometry       
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeLocalisationBarycenterInFuseau  < MIL_Automate >, "DEC_Geometrie_CalculeBarycentreLocalisationDansFuseau"   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::SplitLocalisationInParts               < MIL_Automate >, "DEC_Geometrie_DecoupeLocalisation"                       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::SplitLocalisationInSections            < MIL_Automate >, "DEC_Geometrie_DecoupeFuseauEnTroncons"                   );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeObstaclePosition                < MIL_Automate >, "DEC_Geometrie_CalculerPositionObstacle"                  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDestPoint                       < MIL_Automate >, "DEC_Geometrie_CalculePointArrivee"                       ); 
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeStartPoint                      < MIL_Automate >, "DEC_Geometrie_CalculePointDepart"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::IsPointInFuseau                        < MIL_Automate >, "DEC_Geometrie_EstPointDansFuseau"                        );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputePointBeforeLima                 < MIL_Automate >, "DEC_Geometrie_CalculerPositionParRapportALima"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputePointsBeforeLima                                , "DEC_Geometrie_CalculerPositionsParRapportALima"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeLocalisationPointsForPionsInFuseau              , "DEC_Geometrie_PositionsParRapportALocalisation"          );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::StartComputingFrontAndBackLines                        , "DEC_Geometrie_StartCalculLignesAvantEtArriere"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::StopComputingFrontAndBackLines                         , "DEC_Geometrie_StopCalculLignesAvantEtArriere"            );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDistanceFromFrontLine                           , "DEC_Geometrie_CalculeDistanceLigneAvant"                 );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDistanceFromBackLine                            , "DEC_Geometrie_CalculeDistanceLigneArriere"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeDestPointForPion                                , "DEC_Geometrie_CalculePointArriveePourPion"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeNearestLocalisationPointInFuseau< MIL_Automate >, "DEC_Geometrie_CalculePointProcheLocalisationDansFuseau"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeFrontestAgent                   < MIL_Automate >, "DEC_Geometrie_PionDevant"                                );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_GeometryFunctions::ComputeBackestAgent                    < MIL_Automate >, "DEC_Geometrie_PionDerriere"                              );

    // Orders
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::IsNewMissionStarted< MIL_Automate >, "DEC_NouvelleMission"    );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::FinishMission      < MIL_Automate >, "DEC_FinMission"         );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::SetMissionLimaFlag < MIL_Automate >, "DEC_SetMissionLimaFlag" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GetMissionLimaFlag < MIL_Automate >, "DEC_GetMissionLimaFlag" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::GetLima            < MIL_Automate >, "DEC_GetLima"            );    
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::WaitForOrder       < MIL_Automate >, "DEC_AttenteOrdre"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::StopWaitingForOrder< MIL_Automate >, "DEC_AnnuleAttenteOrdre" );

    // MRT
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::MRT_CreatePionMission                , "DEC_MRT_CreerMissionPion" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::MRT_Validate                         , "DEC_MRT_Valide"           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::MRT_AffectFuseaux                    , "DEC_MRT_AffecteFuseaux"   );

    // Conduite
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::CDT_SendOrderConduiteToPion          , "DEC_CDT_DonneOrdreConduiteAPion"       );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::CDT_CreatePionMission                , "DEC_CDT_CreerMissionPion"              );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::CDT_GivePionMission                  , "DEC_CDT_DonnerMissionPion"             );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::CDT_IsPionWaitingForOrderConduite    , "DEC_CDT_PionEnAttenteOC"               );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::CDT_GetPionsWaitingForOrderConduite  , "DEC_CDT_ObtenirListePionsEnAttenteOC"  );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_OrdersFunctions::CDT_GetListOrderConduiteWaitedByPion , "DEC_CDT_ObtenirListeOCAttendusParPion" );     

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
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_PathFunctions::GetNextObjectOnPionPath                        , "DEC_Automate_ProchainObjetSurItinerairePion"                   );       
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::IsPointInPionFuseau                        , "DEC_Automate_EstPointDansFuseauPion"                           );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::ComputePointBeforeLimaForPion              , "DEC_Automate_CalculerPositionParRapportALimaPourPion"          );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::ComputePionNearestLocalisationPointInFuseau, "DEC_Automate_CalculePointProcheLocalisationDansFuseauPourPion" );    
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

    // Prisonniers
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ObjectFunctions::GetPrisonerCampPosition, "DEC_PositionCampPrisonniers" );

    // Refugies
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_ObjectFunctions  ::GetRefugeeCampPosition          , "DEC_PositionCampRefugies" );
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_AutomateFunctions::NotifyRefugeeManagedStateChanged, "DEC_Automate_ChangeEtatPriseEnCompteRefugies" );

    // Populations
    DEC_RegisterDIACallFunctor( GetFunctionTable(), &DEC_KnowledgePopulationFunctions::IsInZone< MIL_Automate >, "DEC_ConnaissancePopulation_EstDansZone" );
}


// =============================================================================
// INSTANCIATION
// =============================================================================

// -----------------------------------------------------------------------------
// Name: MIL_AutomateType::InstanciateAutomate
// Created: NLD 2004-08-11
// -----------------------------------------------------------------------------
MIL_Automate& MIL_AutomateType::InstanciateAutomate( uint nID, MIL_InputArchive& archive ) const
{
    return *new MIL_Automate( *this, nID, archive );
}
