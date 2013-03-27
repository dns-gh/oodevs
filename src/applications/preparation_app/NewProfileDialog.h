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
class ProfilesChecker_ABC;

namespace gui
{
    class RichLineEdit;
}

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
             NewProfileDialog( QWidget* parent, ProfilesModel& model, ProfilesChecker_ABC& checker );
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
    ProfilesChecker_ABC& checker_;
    gui::RichLineEdit* value_;
    QDialogButtonBox* okButton_;
    QLabel* warningLabel_;
    //@}
};

#endif // __NewProfileDialog_h_
