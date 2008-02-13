// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AutomatTacticalHierarchies_h_
#define __AutomatTacticalHierarchies_h_

#include "game_asn/Asn.h"
#include "clients_kernel/MergingTacticalHierarchies.h"
#include "clients_kernel/Resolver_ABC.h"

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
                                 , public kernel::Updatable_ABC< ASN1T_MsgAutomatChangeSuperior >
{

public:
    //! @name Constructors/Destructor
    //@{
             AutomatTacticalHierarchies( kernel::Controller& controller, kernel::Entity_ABC& holder,
                                         kernel::Entity_ABC& superior,
                                         const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver, const kernel::Resolver_ABC< kernel::Formation_ABC >& formationResolver );
    virtual ~AutomatTacticalHierarchies();
    //@}

    //! @name Operations
    //@{
    virtual std::string GetLevel() const;
    virtual void UpdateSymbol( bool up = true );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AutomatTacticalHierarchies( const AutomatTacticalHierarchies& );            //!< Copy constructor
    AutomatTacticalHierarchies& operator=( const AutomatTacticalHierarchies& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void DoUpdate( const ASN1T_MsgAutomatChangeSuperior& message );
    virtual void DoUpdate( const kernel::InstanciationComplete& message );
    virtual void MergeSymbol( const kernel::Entity_ABC& entity );
    static std::string Max( const std::string& lhs, const std::string& rhs );
    static std::string Increase( const std::string& value );
    static char Level( const std::string& value );
    static unsigned Count( const std::string& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    const kernel::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    kernel::Entity_ABC& superior_;
    std::string level_;
    //@}
};

#endif // __AutomatTacticalHierarchies_h_
