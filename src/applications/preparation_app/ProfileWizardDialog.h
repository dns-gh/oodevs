// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProfileWizardDialog_h_
#define __ProfileWizardDialog_h_

class Model;
class ProfilesModel;
class ProfilesGenerator;

// =============================================================================
/** @class  ProfileWizardDialog
    @brief  ProfileWizardDialog
*/
// Created: SBO 2007-11-07
// =============================================================================
class ProfileWizardDialog : public QDialog
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ProfileWizardDialog( QWidget* parent, const Model& model, ProfilesModel& profiles );
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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ProfileWizardDialog( const ProfileWizardDialog& );            //!< Copy constructor
    ProfileWizardDialog& operator=( const ProfileWizardDialog& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< ProfilesGenerator > generator_;
    ProfilesModel& profiles_;
    Q3ButtonGroup* profileTypes_;
    Q3ButtonGroup* creationMode_;
    Q3ButtonGroup* creationRight_;
    //@}
};

#endif // __ProfileWizardDialog_h_
