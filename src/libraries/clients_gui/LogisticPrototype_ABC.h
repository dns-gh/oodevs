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
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "ValuedComboBox.h"
#include <tools/Observer_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;
    class Entity_ABC;
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
                            , public tools::Observer_ABC
                            , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                            , public tools::ElementObserver_ABC< kernel::Formation_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
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
    virtual bool CheckValidity( const kernel::Team_ABC& ) const;
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
    virtual void NotifyUpdated( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );
    virtual void NotifyContextMenu( const kernel::Automat_ABC&, kernel::ContextMenu& );

    virtual void NotifyCreated( const kernel::Formation_ABC& formation );
    virtual void NotifyUpdated( const kernel::Formation_ABC& formation );
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    virtual void NotifyContextMenu( const kernel::Formation_ABC&, kernel::ContextMenu& );

    void UpdateDisplayName( const kernel::Entity_ABC& entity );
    //@}

protected:
    //! @name Member Data
    //@{
    kernel::Controllers& controllers_;
    ValuedComboBox< const kernel::Entity_ABC* >* logSuperiors_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    //@}
};

}

#endif // __LogisticPrototype_ABC_h_
