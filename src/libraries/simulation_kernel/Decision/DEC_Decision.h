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
#include "Decision/DEC_Decision_ABC.h"
#include "MT_Tools/MT_Logger.h"

class DEC_Knowledge_Object;
class DEC_Model_ABC;
class MIL_Entity_ABC;
class MIL_Mission_ABC;

// =============================================================================
/** @class  DEC_Decision
    @brief  DEC_Decision
    Template T must be a subcless of MIL_Entity_ABC
*/
// Created: LDC 2009-02-27
// =============================================================================
template <class T >
class DEC_Decision : public DEC_Decision_ABC
{
public:
    //! @name Constructor
    //@{
             DEC_Decision( T& entity, unsigned int gcPause, unsigned int gcMult );
    virtual ~DEC_Decision();
    //@}

    //! @name Operations
    //@{
    void SetModel( const DEC_Model_ABC& model );
    virtual void UpdateDecision( float duration );
    virtual void Reset( std::string groupName = "" );
    virtual void Reload();

    virtual void SetMission( boost::shared_ptr< MIL_Mission_ABC > pMission );
    virtual boost::shared_ptr< MIL_Mission_ABC > GetMission();
    virtual void RemoveCallback( unsigned int actionId );
    virtual void CallbackKnowledge( unsigned int actionId, boost::shared_ptr< DEC_Knowledge_Object > value );
    virtual void CallbackPerception( int id );
    virtual const std::string& GetDIAType() const;
    virtual MIL_AgentPion& GetPion() const;
    virtual MIL_Automate& GetAutomate() const;
    virtual unsigned int GetID() const;
    virtual MIL_KnowledgeGroup& GetKnowledgeGroup() const;

    //virtual void GarbageCollect();

    virtual void StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission );
    virtual void StopMissionBehavior ( const boost::shared_ptr< MIL_Mission_ABC > mission );

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
    virtual bool IsNeutralized() const;
    virtual bool IsMoving() const;
    virtual bool IsContaminated() const;
    virtual const MT_Vector2D* GetPosition() const;
    virtual void SetStateVariable( const std::string& name, float value );
    virtual bool IsPC() const;
    virtual bool IsTransported() const;
    virtual bool IsFlying() const;
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
    void InitBrain( const std::string& brainFile, const std::string& type, const std::string& includePath, const std::string& groupName, bool isMasalife, bool reload );
    void CleanStateAfterCrash     ();

    void StartDefaultBehavior     ();
    void StopDefaultBehavior      ();

    void ActivateOrder( const std::string& strBehavior, const boost::shared_ptr< MIL_Mission_ABC > mission );
    void StopMission( const std::string& strBehavior );

    virtual std::string GetGroupName() = 0;
    virtual void EndCleanStateAfterCrash  () = 0;
    virtual void RegisterUserFunctions( directia::brain::Brain& brain ) = 0;
    virtual void RegisterUserArchetypeFunctions ( directia::brain::Brain& brain ) = 0;
    void RegisterReportFunctions( directia::brain::Brain& brain );

    template< typename FunctionType >
    void RegisterFunction( const std::string& strFunctionName, FunctionType function )
    {
        (*pBrain_)[ strFunctionName ] = function;
    }
    //@}

private://! @name Helpers
    //@{
    void HandleUpdateDecisionError( const std::exception* error = 0 );
    /* virtual */ void LogError   ( const std::exception* error = 0 ) const;

    virtual directia::brain::Brain& GetBrain();

    virtual void RegisterSelf( directia::brain::Brain& brain, bool isMasalife, const std::string& groupName ) = 0;
    //@}

protected:
    //!@name Data
    //@{
    T*                              pEntity_;
    boost::shared_ptr< MIL_Mission_ABC > pMission_;
    unsigned int                    gcPause_;
    unsigned int                    gcMult_;
    const DEC_Model_ABC*            model_;
    //@}

private:
    //!@name Data
    //@{
    boost::shared_ptr< directia::brain::Brain > pBrain_;
    std::auto_ptr< struct ScriptRefs > pRefs_;
    bool                               isMasalife_;
    //@}
};

struct ScriptRefs
{
public:
    ScriptRefs( directia::brain::Brain& brain );

    directia::tools::binders::ScriptRef sendEvent_;
    directia::tools::binders::ScriptRef startEvent_;
    directia::tools::binders::ScriptRef stopEvents_;
    directia::tools::binders::ScriptRef setStateVariable_;
    directia::tools::binders::ScriptRef collectgarbage_;
    directia::tools::binders::ScriptRef step_;
    directia::tools::binders::ScriptRef callbackPerception_;
    directia::tools::binders::ScriptRef knowledgeCallbackAction_;
    directia::tools::binders::ScriptRef removeAction_;
    directia::tools::binders::ScriptRef initTaskParameter_;
    directia::tools::binders::ScriptRef cleanBrainBeforeDeletion_;

private:
    ScriptRefs();
    ScriptRefs( const ScriptRefs& );
    ScriptRefs& operator=( const ScriptRefs& );
};

#include "DEC_Decision.inl"

#endif // __DEC_Decision_h_
