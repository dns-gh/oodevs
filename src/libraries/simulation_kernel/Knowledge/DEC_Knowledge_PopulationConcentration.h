// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationConcentration.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationConcentration.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationConcentration_h_
#define __DEC_Knowledge_PopulationConcentration_h_

#include "MIL.h"
#include "Tools/MIL_IDManager.h"

class DEC_Knowledge_Population;
class DEC_Knowledge_PopulationCollision;
class DEC_Knowledge_PopulationConcentrationPerception;
class MIL_PopulationConcentration;
class MIL_PopulationAttitude;
class PHY_PerceptionLevel;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationConcentration : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationConcentration( DEC_Knowledge_Population& populationKnowledge, const MIL_PopulationConcentration& concentrationKnown );
             DEC_Knowledge_PopulationConcentration( DEC_Knowledge_Population& populationKnowledge, const DEC_Knowledge_PopulationConcentration& );
             DEC_Knowledge_PopulationConcentration();
    virtual ~DEC_Knowledge_PopulationConcentration();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    void WriteKnowledges( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    void Update( const DEC_Knowledge_PopulationConcentrationPerception& perception );
    void Update( const DEC_Knowledge_PopulationCollision& collision  );
    void UpdateFromCrowdPerception( int currentTimeStep );
    void UpdateRelevance();
    bool Clean();
    void HackPerceptionLevel( const PHY_PerceptionLevel* pPerceptionLevel );
    double GetRelevance() const;
    MT_Vector2D GetPosition() const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork();
    void SendStateToNewClient();
    //@}

private:
    //! @name Network
    //@{
    void SendFullState();
    void SendMsgCreation() const;
    void SendMsgDestruction() const;
    //@}

    //! @name Tools
    //@{
    void ChangeRelevance( double rNewRelevance );
    //@}

private:
    //! @name  Member data
    //@{
    const DEC_Knowledge_Population* pPopulationKnowledge_;
    const MIL_PopulationConcentration* pConcentrationKnown_;
    const unsigned int nID_;
    unsigned int nTimeLastUpdate_;
    MT_Vector2D position_;
    double rRelevance_;
    const MIL_PopulationAttitude* pAttitude_;
    unsigned int nNbrAliveHumans_;
    unsigned int nNbrDeadHumans_;
    const PHY_PerceptionLevel* pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel* pPreviousPerceptionLevel_;
    const PHY_PerceptionLevel* pHackedPerceptionLevel_;
    bool bReconAttributesValid_;
    // Network
    bool bHumansUpdated_;
    bool bAttitudeUpdated_;
    bool bRealConcentrationUpdated_;
    bool bRelevanceUpdated_;
    double rLastRelevanceSent_;
    static MIL_IDManager idManager_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_PopulationConcentration )

#endif // __DEC_Knowledge_PopulationConcentration_h_
