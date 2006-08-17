// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Entities/Objects/MIL_ObjectManager.h $
// $Author: Nld $
// $Modtime: 10/06/05 10:18 $
// $Revision: 6 $
// $Workfile: MIL_ObjectManager.h $
//
// *****************************************************************************

#ifndef __MIL_ObjectManager_h_
#define __MIL_ObjectManager_h_

#include "MIL.h"

#include "Network/NET_ASN_Types.h"

class NET_AS_MOSServer;
class MIL_Army;
class MIL_RealObjectType;
class TER_Localisation;
class MIL_VirtualObject_ABC;
class MIL_RealObject_ABC;

// =============================================================================
// @class  MIL_ObjectManager
// Created: JVT 2004-08-03
// =============================================================================
class MIL_ObjectManager
{
    MT_COPYNOTALLOWED( MIL_ObjectManager )

public:
     MIL_ObjectManager();
    ~MIL_ObjectManager();

    //! @name Init
    //@{
    void ReadODB( MIL_InputArchive& archive );
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;

    void WriteODB( MT_XXmlOutputArchive& archive ) const;
    //@}
    
    //! @name Operations
    //@{
    void ProcessEvents();
    void UpdateStates ();

    MIL_RealObject_ABC* CreateObject( MIL_Army& army, DIA_Parameters& diaParameters, uint nCurrentParamIdx );
//    MIL_RealObject_ABC* CreateObject( MIL_Army& army, const MIL_RealObjectType& objectType, const TER_Localisation& localisation, MT_Float rParameter );

    void                RegisterObject( MIL_VirtualObject_ABC& object );
    void                RegisterObject( MIL_RealObject_ABC& object    );
    MIL_RealObject_ABC* FindRealObject( uint nID ) const;
    //@}

    //! @name Network
    //@{
    void SendStateToNewClient         ();
    void OnReceiveMsgObjectMagicAction( ASN1T_MsgObjectMagicAction& asnMsg, MIL_MOSContextID nCtx );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< uint, MIL_RealObject_ABC* > T_RealObjectMap;
    typedef T_RealObjectMap::iterator             IT_RealObjectMap;
    typedef T_RealObjectMap::const_iterator       CIT_RealObjectMap;

    typedef std::vector< MIL_VirtualObject_ABC* > T_VirtualObjectVector;
    typedef T_VirtualObjectVector::iterator       IT_VirtualObjectVector;
    typedef T_VirtualObjectVector::const_iterator CIT_VirtualObjectVector;
    //@}

private:
    //! @name Tools
    //@{
    ASN1T_EnumObjectErrorCode CreateObject( uint nID, const ASN1T_MagicActionCreateObject& asn );
    void                      CreateObject( MIL_InputArchive& archive );
    //@}

private:
    T_RealObjectMap       realObjects_;
    T_VirtualObjectVector virtualObjects_;
};

#include "MIL_ObjectManager.inl"

#endif // __MIL_ObjectManager_h_
