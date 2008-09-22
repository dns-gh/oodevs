// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityPublisher_ABC_h_
#define __EntityPublisher_ABC_h_

#include "clients_kernel/Extension_ABC.h"

namespace dispatcher
{
    class ClientPublisher_ABC;

// =============================================================================
/** @class  EntityPublisher_ABC
    @brief  EntityPublisher_ABC
*/
// Created: AGE 2008-06-20
// =============================================================================
class EntityPublisher_ABC : public kernel::Extension_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityPublisher_ABC() {};
    virtual ~EntityPublisher_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void SendFullUpdate ( ClientPublisher_ABC& publisher ) const = 0;
    virtual void SendCreation   ( ClientPublisher_ABC& publisher ) const = 0;
    virtual void SendDestruction( ClientPublisher_ABC& publisher ) const = 0;
    //@}
};

}

#endif // __EntityPublisher_ABC_h_
