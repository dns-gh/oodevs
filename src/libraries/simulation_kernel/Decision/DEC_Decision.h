// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Decision_h_
#define __DEC_Decision_h_

#include "MIL.h"
#include "Brain.h"
#include "DEC_Decision_ABC.h"
#include "DEC_DIAFunctions.h"
#include "DEC_Model_ABC.h"
#include "DEC_PathFunctions.h"
#include "Entities/MIL_EntityManager.h"
#include "Entities/Orders/MIL_Mission_ABC.h"
#include "Entities/Orders/MIL_Report.h"
#include "MT_Tools/MT_Logger.h"
#include <tools/Path.h>
#include <boost/scoped_ptr.hpp>

class DEC_Knowledge_Object;
class DEC_Model_ABC;
class MIL_Entity_ABC;
class MIL_Mission_ABC;

namespace sword
{
    class DEC_Logger;
}

// =============================================================================
/** @class  DEC_Decision
    @brief  DEC_Decision
    Template T must be a subclass of MIL_Entity_ABC
*/
// Created: LDC 2009-02-27
// =============================================================================
template <class T >
class DEC_Decision : public DEC_Decision_ABC
{
public:
    //! @name Constructor
    //@{
             DEC_Decision( T& entity, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger );
    virtual ~DEC_Decision();
    //@}

    //! @name Operations
    //@{
    void SetModel( const DEC_Model_ABC& model );
    const DEC_Model_ABC& GetModel() const;
    virtual void UpdateDecision( float duration );
    virtual void Reset( std::string groupName = "" );
    virtual void Reload( bool doInitBrain );
    virtual void IncDIARef();
    virtual void DecDIARef();
    virtual bool IsUsedByDIA() const;
    virtual void ActivateBrainDebug( bool activate );
    void DeleteBrain();

    virtual void SetMission( boost::shared_ptr< MIL_Mission_ABC > pMission );
    virtual boost::shared_ptr< MIL_Mission_ABC > GetMission();
    virtual void RemoveCallback( unsigned int actionId );
    virtual void CallbackKnowledge( unsigned int actionId, boost::shared_ptr< DEC_Knowledge_Object > value );
    virtual void CallbackPerception( int id );
    virtual const std::string& GetDIAType() const;
    virtual MIL_AgentPion& GetPion() const;
    virtual MIL_Automate& GetAutomate() const;
    virtual unsigned int GetID() const;
    virtual boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const;

    virtual void GarbageCollect();

