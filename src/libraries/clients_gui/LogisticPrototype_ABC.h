// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogisticPrototype_ABC_h_
#define __LogisticPrototype_ABC_h_

#include "ObjectAttributePrototype_ABC.h"
#include "clients_kernel/Resolver.h"
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "ValuedComboBox.h"

namespace kernel
{
    class Automat_ABC;
    class Controllers;
}

namespace gui
{

// =============================================================================
/** @class  LogisticPrototype_ABC
    @brief  LogisticPrototype_ABC
*/
// Created: SBO 2006-04-19
// =============================================================================
class LogisticPrototype_ABC : public ObjectAttributePrototype_ABC
                            , public kernel::Observer_ABC
                            , public kernel::ElementObserver_ABC< kernel::Automat_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructor/Destructor
    //@{
             LogisticPrototype_ABC( QWidget* parent, kernel::Controllers& controllers );
    virtual ~LogisticPrototype_ABC();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity() const;
    //@}

private:
    //! @name Copy constructor/Assignment
    //@{
    LogisticPrototype_ABC( const LogisticPrototype_ABC& );
    LogisticPrototype_ABC& operator=( const LogisticPrototype_ABC& );
    //@}

private slots:
    //! @name Slots
    //@{
    void SetSelected();
    void SelectionChanged();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );
    virtual void NotifyContextMenu( const kernel::Automat_ABC&, kernel::ContextMenu& );
    //@}

protected:
    //! @name Member Data
    //@{
    kernel::Controllers& controllers_;
    ValuedComboBox< const kernel::Automat_ABC* >* tc2s_;
    kernel::SafePointer< kernel::Automat_ABC > selected_;
    //@}
};

}

#endif // __LogisticPrototype_ABC_h_
