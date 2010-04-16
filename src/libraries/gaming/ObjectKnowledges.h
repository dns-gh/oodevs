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
#include "protocol/Protocol.h"
#include "tools/Resolver.h"

using namespace Common;

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
                       , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeCreation >
                       , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeUpdate >
                       , public kernel::Updatable_ABC< MsgsSimToClient::MsgObjectKnowledgeDestruction >
                       , public tools::Resolver< kernel::ObjectKnowledge_ABC >
                       , public kernel::Creatable< ObjectKnowledges >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledges( const kernel::Entity_ABC& team, kernel::Controller& controller, ObjectKnowledgeFactory& factory );
    virtual ~ObjectKnowledges();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ObjectKnowledges( const ObjectKnowledges& );            //!< Copy constructor
    ObjectKnowledges& operator=( const ObjectKnowledges& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeCreation&    message );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeUpdate&      message );
    virtual void DoUpdate( const MsgsSimToClient::MsgObjectKnowledgeDestruction& message );
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
