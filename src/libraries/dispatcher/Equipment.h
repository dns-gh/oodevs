 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Equipment_h_
#define __Equipment_h_

#include "AsnTypes.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  Equipment
    @brief  Equipment
*/
// Created: NLD 2006-09-19
// =============================================================================
class Equipment
{
public:
    //! @name Constructors/Destructor
    //@{
     Equipment( const Model& model, const ASN1T_DotationEquipement& asnMsg );
    ~Equipment();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_DotationEquipement& asnMsg );
    //@}

private:
    const unsigned int nEquipmentType_; // XML reference - no resolved by dispatcher
          unsigned int nNbrAvailable_;
          unsigned int nNbrUnavailable_;
          unsigned int nNbrRepairable_;
          unsigned int nNbrInMaintenance_;
          unsigned int nNbrPrisoner_;
};

}

#endif // __Equipment_h_
