// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticSuperiorEditor_h_
#define __LogisticSuperiorEditor_h_

#include "clients_gui/ValuedComboBox.h"
#include "clients_gui/ValueEditor.h"
#include "clients_kernel/SubTypes.h"
#include "tools/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Ghost_ABC;
}

// =============================================================================
/** @class  LogisticSuperiorEditor
    @brief  LogisticSuperiorEditor
*/
// Created: SBO 2006-10-25
// =============================================================================
class LogisticSuperiorEditor : public gui::ValuedComboBox< const kernel::Entity_ABC* >
                             , public gui::ValueEditor< kernel::LogisticBaseSuperior >
                             , public tools::Observer_ABC
                             , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                             , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                             , public tools::ElementObserver_ABC< kernel::Ghost_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticSuperiorEditor( QWidget* parent, kernel::Controllers& controllers,
                                     const tools::Resolver_ABC< kernel::Automat_ABC >& automatResolver,
                                     const tools::Resolver_ABC< kernel::Formation_ABC >& formationResolver,
                                     const tools::Resolver_ABC< kernel::Ghost_ABC >& ghostResolver,
                                     const kernel::Entity_ABC& selected );
    virtual ~LogisticSuperiorEditor();
    //@}

    //! @name Operations
    //@{
    virtual kernel::LogisticBaseSuperior GetValue();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Formation_ABC& formation );
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );

    virtual void NotifyCreated( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );

    virtual void NotifyCreated( const kernel::Ghost_ABC& automat );
    virtual void NotifyDeleted( const kernel::Ghost_ABC& automat );

    template< typename T >
    bool IsValidSuperior( const T& superiorToTest ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Entity_ABC& selected_;
    //@}
};

#endif // __LogisticSuperiorEditor_h_
