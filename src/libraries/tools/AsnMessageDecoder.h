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

#include "asn/asn1per.h"
#include "asn/asn1PerCppTypes.h"
#include "AsnControllers.h"

namespace DIN
{
    class DIN_Input;
}

namespace tools
{
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
     AsnMessageDecoder( DIN::DIN_Input& dinMsg );
     AsnMessageDecoder( DIN::DIN_Input& dinMsg, unsigned long& nContext );
    ~AsnMessageDecoder();
    //@}

    //! @name Accessors
    //@{
    const T& GetAsnMsg() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    AsnMessageDecoder( const AsnMessageDecoder& );            //!< Copy constructor
    AsnMessageDecoder& operator=( const AsnMessageDecoder& ); //!< Assignement operator
    //@}

    //! @name Tools
    //@{
    void Decode( DIN::DIN_Input& dinMsg );
    void Dump  () const;
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
