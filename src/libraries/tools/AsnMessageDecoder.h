// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AsnMessageDecoder_h_
#define __AsnMessageDecoder_h_

#include <asn1per.h> // $$$$ _RC_ CPG 2008-09-22: Remplacer par #include <masan/asn1per.h> ?
#include <asn1PerCppTypes.h>
#include "AsnControllers.h"

namespace tools
{
    class Message;

// =============================================================================
/** @class  AsnMessageDecoder
    @brief  AsnMessageDecoder
*/
// Created: NLD 2006-09-19
// =============================================================================
template< typename T, typename C = typename AsnControllers< T >::C >
class AsnMessageDecoder
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit AsnMessageDecoder( Message& message );
    //@}

    //! @name Accessors
    //@{
    operator const T&() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AsnMessageDecoder( const AsnMessageDecoder& );            //!< Copy constructor
    AsnMessageDecoder& operator=( const AsnMessageDecoder& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void Decode( Message& message );
    //@}

private:
    ASN1OCTET           aAsnBuffer_[ 100000 ];
    ASN1PERDecodeBuffer asnBuffer_;
    T                   asnMsg_;
    C                   asnMsgCtrl_;
};

}

#include "AsnMessageDecoder.inl"

#endif // __AsnMessageDecoder_h_
