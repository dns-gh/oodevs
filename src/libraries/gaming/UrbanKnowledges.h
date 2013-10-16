// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UrbanKnowledges_h_
#define __UrbanKnowledges_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/Creatable.h"
#include <tools/Resolver.h>

namespace kernel
{
    class Controller;
    class Team_ABC;
    class UrbanKnowledge_ABC;
}

namespace sword
{
    class UrbanKnowledgeCreation;
    class UrbanKnowledgeUpdate;
    class UrbanKnowledgeDestruction;
}

class UrbanKnowledgeFactory;

// =============================================================================
/** @class  UrbanKnowledges
    @brief  UrbanKnowledges
*/
// Created: MGD 2009-12-09
// =============================================================================
class UrbanKnowledges : public kernel::Extension_ABC
                      , public kernel::Updatable_ABC< sword::UrbanKnowledgeCreation >
                      , public kernel::Updatable_ABC< sword::UrbanKnowledgeUpdate >
                      , public kernel::Updatable_ABC< sword::UrbanKnowledgeDestruction >
                      , public tools::Resolver< kernel::UrbanKnowledge_ABC >
                      , public kernel::Creatable< UrbanKnowledges >
{
public:
    //! @name Constructors/Destructor
    //@{
             UrbanKnowledges( const kernel::Team_ABC& team, kernel::Controller& controller, UrbanKnowledgeFactory& factory );
    virtual ~UrbanKnowledges();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UrbanKnowledges( const UrbanKnowledges& );            //!< Copy constructor
    UrbanKnowledges& operator=( const UrbanKnowledges& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::UrbanKnowledgeCreation&    message );
    virtual void DoUpdate( const sword::UrbanKnowledgeUpdate&      message );
    virtual void DoUpdate( const sword::UrbanKnowledgeDestruction& message );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Team_ABC& team_;
    kernel::Controller& controller_;
    UrbanKnowledgeFactory& factory_;
    //@}
};

#endif // __UrbanKnowledges_h_
