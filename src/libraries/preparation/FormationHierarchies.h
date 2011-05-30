// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationHierarchies_h_
#define __FormationHierarchies_h_

#include "clients_kernel/MergingTacticalHierarchies.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel {
    class Formation_ABC;
    class SymbolFactory;
}

// =============================================================================
/** @class  FormationHierarchies
    @brief  FormationHierarchies
*/
// Created: SBO 2006-09-21
// =============================================================================
class FormationHierarchies : public kernel::MergingTacticalHierarchies
                           , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationHierarchies( kernel::Controller& controller, kernel::Formation_ABC& holder, kernel::Entity_ABC* superior, kernel::SymbolFactory& factory );
    virtual ~FormationHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual std::string GetLevel() const;
    void ChangeSuperior( kernel::Entity_ABC& superior );
    virtual void UnregisterParent();
    //@}

private:
    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void SerializeLogistics ( xml::xostream& xos ) const;
    virtual void DoUpdate( const kernel::InstanciationComplete& ic );
    //@}

    //! @name Member data
    //@{
    kernel::Entity_ABC* superior_;
    std::string level_;
    //@}
};

#endif // __FormationHierarchies_h_
