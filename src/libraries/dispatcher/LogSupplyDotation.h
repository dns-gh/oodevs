 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogSupplyDotation_h_
#define __LogSupplyDotation_h_

#include "game_asn/Asn.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  LogSupplyDotation
    @brief  LogSupplyDotation
*/
// Created: NLD 2006-09-19
// =============================================================================
class LogSupplyDotation
{
public:
    //! @name Constructors/Destructor
    //@{
     LogSupplyDotation( const Model& model, const ASN1T_DotationQuery& asnMsg );
    ~LogSupplyDotation();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_DotationQuery& asnMsg );
    void Send  ( ASN1T_DotationQuery& asnMsg ) const ;
    //@}

private:
    const unsigned int nDotationType_; // XML reference - no resolved by dispatcher
          unsigned int nNbrRequested_;
          unsigned int nNbrGranted_;
          unsigned int nNbrConvoyed_;
};

}

#endif // __LogSupplyDotation_h_
