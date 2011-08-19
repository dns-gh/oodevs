// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __ConsumeDotationNotificationHandler_ABC_H__
#define __ConsumeDotationNotificationHandler_ABC_H__

class PHY_DotationCategory;

namespace dotation {

// =============================================================================
/** @class  ConsumeDotationNotificationHandler_ABC
    @brief  ConsumeDotationNotificationHandler_ABC
*/
// Created: MGD 2009-09-24
// =============================================================================
class ConsumeDotationNotificationHandler_ABC {
public:
             ConsumeDotationNotificationHandler_ABC() {}
    virtual ~ConsumeDotationNotificationHandler_ABC() {}

    virtual void NotifyConsumeDotation( const PHY_DotationCategory& category, double& rNbr ) = 0;
};

} // namespace component

#endif /* __ConsumeDotationNotificationHandler_ABC_H__ */
