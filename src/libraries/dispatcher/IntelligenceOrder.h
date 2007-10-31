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

#include "Position.h"

struct ASN1T_Intelligence;

namespace dispatcher
{

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
             IntelligenceOrder( Model& model, const ASN1T_Intelligence& asn );
    virtual ~IntelligenceOrder();
    //@}

    //! @name Operations
    //@{
    void Send( ASN1T_Intelligence& asn ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntelligenceOrder( const IntelligenceOrder& );            //!< Copy constructor
    IntelligenceOrder& operator=( const IntelligenceOrder& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const std::string name_;
    const std::string nature_;
    const ASN1T_EnumNatureLevel level_;
    const bool embarked_;
    const Position position_;
    const ASN1T_EnumDiplomacy diplomacy_;
    const unsigned int formation_;
    //@}
};

}

#endif // __IntelligenceOrder_h_
