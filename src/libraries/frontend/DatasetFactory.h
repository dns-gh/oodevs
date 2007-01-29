// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DatasetFactory_h_
#define __DatasetFactory_h_

#include "DatasetFactory_ABC.h"

namespace kernel
{
    class Controllers;
}

namespace frontend
{

// =============================================================================
/** @class  DatasetFactory
    @brief  DatasetFactory
*/
// Created: SBO 2007-01-29
// =============================================================================
class DatasetFactory : public DatasetFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DatasetFactory( kernel::Controllers& controllers );
    virtual ~DatasetFactory();
    //@}

    //! @name Operations
    //@{
    virtual Dataset* Create();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DatasetFactory( const DatasetFactory& );            //!< Copy constructor
    DatasetFactory& operator=( const DatasetFactory& ); //!< Assignment operator
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

#endif // __DatasetFactory_h_
