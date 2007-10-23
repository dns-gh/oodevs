// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityCommunications_h_
#define __EntityCommunications_h_

#include "clients_kernel/EntityHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
}

// =============================================================================
/** @class  EntityCommunications
    @brief  EntityCommunications
*/
// Created: SBO 2006-10-09
// =============================================================================
class EntityCommunications : public kernel::EntityHierarchies< kernel::CommunicationHierarchies >
{

public:
    //! @name Constructors/Destructor
    //@{
             EntityCommunications( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::Entity_ABC* superior );
    virtual ~EntityCommunications();
    //@}

    //! @name Operations
    //@{
    void ChangeSuperior( kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    EntityCommunications( const EntityCommunications& );            //!< Copy constructor
    EntityCommunications& operator=( const EntityCommunications& ); //!< Assignement operator
    //@}
};

#endif // __EntityCommunications_h_
