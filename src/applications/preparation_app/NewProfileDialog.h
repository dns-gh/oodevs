// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __NewProfileDialog_h_
#define __NewProfileDialog_h_

class ProfilesModel;
class ControlsChecker_ABC;

// =============================================================================
/** @class  NewProfileDialog
    @brief  New profile dialog
*/
// Created: LGY 2011-06-24
// =============================================================================
class NewProfileDialog : public QDialog
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             NewProfileDialog( QWidget* parent, ProfilesModel& model, ControlsChecker_ABC& checker );
    virtual ~NewProfileDialog();
    //@}

    //! @name Operations
    //@{
    void Exec();
    //@}

private slots:
    //! @name Helpers
    //@{
    void UpdateText();
    void Validate();
    //@}

private:
    //! @name Helpers
    //@{
    QString BuildUniqueLogin() const;
    //@}

private:
    //! @name Member Data
    //@{
    ProfilesModel& model_;
    ControlsChecker_ABC& checker_;
    QLineEdit* value_;
    QDialogButtonBox* okButton_;
    QLabel* warningLabel_;
    //@}
};

#endif // __NewProfileDialog_h_
