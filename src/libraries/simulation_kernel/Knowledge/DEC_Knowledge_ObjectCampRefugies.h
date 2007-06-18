// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectCampRefugies.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:10 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectCampRefugies.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectCampRefugies_h_
#define __DEC_Knowledge_ObjectCampRefugies_h_

#include "MIL.h"

#include "DEC_Knowledge_Object.h"

class MIL_AutomateLOG;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectCampRefugies : public DEC_Knowledge_Object
{
    MT_COPYNOTALLOWED( DEC_Knowledge_ObjectCampRefugies ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectCampRefugies( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown );
             DEC_Knowledge_ObjectCampRefugies();
    virtual ~DEC_Knowledge_ObjectCampRefugies();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
private:
    //! @name Update
    //@{
    virtual void UpdateSpecificAttributes( const PHY_PerceptionLevel& currentPerceptionLevel );
    virtual void UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision&  collision  );

    virtual void BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg );

    void UpdateSpecificAttributes();
    //@}

private:
    // Specific attributes
    bool bSpecificAttributesValid_;

    ASN1T_ObjectAttributesRefugeeCamp asnAttributes_;
    MIL_AutomateLOG*             pTC2_;
};

#include "DEC_Knowledge_ObjectCampRefugies.inl"

#endif // __DEC_Knowledge_ObjectCampRefugies_h_
