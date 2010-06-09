 // *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __SupplyRouteAttribute_h_
#define __SupplyRouteAttribute_h_

#include "ObjectAttribute_ABC.h"

namespace dispatcher
{

// =============================================================================
/** @class  SupplyRouteAttribute
    @brief  SupplyRouteAttribute
*/
// Created: NLD 2006-09-19
// =============================================================================
class SupplyRouteAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit SupplyRouteAttribute( const Common::MsgObjectAttributes& message );
    virtual ~SupplyRouteAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Update( const Common::MsgObjectAttributes& message );
    virtual void Send  ( Common::MsgObjectAttributes& message ) const;
    //@}

private:
    //! @name Member data
    //@{
    bool         bEquipped_;
    unsigned int nMaxWeight_;
    unsigned int nWidth_;
    unsigned int nLength_;
    unsigned int nFlow_;
    //@}
};

}

#endif // __SupplyRouteAttribute_h_
