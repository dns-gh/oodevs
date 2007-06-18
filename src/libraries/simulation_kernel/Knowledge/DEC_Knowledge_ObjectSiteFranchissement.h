// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectSiteFranchissement.h $
// $Author: Jvt $
// $Modtime: 25/03/05 15:46 $
// $Revision: 5 $
// $Workfile: DEC_Knowledge_ObjectSiteFranchissement.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectSiteFranchissement_h_
#define __DEC_Knowledge_ObjectSiteFranchissement_h_

#include "MIL.h"

#include "DEC_Knowledge_Object.h"

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectSiteFranchissement : public DEC_Knowledge_Object
{
    MT_COPYNOTALLOWED( DEC_Knowledge_ObjectSiteFranchissement ) 

public:
    //! @name Constructors/Destructor
    //@{
    DEC_Knowledge_ObjectSiteFranchissement( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown );
    DEC_Knowledge_ObjectSiteFranchissement();
    virtual ~DEC_Knowledge_ObjectSiteFranchissement();
    //@}
    
    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    //@}
    
    //! @name Accessors
    //@{
    MT_Float GetWidth() const;
	bool	 IsBanksToFitOut() const;
    //@}

private:
    //! @name Update
    //@{
    virtual void UpdateSpecificAttributes( const PHY_PerceptionLevel& currentPerceptionLevel );
    virtual void UpdateSpecificAttributes( const DEC_Knowledge_ObjectPerception& perception );
    virtual void UpdateSpecificAttributes( const DEC_Knowledge_ObjectCollision&  collision  );

    virtual void BuildMsgSpecificAttributes( ASN1T_MsgObjectKnowledgeUpdate& asnMsg );

    void UpdateSpecificAttributes  ();
    //@}

private:
    // Specific attributes
    ASN1T_ObjectAttributesCrossingSite asnAttributes_;

    MT_Float rWidth_;
    MT_Float rDepth_;
    MT_Float rCurrentSpeed_;  // Vitesse du courant
    bool     bBanksToFitOut_; // Berges a amenager

    bool     bSpecificAttributesValid_;
};

#include "DEC_Knowledge_ObjectSiteFranchissement.inl"

#endif // __DEC_Knowledge_ObjectSiteFranchissement_h_
