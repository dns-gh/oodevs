//*****************************************************************************
//
// $Created: NLD 2002-08-06 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Orders/Limit/MIL_LimitManager.h $
// $Author: Nld $
// $Modtime: 19/01/05 16:42 $
// $Revision: 3 $
// $Workfile: MIL_LimitManager.h $
//
//*****************************************************************************

#ifndef __MIL_LimitManager_h_
#define __MIL_LimitManager_h_

#include "MIL.h"

#include "MIL_Limit.h"

//=============================================================================
// Created: NLD 2002-08-06
//=============================================================================
class MIL_LimitManager
{
    MT_COPYNOTALLOWED( MIL_LimitManager );

public:
     MIL_LimitManager();
    ~MIL_LimitManager();
    
    //! @name Network
    //@{
    void       SendStateToNewClient    ( NET_AS_MOSServer& connection ) const;
    MIL_Limit* OnReceiveMsgCreateLimit ( const ASN1T_MsgLimitCreation&    asnMsg, MIL_MOSContextID nCtx );
    void       OnReceiveMsgDestroyLimit( const ASN1T_MsgLimitDestruction& asnMsg, MIL_MOSContextID nCtx );
    void       OnReceiveMsgUpdateLimit ( const ASN1T_MsgLimitUpdate&      asnMsg, MIL_MOSContextID nCtx );
    //@}

    //! @name Limit management
    //@{
          bool       DestroyLimit( MIL_Limit& Limit );
          bool       DestroyLimit( uint nID );
          MIL_Limit& CreateLimit ( const T_PointVector& points );
    const MIL_Limit* FindLimit   ( uint nID ) const;
    //@}

private:
    //! @name Tools
    //@{
    MIL_Limit* FindLimit( const T_PointVector& points ) const;
    //@}

private:
    //! @name Types 
    //@{
    typedef std::map< uint, MIL_Limit* > T_LimitMap;
    typedef T_LimitMap::iterator         IT_LimitMap;
    typedef T_LimitMap::const_iterator   CIT_LimitMap;
    //@}

private:
    T_LimitMap limits_;
};

#include "MIL_LimitManager.inl"

#endif // __MIL_LimitManager_h_
