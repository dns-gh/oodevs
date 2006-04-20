//****************************************************************************
//
// $Created:  NLD 2002-01-03 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ChangeDiplomacyDialog.h $
// $Author: Age $
// $Modtime: 6/04/05 15:17 $
// $Revision: 1 $
// $Workfile: ChangeDiplomacyDialog.h $
//
//*****************************************************************************

#ifndef __ChangeDiplomacyDialog_h_
#define __ChangeDiplomacyDialog_h_

#include "ElementObserver_ABC.h"
#include "ContextMenuObserver_ABC.h"
#include "ValuedComboBox.h"
#include "ASN_Types.h"

class Team;
class Controllers;

//=============================================================================
// Created:  NLD 2002-01-03 
//=============================================================================
class ChangeDiplomacyDialog : public QDialog
                            , private Observer_ABC
                            , public ElementObserver_ABC< Team >
                            , public ContextMenuObserver_ABC< Team >
{
    Q_OBJECT

public:
    //! @name Constructor/Destructor
    //@{
             ChangeDiplomacyDialog( QWidget* parent, Controllers& controllers );
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
    virtual void NotifyCreated( const Team& );
    virtual void NotifyDeleted( const Team& );
    virtual void NotifyContextMenu( const Team&, QPopupMenu& );
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
    ValuedComboBox< unsigned long >*      pArmy1ComboBox_;
    ValuedComboBox< unsigned long >*      pArmy2ComboBox_;
    ValuedComboBox<ASN1T_EnumDiplomatie>* pDiplomacyComboBox_;
    //@}
};

#endif // __ChangeDiplomacyDialog_h_
