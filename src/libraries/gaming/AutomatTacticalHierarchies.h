// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatTacticalHierarchies_h_
#define __AutomatTacticalHierarchies_h_

#include "clients_kernel/MergingTacticalHierarchies.h"
#include "tools/Resolver_ABC.h"

namespace sword
{
    class AutomatChangeSuperior;
}

namespace kernel
{
    class PropertiesDictionary;
    class Automat_ABC;
    class Formation_ABC;
}

// =============================================================================
/** @class  AutomatTacticalHierarchies
    @brief  AutomatTacticalHierarchies
*/
// Created: AGE 2006-10-19
// =============================================================================
class AutomatTacticalHierarchies : public kernel::MergingTacticalHierarchies
                                 , public kernel::Updatable_ABC< sword::AutomatChangeSuperior >
{
public:
    //! @name Constructors/Destructor
    //@{
             AutomatTacticalHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder,
                                         kernel::Entity_ABC& superior,
                                         const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver );
    virtual ~AutomatTacticalHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual const kernel::Entity_ABC* GetSuperior() const;
    virtual std::string GetLevel() const;
    virtual void UpdateSymbolUpward();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatTacticalHierarchies( const AutomatTacticalHierarchies& );            //!< Copy constructor
    AutomatTacticalHierarchies& operator=( const AutomatTacticalHierarchies& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const sword::AutomatChangeSuperior& message );
    virtual void DoUpdate( const kernel::InstanciationComplete& message );
    virtual void MergeSymbol( const kernel::Entity_ABC& entity );
    virtual void UpdateLevel();
    static std::string Max( const std::string& lhs, const std::string& rhs );
    static std::string Increase( const std::string& value );
    static char Level( const std::string& value );
    static unsigned Count( const std::string& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    kernel::Entity_ABC& superior_;
    std::string level_;
    //@}
};

#endif // __AutomatTacticalHierarchies_h_
