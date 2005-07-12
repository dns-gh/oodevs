// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGE 2004-02-05 $
// $Archive: /MVW_v10/Build/SDK/MIL/src/Network/NET_ASN_MessageController.inl $
// $Author: Age $
// $Modtime: 5/02/04 15:55 $
// $Revision: 2 $
// $Workfile: NET_ASN_MessageController.inl $
//
// *****************************************************************************

// -----------------------------------------------------------------------------
// Name: NET_ASN_MessageController constructor
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
template< typename ASN1Type, typename ASN1Class > 
NET_ASN_MessageController< ASN1Type, ASN1Class >::NET_ASN_MessageController( uint8* pBuffer, unsigned long nBufferSize  )
    : decodeBuffer_( pBuffer, nBufferSize, TRUE )
    , asnMsg_()
    , asnMsgCtrl_( decodeBuffer_, asnMsg_ )
    , pLink_( 0 )
{
    //NOTHING
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_MessageController destructor
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
template< typename ASN1Type, typename ASN1Class > 
NET_ASN_MessageController< ASN1Type, ASN1Class >::~NET_ASN_MessageController()
{
    //NOTHING
}

template< typename ASN1Type, typename ASN1Class > 
MT_ObjectPool< NET_ASN_MessageController< ASN1Type, ASN1Class > > NET_ASN_MessageController< ASN1Type, ASN1Class >::pool_;

template< typename ASN1Type, typename ASN1Class >
inline
int NET_ASN_MessageController< ASN1Type, ASN1Class >::Decode()
{

    return asnMsgCtrl_.Decode();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_MessageController::GetMessage
/** @return
*/
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
template< typename ASN1Type, typename ASN1Class >
inline
ASN1Type& NET_ASN_MessageController< ASN1Type, ASN1Class >::GetMessage()
{
    return asnMsg_;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_MessageController::Print
/** @param  szName
*/
// Created: AGE 2004-02-03
// -----------------------------------------------------------------------------
template< typename ASN1Type, typename ASN1Class >
inline
void NET_ASN_MessageController< ASN1Type, ASN1Class >::Print( const char* szName )
{
    asnMsgCtrl_.Print( szName );
}


// -----------------------------------------------------------------------------
// Name: NET_ASN_MessageController::PrintErrorInfo
// Created: AGE 2004-02-04
// -----------------------------------------------------------------------------
template< typename ASN1Type, typename ASN1Class >
inline
void NET_ASN_MessageController< ASN1Type, ASN1Class >::PrintErrorInfo()
{
    decodeBuffer_.PrintErrorInfo();
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_MessageController::SetTrace
/** @param  bEnable
*/
// Created: AGE 2004-02-04
// -----------------------------------------------------------------------------
template< typename ASN1Type, typename ASN1Class >
inline
void NET_ASN_MessageController< ASN1Type, ASN1Class >::SetTrace( bool bEnable /*= true*/ )
{
    decodeBuffer_.SetTrace( bEnable );
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_MessageController::SetLink
/** @param  link 
*/
// Created: AGE 2004-02-05
// -----------------------------------------------------------------------------
template< typename ASN1Type, typename ASN1Class >
inline
void NET_ASN_MessageController< ASN1Type, ASN1Class >::SetLink( DIN::DIN_Link& link )
{
    pLink_ = &link;
}

// -----------------------------------------------------------------------------
// Name: NET_ASN_MessageController::GetLink
/** @return 
*/
// Created: AGE 2004-02-05
// -----------------------------------------------------------------------------
template< typename ASN1Type, typename ASN1Class >
inline
DIN::DIN_Link* NET_ASN_MessageController< ASN1Type, ASN1Class >::GetLink()
{
    return pLink_;
}


template< typename ASN1Type, typename ASN1Class >
inline
void* NET_ASN_MessageController< ASN1Type, ASN1Class >::operator new( size_t p_size )
{
    return pool_.New( p_size );
}

template< typename ASN1Type, typename ASN1Class >
inline
void NET_ASN_MessageController< ASN1Type, ASN1Class >::operator delete( void* p_ptr )
{
    pool_.Delete( p_ptr, sizeof( NET_ASN_MessageController< ASN1Type, ASN1Class > ) );
}
