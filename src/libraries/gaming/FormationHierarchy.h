// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __FormationHierarchy_h_
#define __FormationHierarchy_h_

#include "clients_kernel/MergingTacticalHierarchies.h"

namespace kernel {
    class Formation_ABC;
    class SymbolFactory;
}

// =============================================================================
/** @class  FormationHierarchy
    @brief  FormationHierarchy
    // $$$$ AGE 2006-10-19: chies
*/
// Created: AGE 2006-10-19
// =============================================================================
class FormationHierarchy : public kernel::MergingTacticalHierarchies
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationHierarchy( kernel::Controller& controller, kernel::Formation_ABC& entity, kernel::Entity_ABC* superior, kernel::SymbolFactory& factory );
    virtual ~FormationHierarchy();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual std::string GetLevel() const;
    virtual void DoUpdate( const kernel::InstanciationComplete& ic );
    virtual void UnregisterParent();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FormationHierarchy( const FormationHierarchy& );            //!< Copy constructor
    FormationHierarchy& operator=( const FormationHierarchy& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Entity_ABC* superior_;
    std::string level_;
    //@}
};

#endif // __FormationHierarchy_h_
