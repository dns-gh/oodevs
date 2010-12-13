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
#include "MT_Tools/MT_Vector2DTypes.h"

namespace sword
{
    class MsgFlowPart;
}

class DEC_Knowledge_PopulationFlowPerception;
class DEC_Knowledge_PopulationCollision;
class PHY_PerceptionLevel;

// =============================================================================
// Created: NLD 2004-03-11
// =============================================================================
class DEC_Knowledge_PopulationFlowPart
{
public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_PopulationFlowPart();
             DEC_Knowledge_PopulationFlowPart( const DEC_Knowledge_PopulationFlowPart& part );
    virtual ~DEC_Knowledge_PopulationFlowPart();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()

    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    void Prepare();
    bool Update( const DEC_Knowledge_PopulationFlowPerception& perception );
    bool Update( const DEC_Knowledge_PopulationCollision& collision );
    bool UpdateRelevance( const double rMaxLifeTime );
    bool Clean();
    //@}

    //! @name Network operations
    //@{
    void Serialize( sword::MsgFlowPart& asn );
    //@}

private:
    //! @name Tools
    //@{
    bool ChangeRelevance( double rRelevance );
    //@}

private:
    //! @name Member data
    //@{
    T_PointVector shape_;
    double rRelevance_;
    double rLastRelevanceSent_;
    bool bPerceived_;
    unsigned int nTimeLastUpdate_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DEC_Knowledge_PopulationFlowPart )

#endif // __DEC_Knowledge_PopulationFlowPart_h_
