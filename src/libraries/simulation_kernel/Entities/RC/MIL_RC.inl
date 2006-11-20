// *****************************************************************************
//
// $Created: JVT 2004-08-03 $
// $Archive: /MVW_v10/Build/SDK/MIL/Src/Entities/Rc/MIL_RC.inl $
// $Author: Nld $
// $Modtime: 14/03/05 18:02 $
// $Revision: 4 $
// $Workfile: MIL_RC.inl $
//
// *****************************************************************************

#include "Network/NET_ASN_Messages.h"

// -----------------------------------------------------------------------------
// Name: MIL_RC::FillRcMessage
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
template< typename T > 
void MIL_RC::FillRcMessage( ASN1T_MsgCR& asnMsg, T& sender, E_RcType nType ) const
{
    assert( nAsnID_ != -1 );
    asnMsg.unit_id  = sender.GetID();
    asnMsg.cr.t     = nAsnID_;
    asnMsg.type_cr  = (ASN1T_EnumTypeCR)nType;
}

// -----------------------------------------------------------------------------
// Name: MIL_RC::GetID
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
uint MIL_RC::GetID() const
{
    return nRC_;
}

// -----------------------------------------------------------------------------
// Name: MIL_RC::Find
// Created: NLD 2004-08-05
// -----------------------------------------------------------------------------
inline
const MIL_RC* MIL_RC::Find( uint nID )
{
    CIT_RCMap it = rcs_.find( nID );
    if( it == rcs_.end() )
        return 0;
    return it->second;
}

// -----------------------------------------------------------------------------
// Name: template < typename T > void MIL_RC::Send
// Created: NLD 2004-10-18
// -----------------------------------------------------------------------------
template < typename T > 
void MIL_RC::Send( const T& sender, E_RcType nType ) const
{
    NET_ASN_MsgCR asnMsg;
    FillRcMessage( asnMsg.GetAsnMsg(), sender, nType );
    asnMsg.Send();
}
