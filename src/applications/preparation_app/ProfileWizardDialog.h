// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileWizardDialog_h_
#define __ProfileWizardDialog_h_

#include "clients_gui/ModalDialog.h"

class Model;
class ProfilesModel;
class ProfilesGenerator;

// =============================================================================
/** @class  ProfileWizardDialog
    @brief  ProfileWizardDialog
*/
// Created: SBO 2007-11-07
// =============================================================================
class ProfileWizardDialog : public ModalDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ProfileWizardDialog( QWidget* parent, const Model& model );
    virtual ~ProfileWizardDialog();
    //@}

    //! @name Operations
    //@{
    virtual QSize sizeHint () const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnAccept();
    void OnChecked();
    //@}

private:
    //! @name Helpers
    //@{
    void OnConfirmation( int result );
    //@}

private:
    //! @name Types
    //@{
    enum
    {
        eANIBAS,
        eENIEX,
        eSUPERVISOR,
        eSIDESUPERVISOR,
        eDIREX,
        eANALYSIS,
        eGESTIM,
        eNbRoles
    };
    //@}

    //! @name Member data
    //@{
    std::auto_ptr< ProfilesGenerator > generator_;
    ProfilesModel& profiles_;
    std::auto_ptr< QMessageBox > confirmation_;
    QPushButton* okBtn_;
    QCheckBox* checkBoxes_[ eNbRoles ];
    //@}
};

#endif // __ProfileWizardDialog_h_
