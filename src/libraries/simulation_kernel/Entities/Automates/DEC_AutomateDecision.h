// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Automates/DEC_AutomateDecision.h $
// $Author: Nld $
// $Modtime: 23/06/05 10:29 $
// $Revision: 9 $
// $Workfile: DEC_AutomateDecision.h $
//
// *****************************************************************************

#ifndef __DEC_AutomateDecision_h_
#define __DEC_AutomateDecision_h_

#include "Decision/DEC_Decision.h"
#include "Entities/Automates/MIL_Automate.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_Automate.h"
#include "MT_Tools/MT_Scipio_enum.h"

namespace client
{
    class AutomatAttributes;
}

class MIL_Mission_ABC;
class DEC_Decision_ABC;

enum E_ForceRatioState;
enum E_RulesOfEngagementState;
enum E_CloseCombatState;

// =============================================================================
// @class  DEC_AutomateDecision
// Created: JVT 2004-08-03
// =============================================================================
class DEC_AutomateDecision : public DEC_Decision< MIL_Automate >
                           , private boost::noncopyable
{
public:
             DEC_AutomateDecision( MIL_Automate& automate, DEC_DataBase& database, unsigned int gcPause, unsigned int gcMult );
    virtual ~DEC_AutomateDecision();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_AutomateDecision* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_AutomateDecision* role, const unsigned int /*version*/ );
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void Clean          ();
    bool HasStateChanged() const;

    void StartMissionMrtBehavior     ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    void StopMissionMrtBehavior      ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    void StartMissionConduiteBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission );
    void StopMissionConduiteBehavior ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    //@}

    //! @name Notifications
    //@{
    void NotifyCloseCombatStateChanged      ( E_CloseCombatState       nState );
    //@}

    //! @name Network
    //@{
    void SendChangedState( client::AutomatAttributes& msg ) const;
    void SendFullState   ( client::AutomatAttributes& msg ) const;
    //@}

    //! @name Accessors
    //@{
    virtual MIL_Automate& GetAutomate() const;
    virtual DEC_AutomateDecision* GetDecAutomate() const;
    virtual std::string GetName() const;

    virtual int GeteEtatPhaseMission() const;
    virtual void SeteEtatPhaseMission( int value );
    virtual int GeteEtatLima() const;
    virtual void SeteEtatLima(  int value );
    virtual int GeteEtatDec() const;
    virtual void SeteEtatDec(  int value );
    virtual int GeteEtatEchelon() const;
    virtual void SeteEtatEchelon( int );

    float GetrDestruction() const;
    void SetrDestruction( float value );
    const MIL_Fuseau* Getfuseau() const;
    void Setfuseau( MIL_Fuseau* value );
    TER_Localisation* Getzone() const;
    void Setzone( TER_Localisation* value );
    bool GetbOrdreAttendre() const;
    void SetbOrdreAttendre( bool value );
    bool GetbOrdrePoursuivre() const;
    void SetbOrdrePoursuivre( bool value );
    bool GetbOrdreRalentir() const;
    void SetbOrdreRalentir( bool value );
    virtual bool GetbOrdreDecrocher() const;
    virtual void SetbOrdreDecrocher( bool value );
    virtual bool GetbOrdreTenirSurLR() const;
    virtual void SetbOrdreTenirSurLR( bool value );
    virtual bool GetbOrdreTenir() const;
    virtual void SetbOrdreTenir( bool value );
    virtual bool IsAutomateEngaged() const;

    virtual std::vector< DEC_Decision_ABC* > GetPionsWithPC();

    virtual const std::string& GetDIAType() const;
    //@}

protected:
    //! @name Tools
    //@{
    virtual void      EndCleanStateAfterCrash      ();

    virtual void RegisterUserFunctions( directia::brain::Brain& brain );
    virtual void RegisterUserArchetypeFunctions( directia::brain::Brain& brain );
    //@}

private:
    //! @name Tools
    //@{
    virtual void RegisterSelf( directia::brain::Brain& brain );
    //@}

private:
    //! @name Functions
    //@{
    bool IsPionNeutralized( DEC_Decision_ABC* );
    //@}

private:
    // Etat d�cisionnel
    E_RulesOfEngagementState nRulesOfEngagementState_;
    E_CloseCombatState       nCloseCombatState_;
    E_OperationalState       nOperationalState_;
    bool                     bStateHasChanged_;
    int                      ePhaseMission_;
    int                      eEtatLima_;
    int                      eEtatDec_;
    float                    rDestruction_;
    int                      eEtatEchelon_;
    bool                     bOrdreAttendre_;
    bool                     bOrdrePoursuivre_;
    bool                     bOrdreRalentir_;
    bool                     bOrdreDecrocher_;
    bool                     bOrdreTenirSurLR_;
    bool                     bOrdreTenir_;

    MIL_Fuseau* fuseau_;

private:
    static int nDIAMissionIdx_; // index de mission_ dans T_Automate
};

BOOST_CLASS_EXPORT_KEY( DEC_AutomateDecision )

template< typename Archive >
void save_construct_data( Archive& archive, const DEC_AutomateDecision* role, const unsigned int /*version*/ )
{
    const DEC_DataBase* const database = &role->database_;
    archive << role->pEntity_
        << database
        << role->gcPause_
        << role->gcMult_;
}

template< typename Archive >
void load_construct_data( Archive& archive, DEC_AutomateDecision* role, const unsigned int /*version*/ )
{
    MIL_Automate* automate;
    DEC_DataBase* database;
    unsigned int gcPause;
    unsigned int gcMult;
    archive >> automate
        >> database
        >> gcPause
        >> gcMult;
    ::new( role )DEC_AutomateDecision( *automate, *database, gcPause, gcMult );
}

#endif // __DEC_AutomateDecision_h_
