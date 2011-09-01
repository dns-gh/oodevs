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
    class CrowdUpdate;
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
             DEC_PopulationDecision( MIL_Population& population, unsigned int gcPause, unsigned int gcMult );
    virtual ~DEC_PopulationDecision();

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    //! @name Checkpoint
    //@{
    template< typename Archive > friend  void save_construct_data( Archive& archive, const DEC_PopulationDecision* role, const unsigned int /*version*/ );
    template< typename Archive > friend  void load_construct_data( Archive& archive, DEC_PopulationDecision* role, const unsigned int /*version*/ );
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void SetModel( const DEC_Model_ABC& model );
    void Clean();

    virtual void StartMissionBehavior( const boost::shared_ptr< MIL_Mission_ABC > mission );
    virtual void StopMissionBehavior ( const boost::shared_ptr< MIL_Mission_ABC > mission );
    //@}

    //! @name Accessors
    //@{
    MIL_Population& GetPopulation() const;
    double GetDominationState() const;
    bool HasStateChanged() const; // Etat decisionnel
    virtual DEC_AutomateDecision* GetDecAutomate() const;
    virtual std::string GetName() const;
    //@}

    //! @name Notifications
    //@{
    void NotifyDominationStateChanged( double rValue );
    //@}

    //! @name Network
    //@{
    void SendChangedState( client::CrowdUpdate& msg );
    void SendFullState   ( client::CrowdUpdate& msg ) const;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void EndCleanStateAfterCrash();
    virtual std::string GetGroupName();
    virtual void RegisterUserFunctions( directia::brain::Brain& brain );
    virtual void RegisterUserArchetypeFunctions( directia::brain::Brain& brain );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void RegisterSelf( directia::brain::Brain& brain, bool isMasalife, const std::string& groupName );
    //@}

private:
    double rDominationState_;
    double rLastDominationState_;
    bool bStateHasChanged_;
    std::string name_;

private:
    static int nDIAMissionIdx_; // index de mission_ dans T_Population
    static int nDIANameIdx_;
};

BOOST_CLASS_EXPORT_KEY( DEC_PopulationDecision )

template< typename Archive >
void save_construct_data( Archive& archive, const DEC_PopulationDecision* role, const unsigned int /*version*/ )
{
    archive << role->pEntity_
            << role->gcPause_
            << role->gcMult_;
}

template< typename Archive >
void load_construct_data( Archive& archive, DEC_PopulationDecision* role, const unsigned int /*version*/ )
{
    MIL_Population* population;
    unsigned int gcPause;
    unsigned int gcMult;
    archive >> population
            >> gcPause
            >> gcMult;
    ::new( role )DEC_PopulationDecision( *population, gcPause, gcMult );
}

#endif // __DEC_PopulationDecision_h_
