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

#include "clients_gui/MergingTacticalHierarchies.h"
#include <tools/Resolver_ABC.h>

namespace sword {
    class FormationChangeSuperior;
}

namespace kernel {
    class Formation_ABC;
    class Team_ABC;
    class SymbolFactory;
}

// =============================================================================
/** @class  FormationHierarchy
    @brief  FormationHierarchy
    // $$$$ AGE 2006-10-19: chies
*/
// Created: AGE 2006-10-19
// =============================================================================
class FormationHierarchy : public gui::MergingTacticalHierarchies
                         , public kernel::Updatable_ABC< sword::FormationChangeSuperior >
{
public:
    //! @name Constructors/Destructor
    //@{
             FormationHierarchy( kernel::Controller& controller, kernel::Formation_ABC& entity, kernel::Entity_ABC* superior, kernel::SymbolFactory& factory,
                     const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver, const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver );
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

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::FormationChangeSuperior& message );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    const tools::Resolver_ABC< kernel::Team_ABC >& teamResolver_;
    kernel::Entity_ABC* superior_;
    std::string level_;
    //@}
};

#endif // __FormationHierarchy_h_
