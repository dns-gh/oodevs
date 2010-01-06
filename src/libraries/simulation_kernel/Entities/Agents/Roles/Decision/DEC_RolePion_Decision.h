// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Agents/Roles/Decision/DEC_RolePion_Decision.h $
// $Author: Nld $
// $Modtime: 23/06/05 10:29 $
// $Revision: 10 $
// $Workfile: DEC_RolePion_Decision.h $
//
// *****************************************************************************

#ifndef __DEC_RolePion_Decision_h_
#define __DEC_RolePion_Decision_h_

#include "Decision/DEC_Decision.h"
#include "Entities/Agents/MIL_AgentPion.h"
#include "Knowledge/DEC_KnowledgeBlackBoard_AgentPion.h"
#include "simulation_kernel/NetworkUnitMessageNotificationHandler_ABC.h"

class DEC_AutomateDecision;
class DEC_Knowledge_Agent;
class DEC_Path_ABC;
class MIL_Mission_ABC;
class MT_Vector2D;
class NET_ASN_MsgUnitAttributes;
class PHY_RoePopulation;

enum E_CloseCombatState;
enum E_FightRateState;
enum E_FireAvailability;
enum E_ForceRatioState;
enum E_OperationalState;
enum E_RulesOfEngagementState;

// =============================================================================
// @class  DEC_RolePion_Decision
// Created: JVT 2004-08-03
// =============================================================================
class DEC_RolePion_Decision : public DEC_Decision< MIL_AgentPion >
                            , public network::NetworkUnitMessageNotificationHandler_ABC
                            , private boost::noncopyable
{

public:
             DEC_RolePion_Decision( MIL_AgentPion& pion, DEC_DataBase& database );
    virtual ~DEC_RolePion_Decision();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Clean         ();

    virtual void StartMissionBehavior( MIL_Mission_ABC& mission );
    virtual void StopMissionBehavior ( MIL_Mission_ABC& mission );
    virtual void NotifyHasChanged();
    //@}

    //! @name Accessors
    //@{
    virtual MIL_AgentPion&        GetPion         () const;

    const PHY_RoePopulation&    GetRoePopulation() const;

    virtual std::string           GetName         () const;
    virtual DEC_AutomateDecision* GetDecAutomate  () const;

    virtual const std::string& GetDIAType() const;

    virtual int GeteEtatDec() const;
    virtual void SeteEtatDec( int value );
    int GeteEtatDecPrudence() const;
    void SeteEtatDecPrudence( int value );
    virtual int GeteEtatLima() const;
    virtual void SeteEtatLima( int value );
    int GeteEtatNbc() const;
    void SeteEtatNbc( int value );
    int GeteEtatDestruction() const;
    void SeteEtatDestruction( int value );
    int GeteEtatFeu() const;
    void SeteEtatFeu( int value );
    int GeteEtatAmbiance() const;
    void SeteEtatAmbiance( int value );
    int GeteEtatRadio() const;
    void SeteEtatRadio( int value );
    int GeteEtatRadar() const;
    void SeteEtatRadar( int value );
    int GeteEtatDeplacement() const;
    void SeteEtatDeplacement( int value );
    int GeteEtatOrdreCoordination() const;
    void SeteEtatOrdreCoordination( int value );
    int GeteConsigneTir() const;
    void SeteConsigneTir( int value );
    int GeteConsigneTirPopulation() const;
    void SeteConsigneTirPopulation( int value );
    virtual int GeteEtatEchelon() const;
    virtual void SeteEtatEchelon( int value );
    int GeteEtatSoutien() const;
    void SeteEtatSoutien( int value );
    int GeteEtatSituationEnnemi() const;
    void SeteEtatSituationEnnemi( int value );
    virtual int GeteEtatPhaseMission() const;
    virtual void SeteEtatPhaseMission( int value );
    bool GetbOrdreInterrompreMission() const;
    void SetbOrdreInterrompreMission( bool value );
    virtual bool GetbOrdreDecrocher() const;
    virtual void SetbOrdreDecrocher( bool value );
    virtual bool GetbOrdreTenirSurLR() const;
    virtual void SetbOrdreTenirSurLR( bool value );
    virtual bool GetbOrdreTenir() const;
    virtual void SetbOrdreTenir( bool value );
    bool GetbPasserSurLC() const;
    void SetbPasserSurLC( bool value );
    DEC_Decision_ABC* GetpionEnEscorte() const;
    void SetpionEnEscorte( DEC_Decision_ABC* value );
    boost::shared_ptr< DEC_Path_ABC > GetitMvt() const;
    void SetitMvt( const boost::shared_ptr< DEC_Path_ABC >& value );
    boost::shared_ptr< MT_Vector2D > GetobjectifEsquive() const;
    void SetobjectifEsquive( MT_Vector2D* value );
    boost::shared_ptr< DEC_Knowledge_Agent > GeteniEnCours() const;
    void SeteniEnCours( boost::shared_ptr< DEC_Knowledge_Agent > value );
    std::string GetmissionPrecedente() const;
    void SetmissionPrecedente( const std::string& value );
    float GetrTenir() const;
    void SetrTenir( float value );
    int GeteTypeContact() const;
    void SeteTypeContact( int value );
    int GeteNiveauAction() const;
    void SeteNiveauAction( int value );
    bool GetbDefenseStatique_Mobile() const;
    void SetbDefenseStatique_Mobile( bool value );
    const std::vector< DEC_Decision_ABC* >* GetselUnitesEnAppui() const;
    void AddToselUnitesEnAppui( DEC_Decision_ABC* pPion );
    void RemoveFromselUnitesEnAppui( DEC_Decision_ABC* pPion );
    //@}

    //! @name Network
    //@{
    virtual void SendChangedState( NET_ASN_MsgUnitAttributes& msg ) const;
    virtual void SendFullState   ( NET_ASN_MsgUnitAttributes& msg ) const;
    //@}

    //! @name Notifications
    //@{
    void NotifyForceRatioStateChanged         ( E_ForceRatioState        nState );
    void NotifyRulesOfEngagementStateChanged  ( E_RulesOfEngagementState nState );
    void NotifyCloseCombatStateChanged        ( E_CloseCombatState       nState );
    void NotifyOperationalStateChanged        ( E_OperationalState       nState );
    void NotifyIndirectFireAvailabilityChanged( E_FireAvailability       nState );
    void NotifyRoePopulationChanged           ( const PHY_RoePopulation& roe );
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void EndCleanStateAfterCrash  ();

    virtual void RegisterUserFunctions( directia::Brain& brain );
    //@}
    
private:
    //! @name Helpers
    //@{
    virtual void RegisterSelf( directia::Brain& brain );
    virtual void UpdateMeKnowledge( directia::Brain& brain );
    //@}

    //! @name Functions called from dia
    //@{
    virtual bool IsNeutralized() const;
    virtual bool IsMoving() const;
    virtual bool IsContaminated() const;
    virtual const MT_Vector2D* GetPosition() const;
    virtual bool IsPC() const;
    virtual bool IsTransported() const;
    virtual bool IsFlying() const;
    virtual MT_Float GetMajorOperationalState() const;
    virtual bool IsAutomateEngaged() const;
    virtual bool IsDead() const;
    virtual void WearNbcProtectionSuit() const;
    virtual void RemoveNbcProtectionSuit() const;
    //@}

    //! @name Checkpoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_RolePion_Decision* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_RolePion_Decision* role, const unsigned int /*version*/ );
    //@}

private:
    // Etat décisionnel
          E_ForceRatioState        nForceRatioState_;
          E_RulesOfEngagementState nRulesOfEngagementState_;
          E_CloseCombatState       nCloseCombatState_;
          E_OperationalState       nOperationalState_;
          E_FireAvailability       nIndirectFireAvailability_;
    const PHY_RoePopulation*       pRoePopulation_; //$$$ à déplacer dans Role_Population ?

    int eEtatDec_;
    int eEtatDecPrudence_;
    int eEtatLima_;
    int eEtatNbc_;
    int eEtatDestruction_;
    int eEtatFeu_;
    int eEtatAmbiance_;
    int eEtatRadio_;
    int eEtatRadar_;
    int eEtatDeplacement_;
    int eEtatOrdreCoordination_;
    int eConsigneTir_;
    int eConsigneTirPopulation_;
    int eEtatEchelon_;
    int eEtatSoutien_;
    int eEtatSituationEnnemi_;
    int eEtatPhaseMission_;
    bool bOrdreInterrompreMission_;
    bool bOrdreDecrocher_;
    bool bOrdreTenirSurLR_;
    bool bOrdreTenir_;
    bool bPasserSurLC_;
    DEC_Decision_ABC* pionEnEscorte_;
    std::vector< DEC_Decision_ABC* > pionsEnAppui_;
    boost::shared_ptr< DEC_Path_ABC > itMvt_;
    boost::shared_ptr< MT_Vector2D > objectifEsquive_;
    boost::shared_ptr< DEC_Knowledge_Agent > eniEnCours_;
    std::string missionPrecedente_;
    float rTenir_;
    int eTypeContact_;
    int eNiveauAction_;
    bool bDefenseStatique_Mobile_;

    // Network
    bool bStateHasChanged_;

    std::string              name_;
    DEC_AutomateDecision*    pAutomate_;
};

template< typename Archive >
void save_construct_data( Archive& archive, const DEC_RolePion_Decision* role, const unsigned int /*version*/ )
{
    const DEC_DataBase* const database = &role->database_;
    archive << role->pEntity_
        << database;
}

template< typename Archive >
void load_construct_data( Archive& archive, DEC_RolePion_Decision* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    DEC_DataBase* database;
    archive >> pion
        >> database;
    ::new( role )DEC_RolePion_Decision( *pion, *database );
}

#endif // __DEC_RolePion_Decision_h_