    virtual void StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission );
    virtual void StopMissionBehavior ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    virtual bool IsFragOrderAvailableForMission( const MIL_MissionType_ABC& missionType, const MIL_FragOrderType& fragOrderType ) const;
    virtual bool IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const;

    virtual int  GeteEtatPhaseMission() const;
    virtual void SeteEtatPhaseMission( int value );
    virtual int  GeteEtatLima() const;
    virtual void SeteEtatLima( int value );
    virtual int  GeteEtatDec() const;
    virtual void SeteEtatDec( int value );
    virtual int  GeteEtatEchelon() const;
    virtual void SeteEtatEchelon( int value );
    virtual bool GetbOrdreDecrocher() const;
    virtual void SetbOrdreDecrocher( bool value );
    virtual bool GetbOrdreTenirSurLR() const;
    virtual void SetbOrdreTenirSurLR( bool value );
    virtual bool GetbOrdreTenir() const;
    virtual void SetbOrdreTenir( bool value );
    virtual void SetAmbianceMission( int );
    virtual void SetAppuieFreinage( bool );
    virtual bool GetDemandeOrdreConduitePoursuivre();
    virtual bool GetEnCoursExtractionPersonnel();
    virtual bool GetEnExploitation();
    virtual void SetbEnExploitation( bool );
    virtual bool GetEnPhaseRavitaillement();
    virtual void SetEnPhaseRavitaillement( bool );
    virtual bool GetMiseEnOeuvre();
    virtual void SetMiseEnOeuvre( bool );
    virtual bool GetSuppliesLoaded();
    virtual void SetSuppliesLoaded( bool );
    virtual int GetEtatFeu();
    virtual std::vector< boost::shared_ptr< DEC_Knowledge_Agent > > GetListeEnisTirAutorise();
    virtual void SetListeEnisTirAutorise( const std::vector< boost::shared_ptr< DEC_Knowledge_Agent > >& );
    virtual std::vector<DEC_Decision_ABC*> GetListePionsCoordination();
    virtual boost::shared_ptr< DEC_Knowledge_Object > GetObjMisEnCours();
    virtual void SetObjMisEnCours( boost::shared_ptr< DEC_Knowledge_Object > );
    virtual boost::shared_ptr< MT_Vector2D > GetObjectifCourant();
    virtual MT_Vector2D* GetPtSauvegarde();
    virtual boost::shared_ptr< DEC_Knowledge_Object > GetPlotRavitaillementAssigne();
    virtual void SetPlotRavitaillementAssigne( boost::shared_ptr< DEC_Knowledge_Object > );
    virtual int GetPorteeAction();
    virtual void SetPorteeAction( int );
    virtual float GetNiveauAlerteRavitaillement();
    virtual boost::shared_ptr< MT_Vector2D > GetLastPointOfPath( const double time, bool bBoundOnPath ) const;
    virtual boost::shared_ptr< MT_Vector2D > ExtrapolatePosition( const double time, bool bBoundOnPath ) const;

    virtual void SetMunition( const PHY_DotationCategory* );
    virtual const PHY_DotationCategory* GetMunition();
    virtual void SetNbIt( int value );
    virtual int GetNbIt();
    virtual void SetTarget( boost::shared_ptr< MT_Vector2D > );
    virtual boost::shared_ptr< MT_Vector2D > GetTarget();

    virtual std::vector<DEC_Knowledge_Object*> GetListeZonesTir();
    virtual void ClearListeZonesTir();
    virtual void ClearListeEnisTirAutorise();
    virtual void ClearListePionsCoordination();
    virtual void ClearPlotsRavitaillement();

    virtual std::vector< DEC_Decision_ABC* > GetPionsWithPC();
    virtual std::vector< DEC_Decision_ABC* > GetCommunicationPionsWithPC();
    virtual bool IsNeutralized() const;
    virtual bool IsMoving() const;
    virtual bool IsContaminated() const;
    virtual bool IsDecontaminationFinished() const;
    virtual bool IsJammed() const;
    virtual bool IsInReceptionBlackout() const;
    virtual bool IsInEmissionBlackout() const;
    virtual const MT_Vector2D* GetPosition() const;
    virtual void SetStateVariable( const std::string& name, float value );
    virtual bool IsPC() const;
    virtual bool IsTransported() const;
    virtual bool IsFlying() const;
    virtual double GetOperationalState() const;
    virtual double GetMajorOperationalState() const;
    virtual bool IsAutomateEngaged() const;
    virtual bool IsDead() const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive& ar, const unsigned int )
    {
        ar & boost::serialization::base_object< DEC_Decision_ABC >( *this );
    }
    //@}

protected:
    //! @name Helpers
    //@{
    void InitBrain( const tools::Path& brainFile, const std::string& type,
                    const tools::Path& includePath, const std::string& groupName,
                    bool isMasalife, bool reload, const tools::Path& integrationDir );
    void CleanStateAfterCrash     ();

    void StartDefaultBehavior     ();
    void StopDefaultBehavior      ();

    void ActivateOrder( const std::string& strBehavior, const boost::shared_ptr< MIL_Mission_ABC > mission );
    void StopMission( const std::string& strBehavior );

    virtual std::string GetGroupName() = 0;
    virtual void EndCleanStateAfterCrash  () = 0;
    virtual void RegisterUserFunctions( sword::Brain& brain ) = 0;
    virtual void RegisterUserArchetypeFunctions( sword::Brain& brain ) = 0;
    void RegisterReportFunctions( sword::Brain& brain );

    template< typename Function >
    void RegisterFunction( const char* const name, const Function& function )
    {
        pBrain_->RegisterFunction( name, function );
    }
    template< typename MethodType >
    void RegisterMethod( const char* name, MethodType function )
    {
        pBrain_->RegisterMethod( name, function );
    }

    virtual void PostStartMission() = 0;
    virtual void PostStopMission() = 0;
    //@}

private://! @name Helpers
    //@{
    void HandleUpdateDecisionError( const std::exception* error = 0 );
    /* virtual */ void LogError   ( const std::exception* error = 0 ) const;

    virtual sword::Brain* GetBrain();

    virtual void RegisterSelf( sword::Brain& brain ) = 0;
    virtual void RegisterSpecific( sword::Brain& brain, bool isMasalife, const std::string& groupName ) = 0;
    //@}

