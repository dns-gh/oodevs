// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AsnMessageEncoder_h_
#define __AsnMessageEncoder_h_

#include "asn1per.h"
#include "asn1PerCppTypes.h"
#include "DIN/MessageService/DIN_BufferedMessage.h"

namespace DIN
{
    class DIN_MessageService_ABC;
}

namespace network 
{
// =============================================================================
/** @class  AsnMessageEncoder
    @brief  AsnMessageEncoder
*/
// Created: NLD 2006-09-19
// =============================================================================
template< typename T, typename C >
class AsnMessageEncoder
{
public:
    //! @name Constructors/Destructor
    //@{
     AsnMessageEncoder( DIN::DIN_MessageService_ABC& msgService, const T& asnMsg );
     AsnMessageEncoder( DIN::DIN_MessageService_ABC& msgService, const T& asnMsg, unsigned long nContext );
    ~AsnMessageEncoder();
    //@}

    //! @name Accessors
    //@{
    const DIN::DIN_BufferedMessage& GetDinMsg() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AsnMessageEncoder( const AsnMessageEncoder& );            //!< Copy constructor
    AsnMessageEncoder& operator=( const AsnMessageEncoder& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void Encode();
    void Dump  () const;
    //@}

private:
    ASN1OCTET                aAsnBuffer_[ 100000 ];
    ASN1PEREncodeBuffer      asnBuffer_;
    C                        asnMsgCtrl_;
    DIN::DIN_BufferedMessage dinMsg_;
};

}

#include "AsnMessageEncoder.inl"

#endif // __AsnMessageEncoder_h_
