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
#include "Entities/Orders/MIL_AutomateOrderManager.h"
#include "MT_Tools/MT_Scipio_enum.h"

namespace client
{
    class AutomatAttributes;
}

class MIL_Mission_ABC;
class PHY_RoePopulation;

namespace sword
{
    class DEC_Logger;
}

// =============================================================================
// @class  DEC_AutomateDecision
// Created: JVT 2004-08-03
// =============================================================================
class DEC_AutomateDecision : public DEC_Decision< MIL_Automate >
{
public:
             DEC_AutomateDecision( MIL_Automate& automate, unsigned int gcPause, unsigned int gcMult, sword::DEC_Logger* logger );
    virtual ~DEC_AutomateDecision();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_AutomateDecision* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_AutomateDecision* role, const unsigned int /*version*/ );
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Clean();
    virtual void Reload( bool force, bool doInitBrain );
    virtual void ActivateBrainDebug( bool activate );

    bool HasStateChanged() const;

    void StartMissionMrtBehavior     ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    void StopMissionMrtBehavior      ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    void StartMissionConduiteBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission );
    void StopMissionConduiteBehavior ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    //@}

    //! @name Notifications
    //@{
    void NotifyCloseCombatStateChanged( E_CloseCombatState nState );
    //@}

    //! @name Network
    //@{
    void SendChangedState( client::AutomatAttributes& msg ) const;
    void SendFullState   ( client::AutomatAttributes& msg ) const;
    //@}

    //! @name Accessors
    //@{
    virtual MIL_Automate& GetAutomate() const;
    virtual boost::shared_ptr< MIL_KnowledgeGroup > GetKnowledgeGroup() const;
    virtual DEC_AutomateDecision* GetDecAutomate();
    virtual std::string GetName() const;
    virtual MIL_OrderManager_ABC& GetOrderManager() const;

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

    void AddReconnoiteredPointBy( DEC_Decision_ABC* pPion, MT_Vector2D& point );
    bool PointIsReconnoiteredByMeOrNoOne( DEC_Decision_ABC* pPion, MT_Vector2D& point ) const;

    virtual std::vector< DEC_Decision_ABC* > GetPionsWithPC();

    virtual const std::string& GetDIAType() const;
    void NotifyRulesOfEngagementStateChanged( E_RulesOfEngagementState state );
    void NotifyRulesOfEngagementPopulationStateChanged( const PHY_RoePopulation& roe );
    const PHY_RoePopulation& GetRoePopulation() const;
    int GetRulesOfEngagementState() const;

    bool IsPionNeutralized( DEC_Decision_ABC* );
    //@}

protected:
    //! @name Tools
    //@{
    virtual void EndCleanStateAfterCrash();
    virtual std::string GetGroupName();

    virtual void RegisterUserFunctions( sword::Brain& brain );
    virtual void RegisterUserArchetypeFunctions( sword::Brain& brain );

    virtual void PostStartMission();
    virtual void PostStopMission();
    //@}

private:
    //! @name Tools
    //@{
    virtual void RegisterSelf( sword::Brain& brain );
    virtual void RegisterSpecific( sword::Brain& brain, bool isMasalife, const std::string& groupName );
    //@}

private:
    // Etat décisionnel
    std::map< MT_Vector2D, DEC_Decision_ABC* > listReconnoitringPoint_;
    const PHY_RoePopulation* pRoePopulation_;
    E_RulesOfEngagementState nRulesOfEngagementState_;
    E_CloseCombatState       nCloseCombatState_;
    E_OperationalState       nOperationalState_;
    int                      ePhaseMission_;
    int                      eEtatLima_;
    int                      eEtatDec_;
    int                      eEtatEchelon_;
    float                    rDestruction_;
    bool                     bStateHasChanged_;
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
    archive << role->pEntity_;
    archive << role->gcPause_;
    archive << role->gcMult_;
    archive << role->logger_;
}

template< typename Archive >
void load_construct_data( Archive& archive, DEC_AutomateDecision* role, const unsigned int /*version*/ )
{
    MIL_Automate* automate;
    unsigned int gcPause;
    unsigned int gcMult;
    sword::DEC_Logger* logger;
    archive >> automate;
    archive >> gcPause;
    archive >> gcMult;
    archive >> logger;
    ::new( role )DEC_AutomateDecision( *automate, gcPause, gcMult, logger );
}

#endif // __DEC_AutomateDecision_h_
