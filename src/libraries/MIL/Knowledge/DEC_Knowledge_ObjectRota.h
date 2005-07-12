// *****************************************************************************
//
// $Created: NLD 2004-05-04 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Knowledge/DEC_Knowledge_ObjectRota.h $
// $Author: Nld $
// $Modtime: 26/04/05 12:07 $
// $Revision: 4 $
// $Workfile: DEC_Knowledge_ObjectRota.h $
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectRota_h_
#define __DEC_Knowledge_ObjectRota_h_

#include "MIL.h"

#include "DEC_Knowledge_Object.h"

class MIL_NbcAgentType;

// =============================================================================
// Created: NLD 2004-05-04
// =============================================================================
class DEC_Knowledge_ObjectRota : public DEC_Knowledge_Object
{
    MT_COPYNOTALLOWED( DEC_Knowledge_ObjectRota ) 

public:
    //! @name Constructors/Destructor
    //@{
             DEC_Knowledge_ObjectRota( const MIL_Army& armyKnowing, MIL_RealObject_ABC& objectKnown );
             DEC_Knowledge_ObjectRota();
    virtual ~DEC_Knowledge_ObjectRota();
    //@}
    
    //! @name Serialization
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
            void UpdateSpecificAttributes  ();
            void CleanMsgSpecificAttributes();
    //@}

public:
    //! @name Types
    //@{
    typedef std::set< const MIL_NbcAgentType* > T_NbcAgentSet;
    typedef T_NbcAgentSet::const_iterator       CIT_NbcAgentSet;
    //@}

private:
    T_NbcAgentSet nbcAgents_;
    uint          nDanger_;

    // Specific attributes
    ASN1T_AttrObjectROTA asnAttributes_;

    bool                 bSpecificAttributesValid_;
};

#include "DEC_Knowledge_ObjectRota.inl"

#endif // __DEC_Knowledge_ObjectRota_h_
