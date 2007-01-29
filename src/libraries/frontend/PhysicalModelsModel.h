// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PhysicalModelsModel_h_
#define __PhysicalModelsModel_h_

#include "clients_kernel/Resolver.h"

namespace frontend
{
    class PhysicalModel;
    class PhysicalModelFactory_ABC;

// =============================================================================
/** @class  PhysicalModelsModel
    @brief  PhysicalModelsModel
*/
// Created: SBO 2007-01-29
// =============================================================================
class PhysicalModelsModel : public kernel::StringResolver< PhysicalModel >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit PhysicalModelsModel( PhysicalModelFactory_ABC& factory );
    virtual ~PhysicalModelsModel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    PhysicalModelsModel( const PhysicalModelsModel& );            //!< Copy constructor
    PhysicalModelsModel& operator=( const PhysicalModelsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    PhysicalModelFactory_ABC& factory_;
    //@}
};

}

#endif // __PhysicalModelsModel_h_
