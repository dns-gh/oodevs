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

#include "game_asn/Simulation.h"

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
     Equipment( const Model& model, const ASN1T_EquipmentDotations& asnMsg );
    ~Equipment();
    //@}

    //! @name Operations
    //@{
    void Update( const ASN1T_EquipmentDotations& asnMsg );
    void Send  ( ASN1T_EquipmentDotations& asnMsg ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Equipment& operator=( const Equipment& );
    //@}

public:
    //! @name Member data
    //@{
    const unsigned int nEquipmentType_; // XML reference - no resolved by dispatcher
          unsigned int nNbrAvailable_;
          unsigned int nNbrUnavailable_;
          unsigned int nNbrRepairable_;
          unsigned int nNbrInMaintenance_;
          unsigned int nNbrPrisoner_;
    //@}
};

}

#endif // __Equipment_h_
