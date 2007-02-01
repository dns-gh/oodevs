// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DatasetFactory_ABC_h_
#define __DatasetFactory_ABC_h_

#include "Messages.h"

namespace frontend
{
    class Dataset;

// =============================================================================
/** @class  DatasetFactory_ABC
    @brief  DatasetFactory_ABC
*/
// Created: SBO 2007-01-29
// =============================================================================
class DatasetFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             DatasetFactory_ABC() {}
    virtual ~DatasetFactory_ABC() {}
    //@}

    //! @name Operations
    //@{
    virtual Dataset* Create( const ASN1T_MsgDatasetCreation& message ) = 0;
    //@}
};

}

#endif // __DatasetFactory_ABC_h_
