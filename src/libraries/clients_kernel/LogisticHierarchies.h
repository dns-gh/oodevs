// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef LogisticHierarchies_h
#define LogisticHierarchies_h

#include "clients_gui/EntityHierarchies.h"
#include "clients_gui/LogisticHierarchiesBase.h"
#include <tools/Resolver_ABC.h>

namespace sword
{
    class ChangeLogisticLinks;
}

namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;

// =============================================================================
/** @class  LogisticHierarchies
    @brief  LogisticHierarchies
*/
// Created: SLI 2014-08-19
// =============================================================================
class LogisticHierarchies : public gui::EntityHierarchies< gui::LogisticHierarchiesBase >
                          , public kernel::Updatable_ABC< sword::ChangeLogisticLinks >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticHierarchies( kernel::Controller& controller, kernel::Entity_ABC& entity,
                                  const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                                  const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver );
    virtual ~LogisticHierarchies();
    //@}

private:
    //! @name Operations
    //@{
    virtual void DoUpdate( const sword::ChangeLogisticLinks& message );
    virtual void SetLogisticSuperior( const kernel::LogisticBaseSuperior& );
    virtual void Load( xml::xistream&, const kernel::Entity_ABC* );
    //@}

private:
    //! @name Member data
    //@{
    const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver_;
    const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver_;
    gui::LogisticHierarchiesBase* currentSuperior_;
    //@}
};

}

#endif // LogisticHierarchies_h
