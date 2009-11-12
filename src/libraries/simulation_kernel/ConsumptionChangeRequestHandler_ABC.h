// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __dotation_ConsumptionChangeRequestHandler_ABC_H__
#define __dotation_ConsumptionChangeRequestHandler_ABC_H__

namespace dotation
{

class ConsumptionModeChangeRequest_ABC;

// =============================================================================
/** @class  ConsumptionChangeRequestHandler_ABC
    @brief  ConsumptionChangeRequestHandler_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================

class ConsumptionChangeRequestHandler_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ConsumptionChangeRequestHandler_ABC() {}
    virtual ~ConsumptionChangeRequestHandler_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual void ChangeConsumptionMode(ConsumptionModeChangeRequest_ABC& request)=0;
    //@}
};

}

#endif /* __dotation_ConsumptionChangeRequestHandler_ABC_H__ */
