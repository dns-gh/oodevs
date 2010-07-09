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

#include "protocol/Protocol.h"

namespace Common
{
    class MsgIntelligence;
    enum EnumNatureLevel;
    enum EnumDiplomacy;
    class MsgCoordLatLong;
}

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
    explicit IntelligenceOrder( const Common::MsgIntelligence& asn );
    virtual ~IntelligenceOrder();
    //@}

    //! @name Operations
    //@{
    void Send( Common::MsgIntelligence& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    std::string name_;
    std::string nature_;
    Common::EnumNatureLevel level_;
    bool embarked_;
    Common::MsgCoordLatLong position_;
    Common::EnumDiplomacy diplomacy_;
    unsigned int formation_;
    //@}
};

}

#endif // __IntelligenceOrder_h_
