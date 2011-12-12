// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "ProfileWizardDialog.h"
#include "moc_ProfileWizardDialog.cpp"
#include "preparation/ProfilesGenerator.h"
#include "preparation/ProfilesModel.h"
#include "icons.h"
#include <boost/bind.hpp>
#include <boost/function.hpp>

namespace
{
    class ConfirmationBox : public QMessageBox
    {
    private:
        typedef boost::function1< void, int > T_Callback;

    public:
        ConfirmationBox( const QString& title, const QString& message, T_Callback callback )
            : QMessageBox( title, message, QMessageBox::Warning, QMessageBox::Yes, QMessageBox::No | QMessageBox::Default, Qt::NoButton )
            , callback_( callback )
        {
            hide();
        }

        virtual void done( int result )
        {
            callback_( result );
            hide();
        }
    private:
        T_Callback callback_;
    };
}

// -----------------------------------------------------------------------------
// Name: ProfileWizardDialog constructor
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
ProfileWizardDialog::ProfileWizardDialog( QWidget* parent, const Model& model, ProfilesModel& profiles )
    : QDialog( parent, "ProfileWizardDialog" )
    , generator_   ( new ProfilesGenerator( model, profiles ) )
    , profiles_    ( profiles )
    , confirmation_( new ConfirmationBox( tr( "Warning" ), tr( "All existing profiles will be deleted, do you want to continue?" ), boost::bind( &ProfileWizardDialog::OnConfirmation, this, _1 ) ) )
{
    setCaption( tr( "User profiles creation wizard" ) );

    QGridLayout* grid = new QGridLayout( this );
    {
        QHBoxLayout* layout = new QHBoxLayout;
        grid->addLayout( layout, 0, 0, 1, 3 );
        QLabel* title = new QLabel( caption() );
        layout->addWidget( title );
        QFont font( "Arial", 16, QFont::Bold );
        title->setFont( font );
        title->setMargin( 10 );
        title->setBackgroundColor( Qt::white );
        QLabel* icon = new QLabel();
        layout->addWidget( icon );
        icon->setPixmap( MAKE_PIXMAP( profiles ) );
        icon->setMaximumWidth( 64 );
        icon->setBackgroundColor( Qt::white );
    }

    {
        QGroupBox* box = new QGroupBox( tr( "Select profiles to generate: " ) );
        grid->addWidget( box, 1, 0, 1, 3 );
        QVBoxLayout* layout = new QVBoxLayout( box );
        for( int i = 0; i < eNbRoles; ++i )
        {
            checkBoxes_[ i ] = new QCheckBox;
            checkBoxes_[ i ]->setChecked( true );
            layout->addWidget( checkBoxes_[ i ] );
            connect( checkBoxes_[ i ], SIGNAL( stateChanged( int ) ), SLOT( OnChecked() ) );
        }
        checkBoxes_[ eANIBAS         ]->setText( tr( "one ANIBAS profile per low-level friend formation" ) );
        checkBoxes_[ eENIEX          ]->setText( tr( "one ENIEX profile per low-level enemy formation" ) );
        checkBoxes_[ eSUPERVISOR     ]->setText( tr( "one global SUPERVISOR profile" ) );
        checkBoxes_[ eSIDESUPERVISOR ]->setText( tr( "one SUPERVISOR profile per side" ) );
        checkBoxes_[ eDIREX          ]->setText( tr( "one DIREX profile" ) );
        checkBoxes_[ eANALYSIS       ]->setText( tr( "one ANALYSIS profile" ) );
    }

    {
        QHBoxLayout* layout = new QHBoxLayout;
        grid->addLayout( layout, 2, 2, 1, 1, Qt::AlignRight );
        layout->setMargin( 5 );
        okBtn_ = new QPushButton( tr( "Ok" ) );
        layout->addWidget( okBtn_ );
        okBtn_->setDefault( true );
        QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ) );
        layout->addWidget( cancelBtn );

        connect( okBtn_,    SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancelBtn, SIGNAL( clicked() ), SLOT( reject() ) );
    }
    hide();
}

// -----------------------------------------------------------------------------
// Name: ProfileWizardDialog destructor
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
ProfileWizardDialog::~ProfileWizardDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ProfileWizardDialog::sizeHint
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
QSize ProfileWizardDialog::sizeHint() const
{
    return QSize( 350, 250 );
}

// -----------------------------------------------------------------------------
// Name: ProfileWizardDialog::OnAccept
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
void ProfileWizardDialog::OnAccept()
{
    confirmation_->show();
    accept();
}

// -----------------------------------------------------------------------------
// Name: ProfileWizardDialog::OnChecked
// Created: JSR 2011-12-07
// -----------------------------------------------------------------------------
void ProfileWizardDialog::OnChecked()
{
    for( int i = 0; i < eNbRoles; ++i )
        if( checkBoxes_[ i ]->isChecked() )
        {
            okBtn_->setEnabled( true );
            return;
        }
    okBtn_->setEnabled( false );
}

// -----------------------------------------------------------------------------
// Name: ProfileWizardDialog::OnConfirmation
// Created: JSR 2011-12-07
// -----------------------------------------------------------------------------
void ProfileWizardDialog::OnConfirmation( int result )
{
    if( result == QMessageBox::Yes )
    {
        profiles_.Purge();
        if( checkBoxes_[ eANIBAS ]->isChecked() )
            generator_->GenerateANIBAS();
        if( checkBoxes_[ eENIEX ]->isChecked() )
            generator_->GenerateENIEX();
        if( checkBoxes_[ eSUPERVISOR ]->isChecked() )
            generator_->GenerateSUPERVISOR();
        if( checkBoxes_[ eSIDESUPERVISOR ]->isChecked() )
            generator_->GenerateSIDESUPERVISOR();
        if( checkBoxes_[ eDIREX ]->isChecked() )
            generator_->GenerateDIREX();
        if( checkBoxes_[ eANALYSIS ]->isChecked() )
            generator_->GenerateANALYSIS();
        if( profiles_.CheckUnicityAndRename() )
            QMessageBox::warning( this, tr( "Warning" ), tr( "Some profiles have similar names. Please edit them manually.") );
    }
}
