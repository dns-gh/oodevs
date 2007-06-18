// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectZoneMineeLineaire.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:10 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectZoneMineeLineaire.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectZoneMineeLineaire_h_
#define __DEC_Knowledge_ObjectZoneMineeLineaire_h_

#include "MIL.h"

#include "DEC_Knowledge_Object.h"

class MIL_AutomateLOG;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectZoneMineeLineaire : public DEC_Knowledge_Object
{
    MT_COPYNOTALLOWED( DEC_Knowledge_ObjectZoneMineeLineaire ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectZoneMineeLineaire( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown );
             DEC_Knowledge_ObjectZoneMineeLineaire();
    virtual ~DEC_Knowledge_ObjectZoneMineeLineaire();
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

    ASN1T_ObjectAttributesLinearMineArea asnAttributes_;
    uint                              nMinesActivityTime_;
    MT_Float                          rMinesDensity_;
};

#include "DEC_Knowledge_ObjectZoneMineeLineaire.inl"

#endif // __DEC_Knowledge_ObjectZoneMineeLineaire_h_
