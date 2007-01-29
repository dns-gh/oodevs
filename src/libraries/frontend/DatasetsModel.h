// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DatasetsModel_h_
#define __DatasetsModel_h_

#include "clients_kernel/Resolver.h"

namespace frontend
{
    class Dataset;
    class DatasetFactory_ABC;

// =============================================================================
/** @class  DatasetsModel
    @brief  DatasetsModel
*/
// Created: SBO 2007-01-29
// =============================================================================
class DatasetsModel : public kernel::StringResolver< Dataset >
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DatasetsModel( DatasetFactory_ABC& factory );
    virtual ~DatasetsModel();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DatasetsModel( const DatasetsModel& );            //!< Copy constructor
    DatasetsModel& operator=( const DatasetsModel& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    DatasetFactory_ABC& factory_;
    //@}
};

}

#endif // __DatasetsModel_h_
