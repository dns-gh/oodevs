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
#include "gaming/ASN_Types.h"
#include "clients_gui/ValuedComboBox.h"

class Team_ABC;
class Controllers;
class Publisher_ABC;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class ChangeDiplomacyDialog : public QDialog
                            , public Observer_ABC
                            , public ElementObserver_ABC< Team_ABC >
                            , public ContextMenuObserver_ABC< Team_ABC >
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             ChangeDiplomacyDialog( QWidget* parent, Controllers& controllers, Publisher_ABC& publisher );
    virtual ~ChangeDiplomacyDialog();
    //@}

private slots:
    //! @name Slots
    //@{
    void Validate();
    void Reject  ();
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyCreated( const Team_ABC& );
    virtual void NotifyDeleted( const Team_ABC& );
    virtual void NotifyContextMenu( const Team_ABC&, ContextMenu& );
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
    Controllers& controllers_;
    Publisher_ABC& publisher_;
    ValuedComboBox< unsigned long >*      pArmy1ComboBox_;
    ValuedComboBox< unsigned long >*      pArmy2ComboBox_;
    ValuedComboBox<ASN1T_EnumDiplomatie>* pDiplomacyComboBox_;
    //@}
};

#endif // __ChangeDiplomacyDialog_h_
