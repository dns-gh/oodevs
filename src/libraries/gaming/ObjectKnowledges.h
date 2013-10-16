// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledges_h_
#define __ObjectKnowledges_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Creatable.h"
#include <tools/Resolver.h>

namespace sword
{
    class ObjectKnowledgeCreation;
    class ObjectKnowledgeDestruction;
    class ObjectKnowledgeUpdate;
}

namespace kernel
{
    class Controller;
    class Entity_ABC;
    class ObjectKnowledge_ABC;
}

class ObjectKnowledgeFactory;

// =============================================================================
/** @class  ObjectKnowledges
    @brief  ObjectKnowledges
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledges : public kernel::Extension_ABC
                       , public kernel::Updatable_ABC< sword::ObjectKnowledgeCreation >
                       , public kernel::Updatable_ABC< sword::ObjectKnowledgeUpdate >
                       , public kernel::Updatable_ABC< sword::ObjectKnowledgeDestruction >
                       , public tools::Resolver< kernel::ObjectKnowledge_ABC >
                       , public kernel::Creatable< ObjectKnowledges >
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledges( const kernel::Entity_ABC& team, kernel::Controller& controller, ObjectKnowledgeFactory& factory );
    virtual ~ObjectKnowledges();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectKnowledges( const ObjectKnowledges& );            //!< Copy constructor
    ObjectKnowledges& operator=( const ObjectKnowledges& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeCreation&    message );
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate&      message );
    virtual void DoUpdate( const sword::ObjectKnowledgeDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC* owner_;
    kernel::Controller& controller_;
    ObjectKnowledgeFactory& factory_;
    //@}
};

#endif // __ObjectKnowledges_h_
