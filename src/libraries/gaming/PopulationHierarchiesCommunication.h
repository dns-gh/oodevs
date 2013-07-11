// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __PopulationHierarchiesCommunication_h_
#define __PopulationHierarchiesCommunication_h_

#include "clients_gui/EntityHierarchies.h"
#include "clients_kernel/CommunicationHierarchies.h"
#include "clients_kernel/Updatable_ABC.h"
#include <boost/noncopyable.hpp>

namespace sword
{
    class CrowdCreation;
}

namespace kernel
{
    class KnowledgeGroup_ABC;
}

// =============================================================================
/** @class  PopulationHierarchiesCommunication
    @brief  PopulationHierarchiesCommunication
*/
// Created: JSR 2013-07-10
// =============================================================================
class PopulationHierarchiesCommunication : public gui::EntityHierarchies< kernel::CommunicationHierarchies >
                                         , private boost::noncopyable

{

public:
    //! @name Constructors/Destructor
    //@{
             PopulationHierarchiesCommunication( kernel::Controller& controller, kernel::Entity_ABC& holder, kernel::KnowledgeGroup_ABC* knowledgeGroup );
    virtual ~PopulationHierarchiesCommunication();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool CanCommunicate() const;
    virtual bool IsJammed() const;
    //@}
};

#endif // __PopulationHierarchiesCommunication_h_
