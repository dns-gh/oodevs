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
#include "game_asn/Asn.h"
#include "clients_gui/ValuedComboBox.h"

namespace kernel
{
    class Team_ABC;
    class Controllers;
    class Profile_ABC;
}

class Publisher_ABC;

// =============================================================================
/** @class  ChangeDiplomacyDialog
    @brief  ChangeDiplomacyDialog
*/
// Created: AGE 2006-10-11
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
             ChangeDiplomacyDialog( QWidget* parent, kernel::Controllers& controllers, Publisher_ABC& publisher, const kernel::Profile_ABC& profile );
    virtual ~ChangeDiplomacyDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    void Reject();
    void UpdateDiplomacy();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const kernel::Team_ABC& );
    virtual void NotifyDeleted( const kernel::Team_ABC& );
    virtual void NotifyContextMenu( const kernel::Team_ABC&, kernel::ContextMenu& );
    //@}

private:
    //! @name Copy / Assignment
    //@{
    ChangeDiplomacyDialog( const ChangeDiplomacyDialog& );
    ChangeDiplomacyDialog& operator=( const ChangeDiplomacyDialog& );
    //@}
  
private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Publisher_ABC& publisher_;
    const kernel::Profile_ABC& profile_;

    gui::ValuedComboBox< const kernel::Team_ABC* >* pArmy1ComboBox_;
    gui::ValuedComboBox< const kernel::Team_ABC* >* pArmy2ComboBox_;
    gui::ValuedComboBox< ASN1T_EnumDiplomatie >*    pDiplomacyComboBox_;
    //@}
};

#endif // __ChangeDiplomacyDialog_h_
