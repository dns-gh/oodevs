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

#include "TacticalHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Entity_ABC;
}

// =============================================================================
/** @class  GhostHierarchies
    @brief  GhostHierarchies
*/
// Created: ABR 2012-06-28
// =============================================================================
class GhostHierarchies : public TacticalHierarchies
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

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual std::string GetLevel() const;
    virtual std::string GetSymbol() const;
    virtual void UpdateSymbolUpward();
    virtual void NotifySuperiorChanged( const kernel::Entity_ABC* newSuperior );
    void UpdateSymbol( const std::string& level, const std::string& symbol );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void DoUpdate( const kernel::InstanciationComplete& );
    void UpdateKarma( const kernel::Entity_ABC& superior );
    //@}

private:
    //! @name Member data
    //@{
    std::string level_;
    std::string baseSymbol_;
    std::string symbol_;
    kernel::Entity_ABC* superior_;
};

#endif // __GhostHierarchies_h_
