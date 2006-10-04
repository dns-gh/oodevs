//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Lima/MIL_LimaManager.h $
// $Author: Nld $
// $Modtime: 19/01/05 16:42 $
// $Revision: 3 $
// $Workfile: MIL_LimaManager.h $
//
//*****************************************************************************

#ifndef __MIL_LimaManager_h_
#define __MIL_LimaManager_h_

#include "MIL.h"

#include "MIL_Lima.h"
#include "Network/NET_ASN_Types.h"

//=============================================================================
// Created: NLD 2002-08-06
//=============================================================================
class MIL_LimaManager
{
    MT_COPYNOTALLOWED( MIL_LimaManager );

public:
    //-------------------------------------------------------------------------
    /** @name Types */
    //-------------------------------------------------------------------------
    //@{
    typedef std::map< uint, MIL_Lima* > T_LimaMap;
    typedef T_LimaMap::iterator         IT_LimaMap;
    typedef T_LimaMap::const_iterator   CIT_LimaMap;
    //@}

public:
     MIL_LimaManager();
    ~MIL_LimaManager();

    //! @name Network
    //@{
    void      SendStateToNewClient   () const;
    MIL_Lima* OnReceiveMsgCreateLima ( const ASN1T_MsgLimaCreation&    asnMsg, MIL_MOSContextID nCtx );
    void      OnReceiveMsgDestroyLima( const ASN1T_MsgLimaDestruction& asnMsg, MIL_MOSContextID nCtx );
    void      OnReceiveMsgUpdateLima ( const ASN1T_MsgLimaUpdate&      asnMsg, MIL_MOSContextID nCtx );
    //@}

    //! @name Lima management
    //@{
    bool      DestroyLima( MIL_Lima& Lima );
    bool      DestroyLima( uint nID );

    const MIL_Lima* FindLima( uint nID ) const;
    //@}

private:
    T_LimaMap limaMap_;
};

#   include "MIL_LimaManager.inl"
#endif // __MIL_LimaManager_h_
