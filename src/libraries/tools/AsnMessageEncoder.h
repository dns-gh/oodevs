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

#include <asn1per.h>
#include <asn1PerCppTypes.h>
#include "AsnControllers.h"

namespace tools 
{
    class Message;

// =============================================================================
/** @class  AsnMessageEncoder
    @brief  AsnMessageEncoder
*/
// Created: NLD 2006-09-19
// =============================================================================
template< typename T, typename C = typename AsnControllers< T >::C >
class AsnMessageEncoder
{
public:
    //! @name Constructors/Destructor
    //@{
     explicit AsnMessageEncoder( const T& asnMsg );
    //@}

    //! @name Accessors
    //@{
    operator const Message&() const;
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
    //@}

private:
    ASN1OCTET           aAsnBuffer_[ 100000 ];
    ASN1PEREncodeBuffer asnBuffer_;
    C                   asnMsgCtrl_;
    Message             message_;
};

}

#include "AsnMessageEncoder.inl"

#endif // __AsnMessageEncoder_h_
