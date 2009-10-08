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
#include "clients_kernel/ValueEditor.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "tools/Resolver_ABC.h"
#include "preparation/LogisticSuperior.h"

// =============================================================================
/** @class  LogisticSuperiorEditor
    @brief  LogisticSuperiorEditor
*/
// Created: SBO 2006-10-25
// =============================================================================
template< typename Superior >
class LogisticSuperiorEditor : public gui::ValuedComboBox< const kernel::Automat_ABC* >
                             , public kernel::ValueEditor< Superior >
                             , public kernel::Observer_ABC
                             , public kernel::ElementObserver_ABC< kernel::Automat_ABC >
{

public:
    //! @name Constructors/Destructor
    //@{
             LogisticSuperiorEditor( QWidget* parent, kernel::Controllers& controllers, const tools::Resolver_ABC< kernel::Automat_ABC >& resolver, const kernel::Entity_ABC& selected );
    virtual ~LogisticSuperiorEditor();
    //@}

    //! @name Operations
    //@{
    virtual Superior GetValue();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LogisticSuperiorEditor( const LogisticSuperiorEditor& );            //!< Copy constructor
    LogisticSuperiorEditor& operator=( const LogisticSuperiorEditor& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    LogisticSuperior filter_;
    //@}
};

#include "LogisticSuperiorEditor.inl"

#endif // __LogisticSuperiorEditor_h_
