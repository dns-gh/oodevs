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
             DEC_Knowledge_PopulationConcentration();
    virtual ~DEC_Knowledge_PopulationConcentration();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Prepare        ();
    void Update         ( const DEC_Knowledge_PopulationConcentrationPerception& perception );
    void Update         ( const DEC_Knowledge_PopulationCollision&               collision  );
    void UpdateRelevance();
    bool Clean          ();
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     ();
    void SendStateToNewClient();
    //@}

private:
    //! @name Network
    //@{
    void SendFullState     ();
    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    //@}

    //! @name Tools
    //@{
    void ChangeRelevance( MT_Float rNewRelevance );
    //@}

private:
    const DEC_Knowledge_Population*     pPopulationKnowledge_;
    const MIL_PopulationConcentration*  pConcentrationKnown_;
    const uint                          nID_;

          uint                          nTimeLastUpdate_;
          MT_Vector2D                   position_;
          MT_Float                      rRelevance_;

          bool                          bReconAttributesValid_;
    const MIL_PopulationAttitude*       pAttitude_;
          MT_Float                      rNbrAliveHumans_;
          MT_Float                      rNbrDeadHumans_;

    const PHY_PerceptionLevel*          pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel*          pPreviousPerceptionLevel_;

    // Network
    bool     bHumansUpdated_;
    bool     bAttitudeUpdated_;
    bool     bRealConcentrationUpdated_;
    bool     bRelevanceUpdated_;
    MT_Float rLastRelevanceSent_;
};

#endif // __DEC_Knowledge_PopulationConcentration_h_
