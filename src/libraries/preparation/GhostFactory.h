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
    class SymbolFactory;
}

namespace xml
{
    class xistream;
}

namespace tools
{
    class IdManager;
}

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
                           tools::IdManager& idManager, kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory, kernel::SymbolFactory& symbolsFactory );
    virtual ~GhostFactory();
    //@}

    //! @name Operations
    //@{
    virtual kernel::Ghost_ABC* Create( kernel::Entity_ABC& parent, const kernel::GhostPrototype& prototype, const geometry::Point2f& position );
    virtual kernel::Ghost_ABC* Create( kernel::Entity_ABC& parent, xml::xistream& xis );
    virtual kernel::Ghost_ABC* Create( kernel::Entity_ABC& parent, xml::xistream& xis, E_GhostType ghostType );
    //@}

private:
    //! @name Helpers
    //@{
    geometry::Point2f ComputeAutomatPosition( xml::xistream& xis ) const;
    void InternalComputeAutomatPosition( xml::xistream& xis, geometry::Polygon2f& ) const;
    void ReadUnitPosition( xml::xistream& xis, geometry::Polygon2f& ) const;
    //@}

private:
    //! @name Member data
    //@{
     kernel::Controllers&               controllers_;
     Model&                             model_;
     const StaticModel&                 staticModel_;
     tools::IdManager&                  idManager_;
     kernel::KnowledgeGroupFactory_ABC& knowledgeGroupFactory_;
     kernel::SymbolFactory&             symbolsFactory_;
    //@}
};

#endif // __GhostFactory_h_
