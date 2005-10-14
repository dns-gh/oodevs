// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlowPerception.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationFlowPerception.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationFlowPerception_h_
#define __DEC_Knowledge_PopulationFlowPerception_h_

#include "MIL.h"

class DEC_Knowledge_PopulationPerception;
class MIL_PopulationFlow;
class PHY_PerceptionLevel;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationFlowPerception
{
    MT_COPYNOTALLOWED( DEC_Knowledge_PopulationFlowPerception ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationFlowPerception( DEC_Knowledge_PopulationPerception& populationKnowledge, MIL_PopulationFlow& flowPerceived );
             DEC_Knowledge_PopulationFlowPerception();
    virtual ~DEC_Knowledge_PopulationFlowPerception();
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
    void Update ( const PHY_PerceptionLevel& level, const T_PointVector& shape );
    bool Clean  ();

    bool IsIdentified();
    //@}

    //! @name Accessors
    //@{
    const MIL_PopulationFlow& GetFlowPerceived() const;
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     () const;
    void SendStateToNewClient() const;
    //@}

private:
    const DEC_Knowledge_PopulationPerception*  pPopulationKnowledge_;
          MIL_PopulationFlow*                  pPopulationFlowPerceived_;
          T_PointVector                        shape_;
          T_PointVector                        previousShape_;
          
};

#include "DEC_Knowledge_PopulationFlowPerception.inl"

#endif // __DEC_Knowledge_PopulationFlowPerception_h_
