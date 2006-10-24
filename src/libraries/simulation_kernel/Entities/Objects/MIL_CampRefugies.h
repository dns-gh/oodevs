//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_CampRefugies.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:11 $
// $Revision: 4 $
// $Workfile: MIL_CampRefugies.h $
//
//*****************************************************************************

#ifndef __MIL_CampRefugies_h_
#define __MIL_CampRefugies_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_AutomateLOG;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_CampRefugies : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_CampRefugies );

public:
             MIL_CampRefugies( const MIL_RealObjectType& type, uint nID, MIL_Army& army );
             MIL_CampRefugies();
    virtual ~MIL_CampRefugies();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create( const MIL_RealObjectType& type, uint nID, MIL_Army& army );

    virtual bool                      Initialize( DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( const ASN1T_MagicActionCreateObject& asn );
    //@}

    //! @name Knowledge
    //@{
    virtual DEC_Knowledge_Object& CreateKnowledge( const MIL_Army& teamKnowing );
    //@}

    //! @name Accessors
    //@{
    MIL_AutomateLOG& GetTC2() const;
    //@}

private:
    //! @name Network
    //@{
    virtual void WriteSpecificAttributes( MT_XXmlOutputArchive& archive ) const;
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg );
    //@}

    //! @name Events
    //@{
    virtual void ProcessAgentInside  ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting ( MIL_Agent_ABC& agent );
    //@}

private:
    MIL_AutomateLOG*             pTC2_;
    ASN1T_AttrObjectCampRefugies asnAttributes_;
};

#include "MIL_CampRefugies.inl"

#endif // __MIL_CampRefugies_h_
