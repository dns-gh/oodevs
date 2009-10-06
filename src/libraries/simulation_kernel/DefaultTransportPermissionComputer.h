// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __transportTransportPermissionComputer_H__
#define __transportTransportPermissionComputer_H__

#include "TransportPermissionComputer_ABC.h"

namespace transport
{

// =============================================================================
/** @class  ConsumptionChangeRequestHandler_ABC
    @brief  ConsumptionChangeRequestHandler_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class DefaultTransportPermissionComputer: public transport::TransportPermissionComputer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    DefaultTransportPermissionComputer();
    virtual ~DefaultTransportPermissionComputer();
    //@}

    //! @name Operations
    //@{
    void Reset();
    void AllowLoading(bool doAllow);
    bool CanBeLoaded() const;
    //@}

private:
    //! @name Attributes
    //@{
    bool  allow_;
    //@}
};

}

#endif /* __transportTransportPermissionComputer_H__ */
