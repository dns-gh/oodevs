// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __HealableComputerFactory_ABC_h_
#define __HealableComputerFactory_ABC_h_

namespace human
{
    class HealComputer_ABC;
// =============================================================================
/** @class  HealableComputerFactory_ABC
    @brief  HealableComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class HealComputerFactory_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             HealComputerFactory_ABC() {}
    virtual ~HealComputerFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual HealComputer_ABC& Create() const = 0;
    //@}
};

}

#endif // __HealableComputerFactory_ABC_h_
