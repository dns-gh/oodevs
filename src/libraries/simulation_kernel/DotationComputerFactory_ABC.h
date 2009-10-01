// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __DotationComputerFactory_ABC_h_
#define __DotationComputerFactory_ABC_h_

namespace dotation
{
    class DotationComputer_ABC;
// =============================================================================
/** @class  DotationComputerFactory_ABC
    @brief  DotationComputerFactory_ABC
*/
// Created: MGD 2009-08-13
// =============================================================================
class DotationComputerFactory_ABC : private boost::noncopyable
{

public:
    //! @name Constructors/Destructor
    //@{
             DotationComputerFactory_ABC();
    virtual ~DotationComputerFactory_ABC();
    //@}

    //! @name Operations
    //@{
    virtual DotationComputer_ABC& Create() const = 0;
    //@}
};

}

#endif // __DotationComputerFactory_ABC_h_
