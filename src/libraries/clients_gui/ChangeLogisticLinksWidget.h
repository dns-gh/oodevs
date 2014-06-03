// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef __ChangeLogisticLinksWidget_h_
#define __ChangeLogisticLinksWidget_h_

#include "clients_gui/ValuedComboBox.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
}

namespace gui
{

// =============================================================================
/** @class  ChangeLogisticLinksWidget
    @brief  ChangeLogisticLinksWidget
*/
// Created: ABR 2014-06-02
// =============================================================================
class ChangeLogisticLinksWidget : public QWidget
                                , public tools::Observer_ABC
                                , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                                , public tools::ElementObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ChangeLogisticLinksWidget( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ChangeLogisticLinksWidget();
    //@}

    //! @name Operations
    //@{
    void SetCurrentVisible( bool visible );

    void AddItem( const kernel::Entity_ABC& entity );
    void RemoveItem( const kernel::Entity_ABC& entity );

    void SetNominalSuperior( const kernel::Entity_ABC* entity );
    void SetCurrentSuperior( const kernel::Entity_ABC* entity );

    const kernel::Entity_ABC* GetNominalSuperior() const;
    const kernel::Entity_ABC* GetCurrentSuperior() const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Automat_ABC& agent );
    virtual void NotifyDeleted( const kernel::Automat_ABC& agent );
    virtual void NotifyCreated( const kernel::Formation_ABC& agent );
    virtual void NotifyDeleted( const kernel::Formation_ABC& agent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    gui::ValuedComboBox< const kernel::Entity_ABC* >* nominalCombo_;
    QLabel* currentColor_;
    QLabel* currentLabel_;
    gui::ValuedComboBox< const kernel::Entity_ABC* >* currentCombo_;
    //@}
};

} //! namespace gui

#endif // __ChangeLogisticLinksWidget_h_
