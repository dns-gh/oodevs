// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __IntelligenceOrder_h_
#define __IntelligenceOrder_h_

#include "game_asn/simulation.h"

namespace dispatcher
{
    class Model;

// =============================================================================
/** @class  IntelligenceOrder
    @brief  IntelligenceOrder
*/
// Created: SBO 2007-10-30
// =============================================================================
class IntelligenceOrder
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit IntelligenceOrder( const ASN1T_Intelligence& asn );
    virtual ~IntelligenceOrder();
    //@}

    //! @name Operations
    //@{
    void Send( ASN1T_Intelligence& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string nature_;
    ASN1T_EnumNatureLevel level_;
    bool embarked_;
    ASN1T_CoordLatLong position_;
    ASN1T_EnumDiplomacy diplomacy_;
    unsigned int formation_;
    //@}
};

}

#endif // __IntelligenceOrder_h_
