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
#include "MT/MT_Logger/MT_Logger_lib.h"

class DEC_DataBase;
class DEC_Knowledge_Object;
class DEC_Model_ABC;
class MIL_Entity_ABC;
class MIL_Mission_ABC;

namespace directia
{
    class Brain;
    class ScriptRef;
}

// =============================================================================
/** @class  DEC_Decision
    @brief  DEC decision
    Template T must be a subclass of MIL_Entity_ABC
*/
// Created: LDC 2009-02-27
// =============================================================================
template < class T >
class DEC_Decision : public DEC_Decision_ABC
{
public:
    //! @name Constructor
    //@{
             DEC_Decision( T& entity, DEC_DataBase& database );
    virtual ~DEC_Decision();
    //@}

    //! @name Operations
    //@{
    void SetModel( const DEC_Model_ABC& model );
    virtual void UpdateDecision( float duration );
    virtual void Reset( std::string groupName = "" );

    virtual void SetMission( boost::shared_ptr< MIL_Mission_ABC > pMission );
    virtual boost::shared_ptr< MIL_Mission_ABC > GetMission();
    virtual void RemoveCallback( unsigned int actionId );
    virtual void CallbackKnowledge( unsigned int actionId, boost::shared_ptr< DEC_Knowledge_Object > value );
    virtual void CallbackPerception( int id );
    virtual const std::string& GetDIAType() const;
    virtual MIL_AgentPion& GetPion() const;
    virtual MIL_Automate& GetAutomate() const;

    virtual void GarbageCollect();

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
    virtual boost::shared_ptr< DEC_Knowledge_Object > GetPlotRavitaillementAssigne();
    virtual void SetPlotRavitaillementAssigne( boost::shared_ptr< DEC_Knowledge_Object > );
    virtual int GetPorteeAction();
    virtual void SetPorteeAction( int );
    virtual float GetNiveauAlerteRavitaillement();
    virtual boost::shared_ptr< MT_Vector2D > GetLastPointOfPath( const MT_Float time, bool bBoundOnPath ) const;
    virtual boost::shared_ptr< MT_Vector2D > ExtrapolatePosition( const MT_Float time, bool bBoundOnPath ) const;

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
    virtual MT_Float GetMajorOperationalState() const;
    virtual bool IsAutomateEngaged() const;
    virtual bool IsDead() const;
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive& ar, const uint )
    {
        ar & boost::serialization::base_object< DEC_Decision_ABC >( *this );
    }
    //@}

protected:
    //! @name Helpers
    //@{
    void InitBrain( const std::string& brainFile, const std::string& type, const std::string& includePath, const std::string& groupName );
    void CleanStateAfterCrash     ();

    void StartDefaultBehavior     ();
    void StopDefaultBehavior      ();
    void LogCrash                 ();

    void ActivateOrder( const std::string& strBehavior, const boost::shared_ptr< MIL_Mission_ABC > mission );
    void StopMission( const std::string& strBehavior );

    virtual void EndCleanStateAfterCrash  () = 0;
    virtual void RegisterUserFunctions( directia::Brain& brain ) = 0;

    template< typename FunctionType >
    void RegisterFunction( const std::string& strFunctionName, FunctionType function )
    {
        pBrain_->RegisterFunction( strFunctionName, function );
    }
    //@}

private://! @name Helpers
    //@{
    void HandleUpdateDecisionError ();
    virtual directia::Brain& GetBrain();

    virtual void RegisterSelf( directia::Brain& brain ) = 0;
    //@}

protected:
    //!@name Data
    //@{
    T*                                   pEntity_;
    boost::shared_ptr< MIL_Mission_ABC > pMission_;
    std::string                          diaType_;
    DEC_DataBase&                        database_;
    //@}

private:
    //!@name Data
    //@{
    std::auto_ptr< directia::Brain >   pBrain_;
    std::auto_ptr< struct ScriptRefs > pRefs_;
    std::string                        brainFile_;
    std::string                        includePath_;
    std::string                        modelName_;
    //@}
};

struct ScriptRefs
{
public:
    ScriptRefs( directia::Brain& brain );

    directia::ScriptRef sendEvent_;
    directia::ScriptRef startEvent_;
    directia::ScriptRef stopEvents_;
    directia::ScriptRef setStateVariable_;
    directia::ScriptRef collectgarbage_;
    directia::ScriptRef step_;
private:
    ScriptRefs();
    ScriptRefs( const ScriptRefs& );
    ScriptRefs& operator=( const ScriptRefs& );
};

#include "DEC_Decision.inl"

#endif // __DEC_Decision_h_
