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

// -----------------------------------------------------------------------------
// Name: ProfileWizardDialog constructor
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
ProfileWizardDialog::ProfileWizardDialog( QWidget* parent, const Model& model, ProfilesModel& profiles )
    : QDialog( parent, "ProfileWizardDialog" )
    , generator_( new ProfilesGenerator( model, profiles ) )
    , profiles_( profiles )
{
    setCaption( tr( "User profiles creation wizard" ) );

    Q3GridLayout* grid = new Q3GridLayout( this, 3, 1 );
    grid->setRowStretch( 0, 1 );
    grid->setRowStretch( 1, 5 );
    grid->setRowStretch( 2, 1 );

    {
        Q3HBox* box = new Q3HBox( this );
        QLabel* title = new QLabel( caption(), box );
        QFont font( "Arial", 16, QFont::Bold );
        title->setFont( font );
        title->setMargin( 10 );
        title->setBackgroundColor( Qt::white );
        QLabel* icon = new QLabel( box );
        icon->setPixmap( MAKE_PIXMAP( profiles ) );
        icon->setMaximumWidth( 64 );
        icon->setBackgroundColor( Qt::white );
        grid->addWidget( box, 0, 0 );
    }

    {
        Q3VBox* box = new Q3VBox( this );
        box->setMargin( 5 );
        profileTypes_ = new Q3VButtonGroup( tr( "Select profiles to generate: " ), box );
        profileTypes_->setRadioButtonExclusive( true );
        profileTypes_->insert( new QRadioButton( tr( "one profile per side" ), profileTypes_ )               , 0 );
        profileTypes_->insert( new QRadioButton( tr( "one profile per top-level formation" ), profileTypes_ ), 1 );
        profileTypes_->insert( new QRadioButton( tr( "one profile per formation" ), profileTypes_ )          , 2 );
        profileTypes_->setButton( 0 );

        creationMode_ = new Q3HButtonGroup( tr( "Select creation mode: " ), box );
        creationMode_->setRadioButtonExclusive( true );
        creationMode_->insert( new QRadioButton( tr( "clear existing profiles" ), creationMode_ )     , 0 );
        creationMode_->insert( new QRadioButton( tr( "add new profiles to existing" ), creationMode_ ), 1 );
        creationMode_->setButton( 0 );

        creationRight_ = new Q3HButtonGroup( tr( "Select creation permissions: " ), box );
        creationRight_->setRadioButtonExclusive( true );
        creationRight_->insert( new QRadioButton( tr( "writeable" ), creationRight_ ), 0 );
        creationRight_->insert( new QRadioButton( tr( "readonly" ) , creationRight_ ), 1 );
        creationRight_->setButton( 0 );

        grid->addWidget( box, 1, 0 );
    }

    {
        Q3HBox* box = new Q3HBox( this );
        box->setMargin( 5 );
        box->setMaximumHeight( 40 );
        QPushButton* okBtn = new QPushButton( tr( "Ok" ), box );
        okBtn->setDefault( true );
        QPushButton* cancelBtn = new QPushButton( tr( "Cancel" ), box );
        grid->addWidget( box, 2, 0, Qt::AlignRight );

        connect( okBtn, SIGNAL( clicked() ), SLOT( OnAccept() ) );
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
    return QSize( 350, 350 );
}

// -----------------------------------------------------------------------------
// Name: ProfileWizardDialog::OnAccept
// Created: SBO 2007-11-07
// -----------------------------------------------------------------------------
void ProfileWizardDialog::OnAccept()
{
    const bool clear    = creationMode_ ->selectedId() == 0;
    const bool readonly = creationRight_->selectedId() == 1;

    if( clear )
        profiles_.Purge();
    switch( profileTypes_->selectedId() )
    {
    default:
    case 0: // one profile per team
        generator_->GenerateTeams( readonly );
        break;
    case 1: // one profile per top-level formation
        generator_->GenerateTopLevelFormations( readonly );
        break;
    case 2: // one profile per formation
        generator_->GenerateFormations( readonly );
        break;
    }
    accept();
}