protected:
    //!@name Data
    //@{
    T*                              pEntity_;
    boost::shared_ptr< MIL_Mission_ABC > pMission_;
    unsigned int                    gcPause_;
    unsigned int                    gcMult_;
    unsigned int                    nDIARef_;
    const DEC_Model_ABC*            model_;
    bool                            brainDebug_;
    sword::DEC_Logger*              logger_;
    //@}

private:
    //!@name Data
    //@{
    boost::shared_ptr< sword::Brain >  pBrain_;
    std::auto_ptr< struct ScriptRefs > pRefs_;
    bool                               isMasalife_;
    //@}
};

struct ScriptRefs
{
public:
    ScriptRefs( sword::Brain& brain );

    directia::tools::binders::ScriptRef startEvent_;
    directia::tools::binders::ScriptRef stopEvents_;
    directia::tools::binders::ScriptRef setStateVariable_;
    directia::tools::binders::ScriptRef collectgarbage_;
    directia::tools::binders::ScriptRef step_;
    directia::tools::binders::ScriptRef callbackPerception_;
    directia::tools::binders::ScriptRef knowledgeCallbackAction_;
    directia::tools::binders::ScriptRef removeAction_;
    directia::tools::binders::ScriptRef initTaskParameter_;
    directia::tools::binders::ScriptRef activateBrainDebug_;

private:
    ScriptRefs();
    ScriptRefs( const ScriptRefs& );
    ScriptRefs& operator=( const ScriptRefs& );
};

