// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PhysicalModelFactory_h_
#define __PhysicalModelFactory_h_

#include "PhysicalModelFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace frontend
{

// =============================================================================
/** @class  PhysicalModelFactory
    @brief  PhysicalModelFactory
*/
// Created: SBO 2007-01-29
// =============================================================================
class PhysicalModelFactory : public PhysicalModelFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PhysicalModelFactory( kernel::Controllers& controllers );
    virtual ~PhysicalModelFactory();
    //@}

    //! @name Operations
    //@{
    virtual PhysicalModel* Create();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PhysicalModelFactory( const PhysicalModelFactory& );            //!< Copy constructor
    PhysicalModelFactory& operator=( const PhysicalModelFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    //@}
};

}

#endif // __PhysicalModelFactory_h_
