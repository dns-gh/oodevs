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
#include <directia/Brain.h>

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
    explicit DEC_Decision( T& entity, DEC_DataBase& database );
    virtual ~DEC_Decision();
    //@}

    //! @name Operations
    //@{
    void SetModel( const DEC_Model_ABC& model );
    virtual void UpdateDecision( float duration );
    virtual void Reset();
    
    virtual void SetMission( MIL_Mission_ABC* pMission );
    virtual MIL_Mission_ABC* GetMission();
    virtual void RemoveCallback( PHY_Action_ABC* pAction );
    virtual void CallbackKnowledge( PHY_Action_ABC* pAction, boost::shared_ptr< DEC_Knowledge_Object > value );
    virtual void CallbackPerception( int id );
    virtual const std::string& GetDIAType() const;
    virtual MIL_AgentPion& GetPion() const;
    virtual MIL_Automate& GetAutomate() const;
    
    virtual void GarbageCollect();
    
    virtual void StartMissionBehavior( MIL_Mission_ABC& mission );
    virtual void StopMissionBehavior ( MIL_Mission_ABC& mission );

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
    virtual std::vector<DEC_Decision_ABC*> GetListePionsCoordination();
    virtual int GetObjMisEnCours();
    virtual void SetObjMisEnCours( int );
    virtual boost::shared_ptr< MT_Vector2D > GetObjectifCourant();
    virtual int GetPlotRavitaillementAssigne();
    virtual void SetPlotRavitaillementAssigne( int );
    virtual int GetPorteeAction();
    virtual void SetPorteeAction( int );
    virtual float GetNiveauAlerteRavitaillement();
    virtual boost::shared_ptr< MT_Vector2D > GetLastPointOfPath( const MT_Float time, bool bBoundOnPath ) const;
    virtual boost::shared_ptr< MT_Vector2D > ExtrapolatePosition( const MT_Float time, bool bBoundOnPath ) const;
   
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
    virtual void WearNbcProtectionSuit() const;
    virtual void RemoveNbcProtectionSuit() const;
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
    void CleanStateAfterCrash     ();

    void StartDefaultBehavior     ();
    void StopDefaultBehavior      ();
    void LogCrash                 ();
    
    void ActivateOrder( const std::string& strBehavior, MIL_Mission_ABC& mission );
    void StopMission( const std::string& strBehavior );

    virtual void EndCleanStateAfterCrash  () = 0;
    virtual void RegisterUserFunctions( directia::Brain& brain ) = 0;
    //@}
    
private://! @name Helpers
    //@{
    void HandleUpdateDecisionError ();
    virtual directia::Brain& GetBrain();
    
    void InitBrain( const std::string& brainFile, const std::string& type, const std::string& includePath );
    
    virtual void RegisterSelf( directia::Brain& brain ) = 0;
    virtual void UpdateMeKnowledge( directia::Brain& brain ) = 0;
    //@}

protected:
    //!@name Data
    //@{
    T*                              pEntity_;
    MIL_Mission_ABC*                pMission_;
    DEC_DataBase&                   database_;
    //@}

private:
    //!@name Data
    //@{
    std::auto_ptr< directia::Brain >   pBrain_;
    bool                               bDIA5_;
    std::auto_ptr< struct ScriptRefs > pRefs_;
    std::string                        brainFile_;
    std::string                        includePath_;
    std::string                        diaType_;
    int                                toto_;
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
