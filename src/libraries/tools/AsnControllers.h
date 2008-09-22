// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AsnControllers_h_
#define __AsnControllers_h_

// =============================================================================
/** @class  AsnControllers
    @brief  AsnControllers
*/
// Created: AGE 2007-08-23
// =============================================================================
namespace tools
{
    template< typename T >
    struct AsnControllers
    {
        // NOTHING
    };
}

#define DECLARE_ASN_CONTROLLER( m )             \
    struct ASN1T_##m; class ASN1C_##m;          \
    template< >                                 \
    struct tools::AsnControllers< ASN1T_##m >   \
    {                                           \
        typedef ASN1C_##m C;                    \
    };

#endif // __AsnControllers_h_
