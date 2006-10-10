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
#include "clients_kernel/Updatable_ABC.h"

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class InstanciationComplete;
}

// =============================================================================
/** @class  EntityCommunications
    @brief  EntityCommunications
*/
// Created: SBO 2006-10-09
// =============================================================================
class EntityCommunications : public kernel::EntityHierarchies< kernel::CommunicationHierarchies >
                           , public kernel::Updatable_ABC< kernel::InstanciationComplete >
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

    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    kernel::Entity_ABC* superior_;
    //@}
};

#endif // __EntityCommunications_h_
