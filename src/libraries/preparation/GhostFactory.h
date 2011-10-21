// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __GhostFactory_h_
#define __GhostFactory_h_

#include "GhostFactory_ABC.h"

namespace kernel
{
    class Controllers;
    class Entity_ABC;
    class Ghost_ABC;
    class GhostPrototype;
    class KnowledgeGroupFactory_ABC;
}

namespace xml
{
    class xistream;
}

class IdManager;
class Model;
class StaticModel;

// =============================================================================
/** @class  GhostFactory
    @brief  GhostFactory
*/
// Created: ABR 2011-10-14
// =============================================================================
class GhostFactory : public GhostFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             GhostFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel,
                           IdManager& idManager, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory );
    virtual ~GhostFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Ghost_ABC* Create( kernel::Entity_ABC& parent, const kernel::GhostPrototype& prototype, const geometry::Point2f& position );
    virtual kernel::Ghost_ABC* Create( kernel::Entity_ABC& parent, xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
     kernel::Controllers&               controllers_;
     Model&                             model_;
     const StaticModel&                 staticModel_;
     IdManager&                         idManager_;
     kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory_;
    //@}
};

#endif // __GhostFactory_h_
