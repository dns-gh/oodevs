// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "DEC_Model_ABC.h"
#include "Decision/DEC_Decision_ABC.h"
#include "Decision/DEC_DataBase.h"
#include "Decision/Functions/DEC_PathFunctions.h"
#include "Entities/Orders/MIL_Report.h"
#include "MT_Tools/MT_CrashHandler.h"

// -----------------------------------------------------------------------------
// Name: DEC_Decision constructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template <class T>
DEC_Decision<T>::DEC_Decision( T& entity, DEC_DataBase& database )
: pEntity_( &entity )
, pMission_( 0 )
, database_( database )
{
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision destructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template <class T>
DEC_Decision<T>::~DEC_Decision()
{
    // NOTHING
}

namespace DEC_DecisionImpl
{
    void RegisterCommonUserFunctions( directia::Brain& brain, unsigned int id );
    void RegisterMissionParameters( directia::ScriptRef& knowledgeCreateFunction, const directia::ScriptRef& refMission, MIL_Mission_ABC& mission );
}

namespace directia
{    
    void UsedByDIA( DEC_Decision_ABC* );
    void ReleasedByDIA( DEC_Decision_ABC* );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::InitBrain
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::InitBrain( const std::string& brainFile, const std::string& /*type*/, const std::string& includePath )
{
    brainFile_ = brainFile;
    includePath_ = includePath;
    pRefs_.reset( 0 );//Must delete ScriptRef before call Brain destructor and destroy vm
    pBrain_.reset( new directia::Brain( includePath ) );
    pBrain_->GetScriptFunction( "include" )( ( brainFile ),(includePath) );
    database_.InitKnowledges( *pBrain_ );//@TODO MGD Find a better way to merge dia4/dia5

    

    pRefs_.reset( new ScriptRefs( *pBrain_) );
    RegisterUserFunctions( *pBrain_ );
    DEC_DecisionImpl::RegisterCommonUserFunctions( *pBrain_, pEntity_->GetID() );
    RegisterSelf( *pBrain_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetModel
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetModel( const DEC_Model_ABC& model )
{
    InitBrain( model.GetScriptFile(), model.GetName(), model.GetIncludePath() );
    diaType_ = model.GetDIAType();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
template <class T>
const std::string& DEC_Decision<T>::GetDIAType() const
{
    return diaType_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::UpdateDecision
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::UpdateDecision( float duration )
{
    __try
    {
        UpdateMeKnowledge( *pBrain_ );
        pBrain_->SelectActions         ();
        pBrain_->TriggerSelectedActions( duration );
    }
    __except( MT_CrashHandler::ExecuteHandler( GetExceptionInformation() ) )
    {
        HandleUpdateDecisionError();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GarbageCollect
// Created: LDC 2009-09-22
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::GarbageCollect()
{
    pRefs_->collectgarbage_( pRefs_->step_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::Reset
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::Reset()
{
    InitBrain( brainFile_, "name", includePath_ );
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::CleanStateAfterCrash
// Created: LDC 2009-04-07
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::CleanStateAfterCrash()
{    
    assert( false ); // To allow debugging ...
    assert( pEntity_ );   
    _clearfp();

//    DEC_Tools::DisplayDiaStack( GetCurrentInstance(), GetCurrentDebugInfo() ); // $$$$ LDC: Is there a way to dump lua state?

    EndCleanStateAfterCrash();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::HandleUpdateDecisionError
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::HandleUpdateDecisionError()
{
    assert( pEntity_ );
    LogCrash();
    CleanStateAfterCrash();
    MIL_Report::PostEvent( *pEntity_, MIL_Report::eReport_MissionImpossible_ );
    pEntity_->GetOrderManager().ReplaceMission();               
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetBrain
// Created: LDC 2009-07-02
// -----------------------------------------------------------------------------
template <class T>
directia::Brain& DEC_Decision<T>::GetBrain()
{
    return *pBrain_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RemoveCallback
// Created: LDC 2009-07-02
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::RemoveCallback( PHY_Action_ABC* pAction )
{
    pBrain_->GetScriptFunction( "RemoveAction" )( pAction );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::LogCrash
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::LogCrash()
{
    MT_LOG_ERROR_MSG( "Entity " << pEntity_->GetID() << "('" << pEntity_->GetName() << "') : Mission '" << pEntity_->GetOrderManager().GetMissionName() << "' impossible" );
}

// =============================================================================
// DEFAULT BEHAVIOR
// =============================================================================

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StartDefaultBehavior
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::StartDefaultBehavior()
{
    try
    {
        pRefs_->startEvent_( std::string( "BEH_Defaut" ) );
    }
    catch( std::runtime_error& )
    {
        // Ignore error if BEH_Defaut doesn't exist
    }
}

// Name: DEC_Decision::StopDefaultBehavior
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::StopDefaultBehavior()
{
    try
    {
        if( pRefs_.get() )
            pRefs_->stopEvents_( std::string( "BEH_Defaut" ) );
    }
    catch( std::runtime_error& )
    {
        // Ignore error if BEH_Defaut doesn't exist
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::ActivateOrder
// Created: LDC 2009-04-07
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::ActivateOrder( const std::string& strBehavior, MIL_Mission_ABC& mission )
{
    pMission_ = &mission;
    // Register mission parameters in the brain...
    directia::ScriptRef refMission = pBrain_->RegisterObject( pMission_ );
    DEC_DecisionImpl::RegisterMissionParameters( pBrain_->GetScriptFunction( "InitTaskParameter" ), refMission, *pMission_ );
    pRefs_->startEvent_( strBehavior, pMission_ );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StopMission
// Created: LDC 2009-04-07
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::StopMission( const std::string& strBehavior )
{
    __try
    {
        pRefs_->stopEvents_.operator ()< const std::string& >( strBehavior );
        pMission_ = 0;
    }
    __except( MT_CrashHandler::ExecuteHandler( GetExceptionInformation() ) )
    {
        CleanStateAfterCrash();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetMission
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetMission( MIL_Mission_ABC* pMission )
{
    pMission_ = pMission;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetMission
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
template <class T>
MIL_Mission_ABC* DEC_Decision<T>::GetMission()
{
    return pMission_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::CallbackKnowledge
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::CallbackKnowledge( PHY_Action_ABC* pAction, boost::shared_ptr< DEC_Knowledge_Object > value )
{
    GetBrain().GetScriptFunction( "KnowledgeCallbackAction" )( pAction, value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::CallbackPerception
// Created: LDC 2009-07-21
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::CallbackPerception( int id )
{
    GetBrain().GetScriptFunction( "CallbackPerception" )( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetPion
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
MIL_AgentPion& DEC_Decision<T>::GetPion() const
{
    throw std::runtime_error( "GetPion cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetAutomate
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
MIL_Automate& DEC_Decision<T>::GetAutomate() const
{
    throw std::runtime_error( "GetAutomate cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StartMissionBehavior
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::StartMissionBehavior( MIL_Mission_ABC& /*mission*/ )
{
    throw std::runtime_error( "StartMissionBehavior cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StopMissionBehavior
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::StopMissionBehavior( MIL_Mission_ABC& /*mission*/ )
{
    throw std::runtime_error( "StopMissionBehavior cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatPhaseMission
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
int DEC_Decision<T>::GeteEtatPhaseMission() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatPhaseMission
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SeteEtatPhaseMission( int /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatLima
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
int DEC_Decision<T>::GeteEtatLima() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatLima
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SeteEtatLima( int /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatDec
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
int DEC_Decision<T>::GeteEtatDec() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatDec
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SeteEtatDec( int /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatEchelon
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
int DEC_Decision<T>::GeteEtatEchelon() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatEchelon
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SeteEtatEchelon( int /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetbOrdreDecrocher
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::GetbOrdreDecrocher() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbOrdreDecrocher
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetbOrdreDecrocher( bool /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetbOrdreTenirSurLR
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::GetbOrdreTenirSurLR() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbOrdreTenirSurLR
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetbOrdreTenirSurLR( bool /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetbOrdreTenir
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::GetbOrdreTenir() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbOrdreTenir
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetbOrdreTenir( bool /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: std::vector< DEC_Decision_ABC* > DEC_Decision::GetPionsWithPC
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
std::vector< DEC_Decision_ABC* > DEC_Decision<T>::GetPionsWithPC()
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsNeutralized
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::IsNeutralized() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsMoving
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::IsMoving() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsContaminated
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::IsContaminated() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetPosition
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template <class T>
const MT_Vector2D* DEC_Decision<T>::GetPosition() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetStateVariable
// Created: LDC 2009-07-15
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetStateVariable( const std::string& name, float value )
{
    pRefs_->setStateVariable_( name, value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetAmbianceMission
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetAmbianceMission( int value )
{
    SetVariable( "myself.ambianceMission_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetAppuieFreinage
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetAppuieFreinage( bool value )
{
    SetVariable( "myself.bAppuieFreinage_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetDemandeOrdreConduitePoursuivre(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::GetDemandeOrdreConduitePoursuivre()
{
    return GetVariable<bool>( "myself.bDemandeOrdreConduitePoursuivre_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetEnCoursExtractionPersonnel(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::GetEnCoursExtractionPersonnel()
{
    return GetVariable<bool>( "myself.bEnCoursExtractionPersonnel_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetEnExploitation(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::GetEnExploitation()
{
    return GetVariable<bool>( "myself.bEnExploitation_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbEnExploitation
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetbEnExploitation( bool value )
{
    SetVariable( "myself.bEnExploitation_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetEnPhaseRavitaillement(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::GetEnPhaseRavitaillement()
{
    return GetVariable<bool>( "myself.bEnPhaseRavitaillement_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetEnPhaseRavitaillement
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetEnPhaseRavitaillement( bool value )
{
    SetVariable( "myself.bEnPhaseRavitaillement_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetMiseEnOeuvre(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
bool DEC_Decision<T>::GetMiseEnOeuvre()
{
    return GetVariable<bool>( "myself.bMiseEnOeuvre_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetMiseEnOeuvre
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetMiseEnOeuvre( bool value )
{
    SetVariable( "myself.bMiseEnOeuvre_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetEtatFeu
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
int DEC_Decision<T>::GetEtatFeu()
{
    return GetVariable<int>( "myself.eEtatFeu_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetListeEnisTirAutorise(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > DEC_Decision<T>::GetListeEnisTirAutorise()
{
    return GetVariable< std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > >( "myself.listeEnisTirAutorise_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetListeEnisTirAutorise
// Created: LDC 2009-12-09
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetListeEnisTirAutorise( const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& list )
{
    SetVariable( "myself.listeEnisTirAutorise_", list );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetListePionsCoordination(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
std::vector<DEC_Decision_ABC*> DEC_Decision<T>::GetListePionsCoordination()
{
    return GetVariable< std::vector<DEC_Decision_ABC*> >( "myself.listePionsCoordination_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetObjMisEnCours(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
int DEC_Decision<T>::GetObjMisEnCours()
{
    return GetVariable<int>( "myself.objMisEnCours_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetObjMisEnCours
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetObjMisEnCours( int value )
{
    SetVariable( "myself.objMisEnCours_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetObjectifCourant(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
boost::shared_ptr< MT_Vector2D > DEC_Decision<T>::GetObjectifCourant()
{
    return GetVariable< boost::shared_ptr< MT_Vector2D > >( "myself.objectifCourant_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetPlotRavitaillementAssigne(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
int DEC_Decision<T>::GetPlotRavitaillementAssigne()
{
    return GetVariable<int>( "myself.plotRavitaillementAssigne_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetPlotRavitaillementAssigne
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetPlotRavitaillementAssigne( int value )
{
    SetVariable( "myself.plotRavitaillementAssigne_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetPorteeAction(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
int DEC_Decision<T>::GetPorteeAction()
{
    return GetVariable<int>( "myself.porteeAction_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetPorteeAmyself.rNiveauAlerteRavitaillement_ction
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
void DEC_Decision<T>::SetPorteeAction( int value )
{
    SetVariable( "myself.porteeAction_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetNiveauAlerteRavitaillement(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template <class T>
float DEC_Decision<T>::GetNiveauAlerteRavitaillement()
{
    return GetVariable<float>( "myself.rNiveauAlerteRavitaillement_" );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_Decision::GetLastPointOfPath
// Created: LDC 2009-11-04
// -----------------------------------------------------------------------------
template <class T>
boost::shared_ptr< MT_Vector2D > DEC_Decision<T>::GetLastPointOfPath( const MT_Float time, bool bBoundOnPath ) const
{
    return DEC_PathFunctions::ExtrapolatePosition( GetPion(), time, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_Decision::ExtrapolatePosition
// Created: LDC 2009-11-04
// -----------------------------------------------------------------------------
template <class T>
boost::shared_ptr< MT_Vector2D > DEC_Decision<T>::ExtrapolatePosition( const MT_Float time, bool bBoundOnPath ) const
{
    return DEC_PathFunctions::ExtrapolatePosition( GetPion(), time, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsPC
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision<T>::IsPC() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsTransported
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision<T>::IsTransported() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsFlying
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision<T>::IsFlying() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetMajorOperationalState
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
MT_Float DEC_Decision<T>::GetMajorOperationalState() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsAutomateEngaged
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision<T>::IsAutomateEngaged() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsDead
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision<T>::IsDead() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::WearNbcProtectionSuit
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision<T>::WearNbcProtectionSuit() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RemoveNbcProtectionSuit
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision<T>::RemoveNbcProtectionSuit() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

