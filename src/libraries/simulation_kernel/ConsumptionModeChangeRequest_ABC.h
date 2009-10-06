// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************
#ifndef __dotation_ConsumptionModeChangeRequest_ABC_H__
#define __dotation_ConsumptionModeChangeRequest_ABC_H__

class PHY_ConsumptionType;

namespace dotation
{

class PHY_RoleInterface_Dotations;

// =============================================================================
/** @class  ConsumptionModeChangeRequest_ABC
    @brief  ConsumptionModeChangeRequest_ABC
*/
// Created: AHC 2009-10-01
// =============================================================================
class ConsumptionModeChangeRequest_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    ConsumptionModeChangeRequest_ABC();
    virtual ~ConsumptionModeChangeRequest_ABC();
    //@}

    //! @name Operations
    //@{
    virtual void Reset(const PHY_ConsumptionType& newMode) =0;
    // Used in execute to know the new mode
    virtual const PHY_ConsumptionType& GetType() const =0;
    // Used in Execute to specify whether change is ok
    // Must revert previous changes if changed is false
    virtual void ConsumptionModeChanged(bool changed, PHY_RoleInterface_Dotations* const dotation) =0;
    // Used by client to check if all changes were applied
    virtual bool AllChanged() const =0;
    //@}
};

}

#endif /* __dotation_ConsumptionModeChangeRequest_ABC_H__ */
