// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************


#ifndef __ChangeDiplomacyDialog_h_
#define __ChangeDiplomacyDialog_h_

#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_gui/ValuedComboBox.h"

namespace kernel
{
    class Team_ABC;
    class Controllers;
}

class Diplomacy;

// =============================================================================
/** @class  ChangeDiplomacyDialog
    @brief  ChangeDiplomacyDialog
*/
// Created: SBO 2006-09-07
// =============================================================================
class ChangeDiplomacyDialog : public QDialog
                            , public kernel::Observer_ABC
                            , public kernel::ElementObserver_ABC< kernel::Team_ABC >
                            , public kernel::ContextMenuObserver_ABC< kernel::Team_ABC >
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             ChangeDiplomacyDialog( QWidget* parent, kernel::Controllers& controllers );
    virtual ~ChangeDiplomacyDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    void Reject();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& );
    virtual void NotifyDeleted( const kernel::Team_ABC& );
    virtual void NotifyContextMenu( const kernel::Team_ABC&, kernel::ContextMenu& );

    virtual void showEvent( QShowEvent* );
    virtual QSize sizeHint();
    //@}

private:
    //! @name Copy / Assignment
    //@{
    ChangeDiplomacyDialog( const ChangeDiplomacyDialog& );
    ChangeDiplomacyDialog& operator=( const ChangeDiplomacyDialog& );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const kernel::Team_ABC* > T_Teams;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    QTable* table_;
    T_Teams teams_;
    QStringList list_;
    //@}
};

#endif // __ChangeDiplomacyDialog_h_
