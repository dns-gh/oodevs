// *****************************************************************************
//
// $Created: NLD 2004-03-11 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_PopulationFlowPart.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_PopulationFlowPart.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_PopulationFlowPart_h_
#define __DEC_Knowledge_PopulationFlowPart_h_

#include "MIL.h"

class DEC_Knowledge_PopulationFlowPerception;
class DEC_Knowledge_PopulationCollision;
class PHY_PerceptionLevel;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationFlowPart : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationFlowPart();
    virtual ~DEC_Knowledge_PopulationFlowPart();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive& , const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}
    
    //! @name Operations
    //@{
    void Prepare        ();
    bool Update         ( const DEC_Knowledge_PopulationFlowPerception& perception );
    bool Update         ( const DEC_Knowledge_PopulationCollision&      collision  );
    bool UpdateRelevance( const MT_Float rMaxLifeTime );
    bool Clean          ();
    //@}

    //! @name Network operations
    //@{
    void Serialize( ASN1T_FlowPart& asn );
    //@}

private:
    //! @name Tools
    //@{
    bool ChangeRelevance( MT_Float rRelevance );
    //@}

private:
    T_PointVector   shape_;
    MT_Float        rRelevance_;
    MT_Float        rLastRelevanceSent_;
    bool            bPerceived_;
    uint            nTimeLastUpdate_;
};

#include "DEC_Knowledge_PopulationFlowPart.inl"

#endif // __DEC_Knowledge_PopulationFlowPart_h_
