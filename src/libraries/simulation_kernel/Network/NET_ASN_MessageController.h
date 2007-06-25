// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __NET_ASN_MessageController_h_
#define __NET_ASN_MessageController_h_

#include "DIN/DIN_Link.h"
#include <ASN/asn1PerCppTypes.h>

// =============================================================================
/** @class  NET_ASN_MessageController
    @brief  Encapsulates the asn buffer, message and controller classes to handle
            incoming messages, decode and keep them before treatment. 
            Templatized on the asn types and classes.
            Use the macro to quickly declare a new one.
*/
// Created: AGE 2004-02-05
// =============================================================================
template< typename ASN1Type, typename ASN1Class > 
class NET_ASN_MessageController
{
    MT_COPYNOTALLOWED( NET_ASN_MessageController );
    
public:
    void* operator new( size_t size );
    void operator delete( void* ptr );
private:
    static MT_ObjectPool< NET_ASN_MessageController< ASN1Type, ASN1Class > > pool_;
    
public:
    //! @name Constructors/Destructor
    //@{
             NET_ASN_MessageController( uint8* pBuffer, unsigned long nBufferSize );
    virtual ~NET_ASN_MessageController();
    //@}

    //! @name Main Methods
    //@{
    int  Decode();
    void Print( const char* szName );
    void PrintErrorInfo();
    void SetTrace( bool bEnable = true );
    void SetLink( DIN::DIN_Link& link );
    //@}

    //! @name Accessors
    //@{
    ASN1Type& GetMessage();
    DIN::DIN_Link* GetLink();
    //@}
private:
    //! @name Member data
    //@{
    ASN1PERDecodeBuffer decodeBuffer_;
    ASN1Type  asnMsg_;
    ASN1Class asnMsgCtrl_;
    DIN::DIN_Link* pLink_;
    //@}
    
};

#define NET_ASN_GENERATE_MESSAGE_CONTROLLER( ASNTYPE )      \
    typedef NET_ASN_MessageController< ASN1T_##ASNTYPE, ASN1C_##ASNTYPE > NET_ASN_##ASNTYPE##Controller;

#include "NET_ASN_MessageController.inl"

#endif // __NET_ASN_MessageController_h_
