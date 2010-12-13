// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectKnowledgeNotifier_h_
#define __ObjectKnowledgeNotifier_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"

namespace sword
{
    class ObjectKnowledgeUpdate;
}

namespace kernel
{
    class Controller;
}

namespace dispatcher
{
    class ObjectKnowledge;
}

namespace plugins
{
namespace script
{

// =============================================================================
/** @class  ObjectKnowledgeNotifier
    @brief  ObjectKnowledgeNotifier
*/
// Created: AGE 2008-07-16
// =============================================================================
class ObjectKnowledgeNotifier : public kernel::Extension_ABC
                              , public kernel::Updatable_ABC< sword::ObjectKnowledgeUpdate >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledgeNotifier( kernel::Controller& controller, const dispatcher::ObjectKnowledge& knowledge );
    virtual ~ObjectKnowledgeNotifier();
    //@}

    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::ObjectKnowledgeUpdate& message );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectKnowledgeNotifier( const ObjectKnowledgeNotifier& );            //!< Copy constructor
    ObjectKnowledgeNotifier& operator=( const ObjectKnowledgeNotifier& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const dispatcher::ObjectKnowledge& knowledge_;
    bool created_;
    //@}
};

}
}

#endif // __ObjectKnowledgeNotifier_h_
