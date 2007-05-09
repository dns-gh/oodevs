 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DotationQuota_h_
#define __DotationQuota_h_

#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  DotationQuota
    @brief  DotationQuota
*/
// Created: NLD 2006-09-19
// =============================================================================
class DotationQuota
{
public:
    //! @name Constructors/Destructor
    //@{
     DotationQuota( const Model& model, const ASN1T_DotationQuota& asnMsg );
    ~DotationQuota();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_DotationQuota& asnMsg );
    void Send  ( ASN1T_DotationQuota& asnMsg ) const ;
    //@}

private:
    const unsigned int nDotationType_; // XML reference - no resolved by dispatcher
          unsigned int nQuota_;
};

}

#endif // __DotationQuota_h_
