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
#include "simulation_kernel/NetworkUnitAttributesMessageSender_ABC.h"

namespace client
{
    class UnitAttributes;
}

class DEC_AutomateDecision;
class DEC_Knowledge_Agent;
class DEC_Path_ABC;
class MIL_Mission_ABC;
class MT_Vector2D;
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
                            , public network::NetworkUnitAttributesMessageSender_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_RolePion_Decision( MIL_AgentPion& pion, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger );
    virtual ~DEC_RolePion_Decision();
   //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Clean();

    virtual void Reload( bool force, bool doInitBrain );
    virtual void ActivateBrainDebug( bool activate );

    virtual void StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission );
    virtual void StopMissionBehavior ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    virtual void NotifyHasChanged();
    //@}

    //! @name Accessors
    //@{
    virtual MIL_AgentPion&        GetPion         () const;
    virtual MIL_Automate&         GetAutomate     () const;
    virtual boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const;

    const PHY_RoePopulation&      GetRoePopulation() const;

    virtual std::string           GetName         () const;
    virtual MIL_OrderManager_ABC& GetOrderManager() const;
    virtual DEC_AutomateDecision* GetDecAutomate  ();

    virtual const std::string& GetDIAType() const;

    virtual int GetRulesOfEngagementState() const;

    virtual int GeteEtatDec() const;
    virtual void SeteEtatDec( int value );
    virtual int GeteEtatDecPrudence() const;
    virtual void SeteEtatDecPrudence( int value );
    virtual int GeteEtatLima() const;
    virtual void SeteEtatLima( int value );
    virtual int GeteEtatNbc() const;
    virtual void SeteEtatNbc( int value );
    virtual int GeteEtatDestruction() const;
    virtual void SeteEtatDestruction( int value );
    virtual int GeteEtatFeu() const;
    virtual void SeteEtatFeu( int value );
    virtual int GeteEtatAmbiance() const;
    virtual void SeteEtatAmbiance( int value );
    virtual int GeteEtatRadio() const;
    virtual void SeteEtatRadio( int value );
    virtual int GeteEtatRadar() const;
    virtual void SeteEtatRadar( int value );
    virtual int GeteEtatDeplacement() const;
    virtual void SeteEtatDeplacement( int value );
    virtual int GeteEtatOrdreCoordination() const;
    virtual void SeteEtatOrdreCoordination( int value );
    virtual int GeteConsigneTir() const;
    virtual void SeteConsigneTir( int value );
    virtual int GeteConsigneTirPopulation() const;
    virtual void SeteConsigneTirPopulation( int value );
    virtual int GeteEtatEchelon() const;
    virtual void SeteEtatEchelon( int value );
    virtual int GeteEtatSoutien() const;
    virtual void SeteEtatSoutien( int value );
    virtual int GeteEtatSituationEnnemi() const;
    virtual void SeteEtatSituationEnnemi( int value );
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
    virtual std::vector< DEC_Decision_ABC* > GetPionsWithPC();
    virtual std::vector< DEC_Decision_ABC* > GetCommunicationPionsWithPC();
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
    virtual void SendChangedState( client::UnitAttributes& msg ) const;
    virtual void SendFullState   ( client::UnitAttributes& msg ) const;
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
    virtual std::string GetGroupName();

    virtual void RegisterUserFunctions( sword::Brain& brain );
    virtual void RegisterUserArchetypeFunctions( sword::Brain& brain );

    virtual void PostStartMission();
    virtual void PostStopMission();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void RegisterSelf( sword::Brain& brain );
    virtual void RegisterSpecific( sword::Brain& brain, bool isMasalife, const std::string& groupName );
    //@}

    //! @name Functions called from dia
    //@{
    virtual bool IsNeutralized() const;
    virtual bool IsMoving() const;
    virtual bool IsContaminated() const;
    virtual bool IsDecontaminationFinished () const;
    virtual bool IsJammed() const;
    virtual const MT_Vector2D* GetPosition() const;
    virtual bool IsPC() const;
    virtual bool IsTransported() const;
    virtual bool IsFlying() const;
    virtual double GetOperationalState() const;
    virtual double GetMajorOperationalState() const;
    virtual bool IsAutomateEngaged() const;
    virtual bool IsDead() const;
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

    int eEtatEchelon_;
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
    int eEtatSoutien_;
    int eEtatSituationEnnemi_;
    int eEtatPhaseMission_;
    int eTypeContact_;
    int eNiveauAction_;

    bool bOrdreInterrompreMission_;
    bool bOrdreDecrocher_;
    bool bOrdreTenirSurLR_;
    bool bOrdreTenir_;
    bool bPasserSurLC_;
    bool bDefenseStatique_Mobile_;
    bool bStateHasChanged_;     // Network
    DEC_Decision_ABC* pionEnEscorte_;
    std::vector< DEC_Decision_ABC* > pionsEnAppui_;
    boost::shared_ptr< DEC_Path_ABC > itMvt_;
    boost::shared_ptr< MT_Vector2D > objectifEsquive_;
    boost::shared_ptr< DEC_Knowledge_Agent > eniEnCours_;
    std::string missionPrecedente_;
    float rTenir_;
    std::vector< unsigned int > internalMissionActions_;

    MIL_AgentPion& pion_;
};

BOOST_CLASS_EXPORT_KEY( DEC_RolePion_Decision )

template< typename Archive >
void save_construct_data( Archive& archive, const DEC_RolePion_Decision* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion = role->pEntity_;
    const unsigned int gcPause = role->gcPause_;
    const unsigned int gcMult = role->gcMult_;
    sword::DEC_Logger* logger = role->logger_;
    archive << pion
            << gcPause
            << gcMult
            << logger;
}

template< typename Archive >
void load_construct_data( Archive& archive, DEC_RolePion_Decision* role, const unsigned int /*version*/ )
{
    MIL_AgentPion* pion;
    unsigned int gcPause;
    unsigned int gcMult;
    sword::DEC_Logger* logger;
    archive >> pion
            >> gcPause
            >> gcMult
            >> logger;
    ::new( role )DEC_RolePion_Decision( *pion, gcPause, gcMult, logger );
}

#endif // __DEC_RolePion_Decision_h_
