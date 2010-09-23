// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Populations/DEC_PopulationDecision.h $
// $Author: Nld $
// $Modtime: 23/06/05 10:29 $
// $Revision: 9 $
// $Workfile: DEC_PopulationDecision.h $
//
// *****************************************************************************

#ifndef __DEC_PopulationDecision_h_
#define __DEC_PopulationDecision_h_

#include "Decision/DEC_Decision.h"
#include "Entities/Populations/MIL_Population.h"

namespace client
{
    class PopulationUpdate;
}

class MIL_Mission_ABC;

// =============================================================================
// @class  DEC_PopulationDecision
// Created: JVT 2004-08-03
// =============================================================================
class DEC_PopulationDecision : public DEC_Decision< MIL_Population >
                             , private boost::noncopyable
{

public:
             DEC_PopulationDecision( MIL_Population& population, DEC_DataBase& database, unsigned int gcPause, unsigned int gcMult );
    virtual ~DEC_PopulationDecision();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    //! @name Checkpoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_PopulationDecision* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_PopulationDecision* role, const unsigned int /*version*/ );
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

    //! @name Operations
    //@{
    void SetModel( const DEC_Model_ABC& model );
    void Clean         ();

    /*
    virtual int GeteEtatDecPrudence() const;
    virtual void SeteEtatDecPrudence(int value );
    virtual int GeteEtatNbc() const;
    virtual void SeteEtatNbc(int value );
    virtual int GeteEtatDestruction() const;
    virtual void SeteEtatDestruction(int value );
    virtual int GeteEtatFeu() const;
    virtual void SeteEtatFeu(int value );
    virtual int GeteEtatAmbiance() const;
    virtual void SeteEtatAmbiance(int value );
    virtual int GeteEtatRadio() const;
    virtual void SeteEtatRadio(int value );
    virtual int GeteEtatRadar() const;
    virtual void SeteEtatRadar(int value );
    virtual int GeteEtatDeplacement() const;
    virtual void SeteEtatDeplacement(int value );
    virtual int GeteEtatOrdreCoordination() const;
    virtual void SeteEtatOrdreCoordination(int value );
    virtual int GeteConsigneTir() const;
    virtual void SeteConsigneTir(int value );
    virtual int GeteConsigneTirPopulation() const;
    virtual void SeteConsigneTirPopulation(int value );
    virtual int GeteEtatSoutien() const;
    virtual void SeteEtatSoutien(int value );
    virtual int GeteEtatSituationEnnemi() const;
    virtual void SeteEtatSituationEnnemi(int value );
    virtual int GeteTypeContact() const;
    virtual void SeteTypeContact(int value );
    virtual int GeteNiveauAction() const;
    virtual void SeteNiveauAction(int value );
    */

    virtual void StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission );
    virtual void StopMissionBehavior ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    //@}

    //! @name Accessors
    //@{
    MIL_Population&    GetPopulation     () const;
    MT_Float           GetDominationState() const;
    bool               HasStateChanged   () const; // Etat decisionnel
    virtual DEC_AutomateDecision* GetDecAutomate() const;
    virtual std::string GetName() const;
    //@}

    //! @name Notifications
    //@{
    void NotifyDominationStateChanged( MT_Float rValue );
    //@}

    //! @name Network
    //@{
    void SendChangedState( client::PopulationUpdate& msg );
    void SendFullState   ( client::PopulationUpdate& msg ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void      EndCleanStateAfterCrash      ();

    virtual void RegisterUserFunctions( directia::brain::Brain& brain );
    virtual void RegisterUserArchetypeFunctions( directia::brain::Brain& brain );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void RegisterSelf( directia::brain::Brain& brain );
    //@}

private:
    MT_Float                             rDominationState_;
    MT_Float                             rLastDominationState_;
    bool                                 bStateHasChanged_;
    std::string                          name_;

private:
    static int nDIAMissionIdx_; // index de mission_ dans T_Population
    static int nDIANameIdx_;
};

BOOST_CLASS_EXPORT_KEY( DEC_PopulationDecision )

template< typename Archive >
void save_construct_data( Archive& archive, const DEC_PopulationDecision* role, const unsigned int /*version*/ )
{
    const DEC_DataBase* const database = &role->database_;
    archive << role->pEntity_
        << database
        << role->gcPause_
        << role->gcMult_;
}

template< typename Archive >
void load_construct_data( Archive& archive, DEC_PopulationDecision* role, const unsigned int /*version*/ )
{
    MIL_Population* population;
    DEC_DataBase* database;
    unsigned int gcPause;
    unsigned int gcMult;
    archive >> population
        >> database
        >> gcPause
        >> gcMult;
    ::new( role )DEC_PopulationDecision( *population, *database, gcPause, gcMult );
}


#endif // __DEC_PopulationDecision_h_
