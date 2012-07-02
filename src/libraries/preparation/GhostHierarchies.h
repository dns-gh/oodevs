// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __GhostHierarchies_h_
#define __GhostHierarchies_h_

#include "AgentHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class InstanciationComplete;
}

// =============================================================================
/** @class  GhostHierarchies
    @brief  GhostHierarchies
*/
// Created: ABR 2012-06-28
// =============================================================================
class GhostHierarchies : public AgentHierarchies
                       , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GhostHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder, const std::string& level, const std::string& symbol, kernel::Entity_ABC* superior );
    virtual ~GhostHierarchies();
    //@}

    //! @name Serializable_ABC operations
    //@{
    virtual void SerializeAttributes( xml::xostream& ) const;
    //@}

    //! @name Creatable_ABC
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    //@}
};

#endif // __GhostHierarchies_h_
