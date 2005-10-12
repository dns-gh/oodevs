// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationConcentrationPerception.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationConcentrationPerception.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationConcentrationPerception_h_
#define __DEC_Knowledge_PopulationConcentrationPerception_h_

#include "MIL.h"

class DEC_Knowledge_PopulationPerception;
class MIL_PopulationConcentration;
class PHY_PerceptionLevel;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationConcentrationPerception
{
    MT_COPYNOTALLOWED( DEC_Knowledge_PopulationConcentrationPerception ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationConcentrationPerception( DEC_Knowledge_PopulationPerception& populationKnowledge, MIL_PopulationConcentration& concentrationPerceived );
             DEC_Knowledge_PopulationConcentrationPerception();
    virtual ~DEC_Knowledge_PopulationConcentrationPerception();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Prepare();
    void Update ( const PHY_PerceptionLevel& level );
    bool Clean  ();

    bool IsIdentified();
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     () const;
    void SendStateToNewClient() const;
    //@}

private:
    const DEC_Knowledge_PopulationPerception*  pPopulationKnowledge_;
          MIL_PopulationConcentration*         pPopulationConcentrationPerceived_;

    const PHY_PerceptionLevel*                 pCurrentPerceptionLevel_;
    const PHY_PerceptionLevel*                 pPreviousPerceptionLevel_;
};

#include "DEC_Knowledge_PopulationConcentrationPerception.inl"

#endif // __DEC_Knowledge_PopulationConcentrationPerception_h_
