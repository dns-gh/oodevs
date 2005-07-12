// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectNuageNBC.h $
// $Author: Jvt $
// $Modtime: 30/03/05 11:15 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_ObjectNuageNBC.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectNuageNBC_h_
#define __DEC_Knowledge_ObjectNuageNBC_h_

#include "MIL.h"

#include "DEC_Knowledge_Object.h"

class MIL_NbcAgentType;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectNuageNBC : public DEC_Knowledge_Object
{
    MT_COPYNOTALLOWED( DEC_Knowledge_ObjectNuageNBC ) 

public:
    //! @name Constructors/Destructor
    //@{
    DEC_Knowledge_ObjectNuageNBC( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown );
    DEC_Knowledge_ObjectNuageNBC();
    virtual ~DEC_Knowledge_ObjectNuageNBC();
    //@}
    
    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
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
    ASN1T_AttrObjectNuageNBC asnAttributes_;

    const MIL_NbcAgentType* pNbcAgentType_;
    bool                    bSpecificAttributesValid_;
};

#include "DEC_Knowledge_ObjectNuageNBC.inl"

#endif // __DEC_Knowledge_ObjectNuageNBC_h_
