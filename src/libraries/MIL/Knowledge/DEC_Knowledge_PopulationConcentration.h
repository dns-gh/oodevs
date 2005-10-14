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

#include "Tools/MIL_MOSIDManager.h"

class DEC_Knowledge_Population;
class DEC_Knowledge_PopulationConcentrationPerception;
class MIL_PopulationConcentration;
class MIL_PopulationAttitude;
class PHY_PerceptionLevel;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationConcentration
{
    MT_COPYNOTALLOWED( DEC_Knowledge_PopulationConcentration ) 

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
    void UpdateRelevance();
    bool Clean          ();
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     () const;
    void SendStateToNewClient() const;
    //@}

private:
    //! @name Network
    //@{
    void SendFullState     () const;
    void SendMsgCreation   () const;
    void SendMsgDestruction() const;
    //@}

private:
    const DEC_Knowledge_Population*     pPopulationKnowledge_;
    const MIL_PopulationConcentration*  pConcentrationKnown_;
    const uint                          nID_;

          MT_Vector2D                   position_;
          uint                          nNbrAliveHumans_;
          uint                          nNbrDeadHumans_;
    const MIL_PopulationAttitude*       pAttitude_;
          MT_Float                      rRelevance_;

    const PHY_PerceptionLevel*          pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel*          pPreviousPerceptionLevel_;

    // Network
    bool bHumansUpdated_;
    bool bAttitudeUpdated_;
    bool bRealConcentrationUpdated_;
    bool bRelevanceUpdated_;

private:
    static MIL_MOSIDManager idManager_;
};

#include "DEC_Knowledge_PopulationConcentration.inl"

#endif // __DEC_Knowledge_PopulationConcentration_h_
