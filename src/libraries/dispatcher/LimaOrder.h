// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimaOrder_h_
#define __LimaOrder_h_

#include "Localisation.h"

namespace Common
{
    class MsgLimaOrder;
}

namespace dispatcher
{
// =============================================================================
/** @class  LimaOrder
    @brief  LimaOrder
*/
// Created: NLD 2007-04-20
// =============================================================================
class LimaOrder
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit LimaOrder( const Common::MsgLimaOrder& asn );
    virtual ~LimaOrder();
    //@}

    //! @name Operations
    //@{
    void Send( Common::MsgLimaOrder& asn ) const;
    //@}

private:
    //! @name Member data
    //@{
    Localisation location_;
    std::vector< int > functions_;
    std::string schedule_;
    //@}
};

}

#endif // __LimaOrder_h_
