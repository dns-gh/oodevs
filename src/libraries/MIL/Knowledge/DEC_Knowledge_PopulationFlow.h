// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlow.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationFlow.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationFlow_h_
#define __DEC_Knowledge_PopulationFlow_h_

#include "MIL.h"

class DEC_Knowledge_Population;
class MIL_PopulationFlow;
class PHY_Level;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationFlow
{
    MT_COPYNOTALLOWED( DEC_Knowledge_PopulationFlow ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationFlow( DEC_Knowledge_Population& populationKnowledge, MIL_PopulationFlow& flowPerceived );
             DEC_Knowledge_PopulationFlow();
    virtual ~DEC_Knowledge_PopulationFlow();
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
    void Update ( const PHY_Level& level, const T_PointVector& shape );
    bool Clean  ();

    bool IsIdentified();
    //@}

    //! @name Network operations
    //@{
    void UpdateOnNetwork     () const;
    void SendStateToNewClient() const;
    //@}

private:
    const DEC_Knowledge_Population*  pPopulationKnowledge_;
          MIL_PopulationFlow*                  pPopulationFlowPerceived_;
          T_PointVector                        shape_;
          T_PointVector                        previousShape_;
          
};

#include "DEC_Knowledge_PopulationFlow.inl"

#endif // __DEC_Knowledge_PopulationFlow_h_
