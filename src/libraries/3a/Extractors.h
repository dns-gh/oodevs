// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Extractors_h_
#define __Extractors_h_

// =============================================================================
/** @namespace Extractors
    @brief     Extractors
*/
// Created: AGE 2007-08-30
// =============================================================================
namespace extractors
{
    template< typename T >
    struct Extractor
    {
        typedef T Type;
    };

    struct OperationalState : public Extractor< float >
    {
        bool  HasFlag( const ASN1T_MsgUnitAttributes& attributes ) 
        { return attributes.m.etat_operationnel_brutPresent; }
        float Extract( const ASN1T_MsgUnitAttributes& attributes ) 
        { return float( attributes.etat_operationnel_brut ) * 0.01f; }
    };
}

#endif // __Extractors_h_
