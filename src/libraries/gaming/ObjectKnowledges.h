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

#include "game_asn/Asn.h"
#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Creatable.h"

namespace kernel
{
    class Controller;
    class Team_ABC;
}

class ObjectKnowledge_ABC;
class ObjectKnowledgeFactory;

// =============================================================================
/** @class  ObjectKnowledges
    @brief  ObjectKnowledges
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectKnowledges : public kernel::Extension_ABC
                       , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeCreation >
                       , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeUpdate >
                       , public kernel::Updatable_ABC< ASN1T_MsgObjectKnowledgeDestruction >
                       , public kernel::Resolver< ObjectKnowledge_ABC >
                       , public kernel::Creatable< ObjectKnowledges >
{

public:
    //! @name Constructors/Destructor
    //@{
             ObjectKnowledges( const kernel::Team_ABC& team, kernel::Controller& controller, ObjectKnowledgeFactory& factory );
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
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeCreation&    message );
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeUpdate&      message );
    virtual void DoUpdate( const ASN1T_MsgObjectKnowledgeDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Team_ABC& team_;
    kernel::Controller& controller_;
    ObjectKnowledgeFactory& factory_;
    //@}
};

#endif // __ObjectKnowledges_h_
