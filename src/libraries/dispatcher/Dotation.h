 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Dotation_h_
#define __Dotation_h_

#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  Dotation
    @brief  Dotation
*/
// Created: NLD 2006-09-19
// =============================================================================
class Dotation
{
public:
    //! @name Constructors/Destructor
    //@{
     Dotation( const Model& model, const ASN1T_DotationRessource& asnMsg );
     Dotation( const Model& model, const ASN1T_DotationStock    & asnMsg );
    ~Dotation();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_DotationRessource& asnMsg );
    void Update( const ASN1T_DotationStock& asnMsg );
    void Send  ( ASN1T_DotationRessource& asnMsg ) const ;
    void Send  ( ASN1T_DotationStock& asnMsg ) const ;
    //@}

private:
    const unsigned int nDotationType_; // XML reference - no resolved by dispatcher
          unsigned int nNbr_;
};

}

#endif // __Dotation_h_
