// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __HealableComputerFactory_h_
#define __HealableComputerFactory_h_

#include "simulation_kernel/HealComputerFactory_ABC.h"
#include "simulation_kernel/PostureComputer_ABC.h"

namespace human
{
// =============================================================================
/** @class  HealableComputerFactory
    @brief  HealableComputerFactory
*/
// Created: MGD 2009-08-13
// =============================================================================
class HealComputerFactory : public HealComputerFactory_ABC
{

public:
    //! @name ConstructorsDestructor
    //@{
             HealComputerFactory();
    virtual ~HealComputerFactory();
    //@}

    //! @name Operations
    //@{
    virtual HealComputer_ABC& Create( const PHY_HumanRank& rank, unsigned int nNbrToChange ) const;
    //@}

private:
    //! @name Operations
    //@{
    std::auto_ptr< HealComputer_ABC > pHealableComputer_;
    //@}
};

}

#endif // __HealableComputerFactory_h_
