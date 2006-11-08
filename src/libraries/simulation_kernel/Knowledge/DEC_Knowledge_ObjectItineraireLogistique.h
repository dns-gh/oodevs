// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectItineraireLogistique.h $
// $Author: Nld $
// $Modtime: 27/04/05 15:10 $
// $Revision: 3 $
// $Workfile: DEC_Knowledge_ObjectItineraireLogistique.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectItineraireLogistique_h_
#define __DEC_Knowledge_ObjectItineraireLogistique_h_

#include "MIL.h"

#include "DEC_Knowledge_Object.h"

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectItineraireLogistique : public DEC_Knowledge_Object
{
    MT_COPYNOTALLOWED( DEC_Knowledge_ObjectItineraireLogistique ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectItineraireLogistique( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown );
             DEC_Knowledge_ObjectItineraireLogistique();
    virtual ~DEC_Knowledge_ObjectItineraireLogistique();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Accessors
    //@{
    bool IsEquipped() const;
    //@}

private:
    //! @name Update
    //@{
    virtual void UpdateSpecificAttributes( const PHY_PerceptionLevel& currentPerceptionLevel );
    virtual void UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision&  collision  );

    virtual void BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg );

    void UpdateSpecificAttributes( bool bRecognized );
    //@}

private:
    // Specific attributes
    ASN1T_AttrObjectItineraireLogistique asnAttributes_;

    bool     bEquipped_;
    MT_Float rWeightSupported_;
    MT_Float rWidth_;
    MT_Float rLength_;
    MT_Float rFlow_;
};

#include "DEC_Knowledge_ObjectItineraireLogistique.inl"

#endif // __DEC_Knowledge_ObjectItineraireLogistique_h_
