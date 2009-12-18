// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __dotation_DefaultConsumptionModeChangeRequest_H__
#define __dotation_DefaultConsumptionModeChangeRequest_H__

#include "simulation_kernel/ConsumptionModeChangeRequest_ABC.h"
#include <vector>

namespace dotation
{

// =============================================================================
/** @class  DefaultConsumptionModeChangeRequest
    @brief  DefaultConsumptionModeChangeRequest
*/
// Created: AHC 2009-10-01
// =============================================================================
class DefaultConsumptionModeChangeRequest : public ConsumptionModeChangeRequest_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit DefaultConsumptionModeChangeRequest( const PHY_ConsumptionType& newMode );
    virtual ~DefaultConsumptionModeChangeRequest();
    //@}

    //! @name Operations
    //@{
    virtual const PHY_ConsumptionType& GetType() const;
    virtual void ConsumptionModeChanged(bool changed, PHY_RoleInterface_Dotations* const dotation);
    virtual bool AllChanged() const;
    //@}

private:
    //! @name Operations
    //@{
    void Rollback();
    //@}

private:
    //! @name Attributes
    //@{
    const PHY_ConsumptionType* newMode_;
    std::vector<PHY_RoleInterface_Dotations* const> changedDotations_;
    bool failed_;
    //@}
};

}

#endif /* __dotation_DefaultConsumptionModeChangeRequest_H__ */
