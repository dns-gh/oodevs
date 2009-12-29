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

class MIL_Mission_ABC;

class NET_ASN_MsgAutomatAttributes;
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
    explicit DEC_AutomateDecision( MIL_Automate& automate, DEC_DataBase& database );
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

    void StartMissionMrtBehavior     ( MIL_Mission_ABC& mission );
    void StopMissionMrtBehavior      ( MIL_Mission_ABC& mission );
    void StartMissionConduiteBehavior( MIL_Mission_ABC& mission );
    void StopMissionConduiteBehavior ( MIL_Mission_ABC& mission );
    //@}

    //! @name Notifications
    //@{
    void NotifyCloseCombatStateChanged      ( E_CloseCombatState       nState );
    //@}

    //! @name Network
    //@{
    void SendChangedState( NET_ASN_MsgAutomatAttributes& msg ) const;
    void SendFullState   ( NET_ASN_MsgAutomatAttributes& msg ) const;
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
    float GetrDestruction() const;
    void SetrDestruction( float value );
    virtual int GeteEtatEchelon() const;
    virtual void SeteEtatEchelon( int );
    MIL_Fuseau* Getfuseau() const;
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

    virtual std::vector< DEC_Decision_ABC* > GetPionsWithPC();

    virtual const std::string& GetDIAType() const;
    //@}

protected:
    //! @name Tools
    //@{
    virtual void      EndCleanStateAfterCrash      ();

    virtual void RegisterUserFunctions( directia::Brain& brain );
    //@}

private:
    //! @name Tools
    //@{
    virtual void RegisterSelf( directia::Brain& brain );
    virtual void UpdateMeKnowledge( directia::Brain& brain );
    //@}

private:
    //! @name Functions
    //@{
    bool IsPionNeutralized( DEC_Decision_ABC* );
    //@}

private:
    // Etat décisionnel
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


private:
    static int nDIAMissionIdx_; // index de mission_ dans T_Automate
    //    static int nDIANameIdx_;
};

template< typename Archive >
void save_construct_data( Archive& archive, const DEC_AutomateDecision* role, const unsigned int /*version*/ )
{
    const DEC_DataBase* const database = &role->database_;
    archive << role->pEntity_ 
        << database;
}

template< typename Archive >
void load_construct_data( Archive& archive, DEC_AutomateDecision* role, const unsigned int /*version*/ )
{
    MIL_Automate* automate;
    DEC_DataBase* database;
    archive >> automate
        >> database;
    ::new( role )DEC_AutomateDecision( *automate, *database );
}

#endif // __DEC_AutomateDecision_h_