// -----------------------------------------------------------------------------
// Name: DEC_Decision constructor
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template< class T >
DEC_Decision< T >::DEC_Decision( T& entity, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger )
    : pEntity_   ( &entity )
    , gcPause_   ( gcPause)
    , gcMult_    ( gcMult )
    , nDIARef_   ( 0 )
    , model_     ( 0 )
    , brainDebug_( false )
    , logger_    ( logger )
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
    void RegisterCommonUserFunctions( sword::Brain& brain , bool isMasalife );
    void RegisterMissionParameters( sword::Brain& brain, directia::tools::binders::ScriptRef& knowledgeCreateFunction, const directia::tools::binders::ScriptRef& refMission, const boost::shared_ptr< MIL_Mission_ABC > mission, bool isMasalife );
    bool CreateBrain( boost::shared_ptr< sword::Brain >& pArchetypeBrain,
                      boost::shared_ptr< sword::Brain >& pBrain, const tools::Path& includePath,
                      const tools::Path& brainFile, bool isMasalife, const std::string& type,
                      bool reload, const tools::Path& integrationDir, sword::DEC_Logger* logger );
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
void DEC_Decision< T >::InitBrain( const tools::Path& brainFile, const std::string& type,
                                   const tools::Path& includePath,const std::string& groupName,
                                   bool isMasalife, bool reload, const tools::Path& integrationDir )
{
    tools::Path realIncludePath = includePath;
    realIncludePath.Normalize();
    isMasalife_ = isMasalife;
    pRefs_.reset( 0 );//Must delete ScriptRef before call Brain destructor and destroy vm
    boost::shared_ptr< sword::Brain > pArchetypeBrain;

    bool newBrain = DEC_DecisionImpl::CreateBrain( pArchetypeBrain, pBrain_, realIncludePath, brainFile, isMasalife_, type, reload, integrationDir, logger_ );

    RegisterSelf( *pBrain_ );

    if( newBrain )
    {
        DEC_DecisionImpl::RegisterCommonUserFunctions( *pArchetypeBrain, isMasalife_ );
        RegisterUserArchetypeFunctions( *pArchetypeBrain );
    }

    RegisterUserFunctions( *pBrain_ );

    //Enregistrement à la main de BreakForDebug
    pBrain_->RegisterFunction( "BreakForDebug",
        boost::function< void( const std::string& ) >( boost::bind( &DEC_DIAFunctions::BreakForDebug, pEntity_->GetID() ,_1 ) ) );

    RegisterSpecific( *pBrain_, isMasalife_, groupName );

    pRefs_.reset( new ScriptRefs( *pBrain_ ) );

//    float size = (*pBrain_)[ "collectgarbage" ].Call<float>( "count" );
//    std::stringstream stream;
//    stream << "Memory: " << size;
//    MT_LOG_WARNING( stream.str(), 2, "whatever" );

    if ( newBrain )//Call GC only for a new archetype
    {
        pRefs_->collectgarbage_( "setpause", gcPause_ );
        pRefs_->collectgarbage_( "setstepmul", gcMult_ );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::DeleteBrain
// Created: JSR 2013-02-01
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::DeleteBrain()
{
    pBrain_.reset();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetModel
// Created: LDC 2009-04-08
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetModel( const DEC_Model_ABC& model )
{
    model_ = &model;
    InitBrain( model.GetScriptFile(), model.GetDIAType(), model.GetIncludePath(),
               GetAutomate().GetName(), model.IsMasalife(), false, model.GetIntegrationDir() );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetDIAType
// Created: LDC 2009-07-09
// -----------------------------------------------------------------------------
template< class T >
const std::string& DEC_Decision< T >::GetDIAType() const
{
    return model_->GetDIAType();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::UpdateDecision
// Created: LDC 2009-02-27
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::UpdateDecision( float duration )
{
    if( pBrain_.get() == 0 )
        return;
    try
    {
        pBrain_->SelectActions         ();
        pBrain_->TriggerSelectedActions( duration );
    }
    catch( const std::exception& e )
    {
        HandleUpdateDecisionError( &e );
    }
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GarbageCollect
// Created: LDC 2009-09-22
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::GarbageCollect()
{
    pRefs_->collectgarbage_( pRefs_->step_ );
    //pRefs_->collectgarbage_( 3 );
}

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
    assert( pEntity_ );
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
    if( !pEntity_ )
    {
        MT_LOG_ERROR_MSG( "Update decision error in nil entity" );
        return;
    }
    if( error )
        LogError( error );
    CleanStateAfterCrash();
    MIL_Report::PostEvent( *pEntity_, report::eRC_MissionImpossible );
    pEntity_->GetOrderManager().CancelMission();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetBrain
// Created: LDC 2009-07-02
// -----------------------------------------------------------------------------
template< class T >
sword::Brain* DEC_Decision< T >::GetBrain()
{
    return pBrain_.get();
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
        MT_LOG_ERROR_MSG( tools::GetExceptionMsg( *e ) );
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
    catch( const std::exception& )
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
    catch( const std::exception& )
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
    if( pBrain_.get() == 0 )
        return;
    pMission_ = mission;
    // Register mission parameters in the brain...
    directia::tools::binders::ScriptRef refMission = pBrain_->GetScriptRef();
    refMission = pMission_;
    DEC_DecisionImpl::RegisterMissionParameters( *pBrain_, pRefs_->initTaskParameter_, refMission, pMission_, isMasalife_ );
    pRefs_->startEvent_( strBehavior, refMission );
    PostStartMission();
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
        pRefs_->stopEvents_( strBehavior );
        pMission_.reset();
        PostStopMission();
    }
    catch( const std::exception& )
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
    throw MASA_EXCEPTION( "GetPion cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetAutomate
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
MIL_Automate& DEC_Decision< T >::GetAutomate() const
{
    throw MASA_EXCEPTION( "GetAutomate cannot be called for this Decision class" );
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
boost::shared_ptr< MIL_KnowledgeGroup > DEC_Decision<T>::GetKnowledgeGroup() const
{
    throw MASA_EXCEPTION( "GetKnowledgeGroup cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StartMissionBehavior
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > /*mission*/ )
{
    throw MASA_EXCEPTION( "StartMissionBehavior cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::StopMissionBehavior
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::StopMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > /*mission*/ )
{
    throw MASA_EXCEPTION( "StopMissionBehavior cannot be called for this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsFragOrderAvailableForMission
// Created: LDC 2012-09-06
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsFragOrderAvailableForMission( const MIL_MissionType_ABC& missionType, const MIL_FragOrderType& fragOrderType ) const
{
    return model_->IsFragOrderAvailableForMission( missionType, fragOrderType );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsFragOrderAvailable
// Created: LDC 2012-09-06
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsFragOrderAvailable( const MIL_FragOrderType& fragOrderType ) const
{
    return model_->IsFragOrderAvailable( fragOrderType );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatPhaseMission
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GeteEtatPhaseMission() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatPhaseMission
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SeteEtatPhaseMission( int /*value*/ )
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatLima
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GeteEtatLima() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatLima
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SeteEtatLima( int /*value*/ )
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatDec
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GeteEtatDec() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatDec
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SeteEtatDec( int /*value*/ )
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GeteEtatEchelon
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
int DEC_Decision< T >::GeteEtatEchelon() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SeteEtatEchelon
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SeteEtatEchelon( int /*value*/ )
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetbOrdreDecrocher
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetbOrdreDecrocher() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbOrdreDecrocher
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetbOrdreDecrocher( bool /*value*/ )
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetbOrdreTenirSurLR
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetbOrdreTenirSurLR() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbOrdreTenirSurLR
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetbOrdreTenirSurLR( bool /*value*/ )
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetbOrdreTenir
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetbOrdreTenir() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetbOrdreTenir
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetbOrdreTenir( bool /*value*/ )
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: std::vector< DEC_Decision_ABC* > DEC_Decision::GetPionsWithPC
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
std::vector< DEC_Decision_ABC* > DEC_Decision< T >::GetPionsWithPC()
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: std::vector< DEC_Decision_ABC* > DEC_Decision::GetCommunicationPionsWithPC
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
std::vector< DEC_Decision_ABC* > DEC_Decision< T >::GetCommunicationPionsWithPC()
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsNeutralized
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsNeutralized() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsMoving
// Created: LDC 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsMoving() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsContaminated
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsContaminated() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsDecontaminationFinished
// Created: LGY 2013-01-07
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsDecontaminationFinished() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsJammed
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsJammed() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsInReceptionBlackout
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsInReceptionBlackout() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsInEmissionBlackout
// Created: MMC 2012-07-03
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsInEmissionBlackout() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetPosition
// Created: LDC 2009-07-13
// -----------------------------------------------------------------------------
template< class T >
const MT_Vector2D* DEC_Decision< T >::GetPosition() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
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
    return GetVariable<bool>( "myself.bDemandeOrdreConduitePoursuivre_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetEnCoursExtractionPersonnel(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetEnCoursExtractionPersonnel()
{
    return GetVariable<bool>( "myself.bEnCoursExtractionPersonnel_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetEnExploitation(
// Created: LDC 2009-08-04
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetEnExploitation()
{
    return GetVariable<bool>( "myself.bEnExploitation_" );
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
    return GetVariable<bool>( "myself.bEnPhaseRavitaillement_" );
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
    return GetVariable<bool>( "myself.bMiseEnOeuvre_" );
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
    return GetVariable<int>( "myself.eEtatFeu_" );
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
// Name: DEC_Decision::GetSuppliesLoaded(
// Created: NMI 2013-06-07
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::GetSuppliesLoaded()
{
    return GetVariable<bool>( "myself.bSuppliesLoaded_" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::SetSuppliesLoaded
// Created: NMI 2013-06-07
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::SetSuppliesLoaded( bool value )
{
    SetVariable( "myself.bSuppliesLoaded_", value );
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
    return GetVariable<int>( "myself.porteeAction_" );
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
    return GetVariable<float>( "myself.rNiveauAlerteRavitaillement_" );
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
    return GetVariable< int >( "myself.nbIT_" );
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
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsTransported
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsTransported() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsFlying
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsFlying() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetOperationalState
// Created: NMI 2013-06-12
// -----------------------------------------------------------------------------
template< class T >
double DEC_Decision< T >::GetOperationalState() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetMajorOperationalState
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
double DEC_Decision< T >::GetMajorOperationalState() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsAutomateEngaged
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsAutomateEngaged() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsDead
// Created: SBO 2009-07-29
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsDead() const
{
    throw MASA_EXCEPTION( "Invalid call of this Decision class" );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::Reload
// Created: LDC 2011-08-18
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::Reload( bool doInitBrain )
{
    if( doInitBrain )
        InitBrain( model_->GetScriptFile(), model_->GetDIAType(), model_->GetIncludePath(), GetGroupName(), model_->IsMasalife(), true, model_->GetIntegrationDir() );
    StartDefaultBehavior();
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IncDIARef
// Created: JSR 2013-02-01
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::IncDIARef()
{
    ++nDIARef_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::DecDIARef
// Created: JSR 2013-02-01
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::DecDIARef()
{
    assert( nDIARef_ > 0 );
    --nDIARef_;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::IsUsedByDIA
// Created: JSR 2013-02-01
// -----------------------------------------------------------------------------
template< class T >
bool DEC_Decision< T >::IsUsedByDIA() const
{
    return nDIARef_ > 0;
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::ActivateBrainDebug
// Created: SLI 2013-06-18
// -----------------------------------------------------------------------------
template< class T >
void DEC_Decision< T >::ActivateBrainDebug( bool activate )
{
    brainDebug_ = activate;
    pRefs_->activateBrainDebug_( activate );
}

// -----------------------------------------------------------------------------
// Name: DEC_Decision::GetModel
// Created: AHC 2012-01-27
// -----------------------------------------------------------------------------
template< class T >
const DEC_Model_ABC& DEC_Decision< T >::GetModel() const
{
    return *model_;
}

#endif // __DEC_Decision_h_
