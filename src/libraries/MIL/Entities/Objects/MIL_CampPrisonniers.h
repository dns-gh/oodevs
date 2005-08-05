//*****************************************************************************
//
// $Created: NLD 2002-12-12 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_CampPrisonniers.h $
// $Author: Jvt $
// $Modtime: 14/04/05 11:11 $
// $Revision: 5 $
// $Workfile: MIL_CampPrisonniers.h $
//
//*****************************************************************************

#ifndef __MIL_CampPrisonniers_h_
#define __MIL_CampPrisonniers_h_

#include "MIL.h"

#include "MIL_RealObject_ABC.h"

class MIL_AutomateLOG;

//=============================================================================
// Created: NLD 2002-12-12
// Last modified: JVT 03-07-31
//=============================================================================
class MIL_CampPrisonniers : public MIL_RealObject_ABC
{
    MT_COPYNOTALLOWED( MIL_CampPrisonniers );

public:
             MIL_CampPrisonniers();
    virtual ~MIL_CampPrisonniers();
 
    //! @name CheckPoints
    //@{
    template < typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Init
    //@{
    static MIL_RealObject_ABC& Create();

    virtual bool                      Initialize( MIL_Army& army, DIA_Parameters& diaParameters, uint& nCurrentParamIdx );
    virtual void                      Initialize( uint nID, MIL_InputArchive& archive );
    virtual ASN1T_EnumObjectErrorCode Initialize( uint nID, const ASN1T_MagicActionCreateObject& asn );
    //@}

    //! @name Accessors
    //@{
    MIL_AutomateLOG& GetTC2() const;
    //@}

private:
    //! @name Network
    //@{
    virtual void WriteSpecificAttributes( NET_ASN_MsgObjectCreation& asnMsg );
    //@}

    //! @name Events
    //@{
    virtual void ProcessAgentInside  ( MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting ( MIL_Agent_ABC& agent );
    //@}

private:
    MIL_AutomateLOG*                pTC2_;
    ASN1T_AttrObjectCampPrisonniers asnAttributes_;
};

#include "MIL_CampPrisonniers.inl"

#endif // __MIL_CampPrisonniers_h_
