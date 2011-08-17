// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#include "DEC_Model_ABC.h"
#include "Decision/DEC_PathFunctions.h"
#include "Decision/DEC_DIAFunctions.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "MIL_Singletons.h"
#include <directia/brain/Brain.h>
//#include "MT_Tools/MT_Logger.h"

// -----------------------------------------------------------------------------
// Name: DEC_Decision constructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template< class T >
DEC_Decision< T >::DEC_Decision( T& entity, unsigned int gcPause, unsigned int gcMult )
    : pEntity_ ( &entity )
    , gcPause_ ( gcPause)
    , gcMult_  ( gcMult )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision destructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template< class T >
DEC_Decision< T >::~DEC_Decision()
{
    // NOTHING
}

namespace DEC_DecisionImpl
{
    void RegisterCommonUserFunctions( directia::brain::Brain& brain , bool isMasalife );
    void RegisterMissionParameters( directia::brain::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const boost::shared_ptr< MIL_Mission_ABC > mission, bool isMasalife );
    bool CreateBrain( boost::shared_ptr< directia::brain::Brain >& pArchetypeBrain, boost::shared_ptr< directia::brain::Brain >& pBrain, const std::string& includePath, const std::string& brainFile, bool isMasalife, const std::string& type );
}

namespace directia
{    
    void UsedByDIA( DEC_Decision_ABC* );
    void ReleasedByDIA( DEC_Decision_ABC* );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::InitBrain
// Created: MGD 2010-01-27
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::InitBrain( const std::string& brainFile, const std::string& type, const std::string& includePath, const std::string& groupName, bool isMasalife )
{
    brainFile_ = brainFile;
    modelName_ = type;
    includePath_ = includePath;
    isMasalife_ = isMasalife;
    std::size_t lookHere = 0;
    std::size_t foundHere;
    while( ( foundHere = includePath_.find( "\\", lookHere ) ) != std::string::npos )
    {
        includePath_.replace( foundHere, 1, "/" );
        lookHere = foundHere + 1;
    }
    
    pRefs_.reset( 0 );//Must delete ScriptRef before call Brain destructor and destroy vm
    boost::shared_ptr< directia::brain::Brain > pArchetypeBrain;

    bool newBrain = DEC_DecisionImpl::CreateBrain( pArchetypeBrain, pBrain_, includePath_, brainFile, isMasalife_, type );

    if( newBrain )
    {
        DEC_DecisionImpl::RegisterCommonUserFunctions( *pArchetypeBrain, isMasalife_ );
        RegisterUserArchetypeFunctions( *pArchetypeBrain );
    }

    RegisterUserFunctions( *pBrain_ );

    //Enregistrement à la main de BreakForDebug
    (*pBrain_)[ "BreakForDebug" ] =
        boost::function< void( const std::string& ) >( boost::bind( &DEC_DIAFunctions::BreakForDebug, pEntity_->GetID() ,_1 ) ) ;

    RegisterSelf( *pBrain_, isMasalife_, groupName );

    pRefs_.reset( new ScriptRefs( *pBrain_) );

//    float size = (*pBrain_)[ "collectgarbage" ].Call<float>( "count" );
//    std::stringstream stream;
//    stream << "Memory: " << size;
//    MT_LOG_WARNING( stream.str(), 2, "whatever" );
    
    if ( newBrain )//Call GC only for a new archetype
    {
        pRefs_->collectgarbage_("setpause", gcPause_);
        pRefs_->collectgarbage_("setstepmul", gcMult_);
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetModel
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetModel( const DEC_Model_ABC& model )
{
    diaType_ = model.GetDIAType();
    InitBrain( model.GetScriptFile(), diaType_, model.GetIncludePath(), GetAutomate().GetName(), model.IsMasalife() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
template< class T >
const std::string& DEC_Decision< T >::GetDIAType() const
{
    return diaType_;
}   

// -----------------------------------------------------------------------------
// Name: DEC_Decision::UpdateDecision
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::UpdateDecision( float duration )
{
    try
    {
        pBrain_->SelectActions         ();
        pBrain_->TriggerSelectedActions( duration );
    }
    catch( std::runtime_error& e )
    {
        HandleUpdateDecisionError( &e );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GarbageCollect
// Created: LDC 2009-09-22
// -----------------------------------------------------------------------------
/*template< class T >
void DEC_Decision< T >::GarbageCollect()
{
    //pRefs_->collectgarbage_( pRefs_->step_ );
}*/

// -----------------------------------------------------------------------------
// Name: DEC_Decision::Reset
// Created: MGD 2010-01-27
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::Reset( std::string groupName )
{
    StopDefaultBehavior();
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::CleanStateAfterCrash
// Created: LDC 2009-04-07
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::CleanStateAfterCrash()
{
    assert( false ); // To allow debugging ...
    assert( pEntity_ );
    _clearfp();
//    DEC_Tools::DisplayDiaStack( GetCurrentInstance(), GetCurrentDebugInfo() ); // $$$$ LDC: Is there a way to dump lua state?
//    Reset();
    EndCleanStateAfterCrash();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::HandleUpdateDecisionError
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::HandleUpdateDecisionError( const std::exception* error )
{
    assert( pEntity_ );
    if( error )
        LogError( error );
    CleanStateAfterCrash();
    MIL_Report::PostEvent( *pEntity_, MIL_Report::eReport_MissionImpossible_ );
    pEntity_->GetOrderManager().CancelMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetBrain
// Created: LDC 2009-07-02
// -----------------------------------------------------------------------------
template< class T >
directia::brain::Brain& DEC_Decision< T >::GetBrain()
{
    return *pBrain_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::RemoveCallback
// Created: LDC 2009-07-02
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::RemoveCallback( unsigned int actionId )
{
    pRefs_->removeAction_( actionId );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::LogError
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::LogError( const std::exception* e ) const
{
    MT_LOG_ERROR_MSG( "Entity " << pEntity_->GetID() << "('" << pEntity_->GetName() << "') : Mission '" << pEntity_->GetOrderManager().GetMissionName() << "' impossible" );
    if( e )
        MT_LOG_ERROR_MSG( e->what() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StartDefaultBehavior
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::StartDefaultBehavior()
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

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StopDefaultBehavior
// Created: LDC 2009-03-02
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::StopDefaultBehavior()
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
template< class T >
void DEC_Decision< T >::ActivateOrder( const std::string& strBehavior, const boost::shared_ptr< MIL_Mission_ABC > mission )
{
    pMission_ = mission;
    // Register mission parameters in the brain...
    directia::tools::binders::ScriptRef refMission( *pBrain_ );
    refMission = pMission_;
    DEC_DecisionImpl::RegisterMissionParameters( *pBrain_, pRefs_->initTaskParameter_, refMission, pMission_, isMasalife_ );
    pRefs_->startEvent_( strBehavior, refMission );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StopMission
// Created: LDC 2009-04-07
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::StopMission( const std::string& strBehavior )
{
    try
    {
        pRefs_->stopEvents_.operator ()< const std::string& >( strBehavior );
        pMission_.reset();
    }
    catch( std::runtime_error& )
    {
        CleanStateAfterCrash();
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetMission
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetMission( boost::shared_ptr< MIL_Mission_ABC > pMission )
{
    pMission_ = pMission;
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetMission
// Created: LDC 2009-04-09
// -----------------------------------------------------------------------------
template< class T >
boost::shared_ptr< MIL_Mission_ABC > DEC_Decision< T >::GetMission()
{
    return pMission_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::CallbackKnowledge
// Created: LDC 2009-07-06
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::CallbackKnowledge( unsigned int actionId, boost::shared_ptr< DEC_Knowledge_Object > value )
{
    pRefs_->knowledgeCallbackAction_( actionId, value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::CallbackPerception
// Created: LDC 2009-07-21
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::CallbackPerception( int id )
{
    pRefs_->callbackPerception_( id );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetPion
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
MIL_AgentPion& DEC_Decision< T >::GetPion() const
{
    throw std::runtime_error( "GetPion cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetAutomate
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
MIL_Automate& DEC_Decision< T >::GetAutomate() const
{
    throw std::runtime_error( "GetAutomate cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetID
// Created: LDC 2011-05-05
// -----------------------------------------------------------------------------
template< class T >
unsigned int DEC_Decision< T >::GetID() const
{
    return pEntity_->GetID();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetKnowledgeGroup
// Created: LDC 2011-05-05
// -----------------------------------------------------------------------------
template< class T >
MIL_KnowledgeGroup& DEC_Decision<T>::GetKnowledgeGroup() const
{
    throw std::runtime_error( "GetKnowledgeGroup cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StartMissionBehavior
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > /*mission*/ )
{
    throw std::runtime_error( "StartMissionBehavior cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StopMissionBehavior
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::StopMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > /*mission*/ )
{
    throw std::runtime_error( "StopMissionBehavior cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatPhaseMission
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GeteEtatPhaseMission() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatPhaseMission
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SeteEtatPhaseMission( int /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatLima
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GeteEtatLima() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatLima
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SeteEtatLima( int /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatDec
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GeteEtatDec() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatDec
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SeteEtatDec( int /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatEchelon
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GeteEtatEchelon() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatEchelon
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SeteEtatEchelon( int /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetbOrdreDecrocher
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetbOrdreDecrocher() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbOrdreDecrocher
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetbOrdreDecrocher( bool /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetbOrdreTenirSurLR
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetbOrdreTenirSurLR() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbOrdreTenirSurLR
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetbOrdreTenirSurLR( bool /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetbOrdreTenir
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetbOrdreTenir() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbOrdreTenir
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetbOrdreTenir( bool /*value*/ )
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: std::vector< DEC_Decision_ABC* > DEC_Decision::GetPionsWithPC
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
std::vector< DEC_Decision_ABC* > DEC_Decision< T >::GetPionsWithPC()
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsNeutralized
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsNeutralized() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsMoving
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsMoving() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsContaminated
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsContaminated() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetPosition
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
const MT_Vector2D* DEC_Decision< T >::GetPosition() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetStateVariable
// Created: LDC 2009-07-15
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetStateVariable( const std::string& name, float value )
{
    pRefs_->setStateVariable_( name, value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetAmbianceMission
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetAmbianceMission( int value )
{
    SetVariable( "myself.ambianceMission_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetAppuieFreinage
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetAppuieFreinage( bool value )
{
    SetVariable( "myself.bAppuieFreinage_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetDemandeOrdreConduitePoursuivre(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetDemandeOrdreConduitePoursuivre()
{
    return GetScalarVariable<bool>( "myself.bDemandeOrdreConduitePoursuivre_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetEnCoursExtractionPersonnel(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetEnCoursExtractionPersonnel()
{
    return GetScalarVariable<bool>( "myself.bEnCoursExtractionPersonnel_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetEnExploitation(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetEnExploitation()
{
    return GetScalarVariable<bool>( "myself.bEnExploitation_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbEnExploitation
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetbEnExploitation( bool value )
{
    SetVariable( "myself.bEnExploitation_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetEnPhaseRavitaillement(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetEnPhaseRavitaillement()
{
    return GetScalarVariable<bool>( "myself.bEnPhaseRavitaillement_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetEnPhaseRavitaillement
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetEnPhaseRavitaillement( bool value )
{
    SetVariable( "myself.bEnPhaseRavitaillement_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetMiseEnOeuvre(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetMiseEnOeuvre()
{
    return GetScalarVariable<bool>( "myself.bMiseEnOeuvre_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetMiseEnOeuvre
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetMiseEnOeuvre( bool value )
{
    SetVariable( "myself.bMiseEnOeuvre_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetEtatFeu
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GetEtatFeu()
{
    return GetScalarVariable<int>( "myself.eEtatFeu_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetListeEnisTirAutorise(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > DEC_Decision< T >::GetListeEnisTirAutorise()
{
    return GetVariable< std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > >( "myself.listeEnisTirAutorise_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetListeEnisTirAutorise
// Created: LDC 2009-12-09
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetListeEnisTirAutorise( const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& list )
{
    SetVariable( "myself.listeEnisTirAutorise_", list );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetListePionsCoordination(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
std::vector<DEC_Decision_ABC*> DEC_Decision< T >::GetListePionsCoordination()
{
    return GetVariable< std::vector<DEC_Decision_ABC*> >( "myself.listePionsCoordination_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetObjMisEnCours(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
boost::shared_ptr< DEC_Knowledge_Object > DEC_Decision< T >::GetObjMisEnCours()
{
    return GetVariable< boost::shared_ptr< DEC_Knowledge_Object > >( "myself.objMisEnCours_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetObjMisEnCours
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetObjMisEnCours( boost::shared_ptr< DEC_Knowledge_Object > value )
{
    SetVariable( "myself.objMisEnCours_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetObjectifCourant(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
boost::shared_ptr< MT_Vector2D > DEC_Decision< T >::GetObjectifCourant()
{
    return GetVariable< boost::shared_ptr< MT_Vector2D > >( "myself.objectifCourant_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetPtSauvegarde
// Created: LDC 2011-08-16
// -----------------------------------------------------------------------------
template< class T >
MT_Vector2D* DEC_Decision< T >::GetPtSauvegarde()
{
    return GetVariable< MT_Vector2D* >( "myself.ptSauvegarde_" );
}
    
// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetPlotRavitaillementAssigne(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
boost::shared_ptr< DEC_Knowledge_Object > DEC_Decision< T >::GetPlotRavitaillementAssigne()
{
    return GetVariable< boost::shared_ptr< DEC_Knowledge_Object > >( "myself.plotRavitaillementAssigne_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetPlotRavitaillementAssigne
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetPlotRavitaillementAssigne( boost::shared_ptr< DEC_Knowledge_Object > value )
{
    SetVariable( "myself.plotRavitaillementAssigne_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetPorteeAction(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GetPorteeAction()
{
    return GetScalarVariable<int>( "myself.porteeAction_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetPorteeAmyself.rNiveauAlerteRavitaillement_ction
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetPorteeAction( int value )
{
    SetVariable( "myself.porteeAction_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetNiveauAlerteRavitaillement(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
float DEC_Decision< T >::GetNiveauAlerteRavitaillement()
{
    return GetScalarVariable<float>( "myself.rNiveauAlerteRavitaillement_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::ClearListeEnisTirAutorise
// Created: LDC 2010-04-27
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::ClearListeEnisTirAutorise()
{
    const std::vector< int > list;
    SetVariable( "myself.listeEnisTirAutorise_", list );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::ClearListePionsCoordination
// Created: LDC 2010-04-27
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::ClearListePionsCoordination()
{
    const std::vector< int > list;
    SetVariable( "myself.listePionsCoordination_", list );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::ClearPlotsRavitaillement
// Created: LDC 2010-04-27
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::ClearPlotsRavitaillement()
{
    const std::vector< int > list;
    SetVariable( "myself.plotsRavitaillement_", list );
}

// -----------------------------------------------------------------------------
// Name: std::vector<DEC_Knowledge_Object*> DEC_Decision::GetListeZonesTir
// Created: LDC 2011-08-16
// -----------------------------------------------------------------------------
template< class T >
std::vector<DEC_Knowledge_Object*> DEC_Decision< T >::GetListeZonesTir()
{
    return GetVariable< std::vector<DEC_Knowledge_Object*> >( "myself.listeZonesTir_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::ClearListeZonesTir
// Created: LDC 2011-08-16
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::ClearListeZonesTir()
{
    const std::vector< int > list;
    SetVariable( "myself.listeZonesTir_", list );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_Decision::GetLastPointOfPath
// Created: LDC 2009-11-04
// -----------------------------------------------------------------------------
template< class T >
boost::shared_ptr< MT_Vector2D > DEC_Decision< T >::GetLastPointOfPath( const double time, bool bBoundOnPath ) const
{
    return DEC_PathFunctions::ExtrapolatePosition( GetPion(), time, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetMunition
// Created: JCR 2010-11-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetMunition( const PHY_DotationCategory* value )
{
    SetVariable( "myself.munitions_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetMunition
// Created: JCR 2010-11-04
// -----------------------------------------------------------------------------
template< class T >
const PHY_DotationCategory* DEC_Decision< T >::GetMunition()
{
    return GetVariable< const PHY_DotationCategory* >( "myself.munitions_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetNbIt
// Created: JCR 2010-11-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetNbIt( int value )
{
    SetVariable( "myself.nbIT_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetNbIt
// Created: JCR 2010-11-04
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GetNbIt()
{
    return GetScalarVariable< int >( "myself.nbIT_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetTarget
// Created: JCR 2010-11-04
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetTarget( boost::shared_ptr< MT_Vector2D > value )
{
    SetVariable( "myself.cible_", value );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetTarget
// Created: JCR 2010-11-04
// -----------------------------------------------------------------------------
template< class T >
boost::shared_ptr< MT_Vector2D > DEC_Decision< T >::GetTarget()
{
    return GetVariable< boost::shared_ptr< MT_Vector2D > >( "myself.cible_" );
}

// -----------------------------------------------------------------------------
// Name: boost::shared_ptr< MT_Vector2D > DEC_Decision::ExtrapolatePosition
// Created: LDC 2009-11-04
// -----------------------------------------------------------------------------
template< class T >
boost::shared_ptr< MT_Vector2D > DEC_Decision< T >::ExtrapolatePosition( const double time, bool bBoundOnPath ) const
{
    return DEC_PathFunctions::ExtrapolatePosition( GetPion(), time, bBoundOnPath );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsPC
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsPC() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsTransported
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsTransported() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsFlying
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsFlying() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetMajorOperationalState
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
double DEC_Decision< T >::GetMajorOperationalState() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsAutomateEngaged
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsAutomateEngaged() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsDead
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsDead() const
{
    throw std::runtime_error( "Invalid call of this Decision class" );
}
