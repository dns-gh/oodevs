// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "PopulationRepartitionEditor.h"
#include "moc_PopulationRepartitionEditor.cpp"
#include "clients_kernel/Controller.h"
#include "clients_gui/RichPushButton.h"
#include "clients_gui/RichLineEdit.h"
#include "clients_gui/RichGroupBox.h"
#include "preparation/PopulationRepartition.h"

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor constructor
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
PopulationRepartitionEditor::PopulationRepartitionEditor( QWidget* parent, kernel::Controller& controller )
    : gui::PropertyDialog( parent )
    , controller_( controller )
{
    gui::SubObjectName subObject( "PopulationRepartitionEditor" );
    setCaption( tr( "Repartition Editor" ) );

    gui::RichGroupBox* repartitionBox = new gui::RichGroupBox( "repartitionBox", this );
    QHBoxLayout* repartitionBoxLayout = new QHBoxLayout( repartitionBox );
    {
        QLabel* maleLabel = new QLabel( tr( "Male (%)" ) );

        male_ = new gui::RichLineEdit( "male" );
        male_->setValidator( new QIntValidator( 0, 100, male_ ) );
        male_->setFixedWidth( 40 );
        connect( male_, SIGNAL( textChanged( const QString& ) ), SLOT( OnValueChanged() ) );

        repartitionBoxLayout->addWidget( maleLabel );
        repartitionBoxLayout->addWidget( male_ );
    }
    {
        QLabel* femaleLabel = new QLabel( tr( "Female (%)" ) );
        female_ = new gui::RichLineEdit( "female" );
        female_->setValidator( new QIntValidator( 0, 100, female_ ) );
        female_->setFixedWidth( 40 );
        connect( female_, SIGNAL( textChanged( const QString& ) ), SLOT( OnValueChanged() ) );

        repartitionBoxLayout->addWidget( femaleLabel );
        repartitionBoxLayout->addWidget( female_ );
    }
    {
        QLabel* childrenLabel = new QLabel( tr( "Children (%)" ) );
        children_ = new gui::RichLineEdit( "children" );
        children_->setValidator( new QIntValidator( 0, 100, children_ ) );
        children_->setFixedWidth( 40 );
        connect( children_, SIGNAL( textChanged( const QString& ) ), SLOT( OnValueChanged() ) );

        repartitionBoxLayout->addWidget( childrenLabel );
        repartitionBoxLayout->addWidget( children_ );
    }
    warning_ = new QLabel( this );


    //button box
    Q3HBox* pbuttonBox = new Q3HBox( this );

    okBtn_ = new gui::RichPushButton( "ok", tr( "Ok" ), pbuttonBox );
    okBtn_->setDefault( true );
    okBtn_->setMaximumWidth( 60 );
    connect( okBtn_   , SIGNAL( clicked() ), SLOT( OnAccept() ) );

    gui::RichPushButton* cancelBtn = new gui::RichPushButton( "cancel", tr( "Cancel" ), pbuttonBox );
    cancelBtn->setMaximumWidth( 60 );
    connect( cancelBtn, SIGNAL( clicked() ), SLOT( OnReject() ) );

    //main Layout
    QVBoxLayout* pMainLayout = new QVBoxLayout( this );
    pMainLayout->setMargin( 10 );
    pMainLayout->setSpacing( 10 );
    pMainLayout->addWidget( repartitionBox );
    pMainLayout->addWidget( warning_ );
    pMainLayout->addWidget( pbuttonBox, 0, Qt::AlignRight );



}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor destructor
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
PopulationRepartitionEditor::~PopulationRepartitionEditor()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::SetValue
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void PopulationRepartitionEditor::SetValue( PopulationRepartition*& value )
{
    value_ = &value;
    male_->setText( locale().toString( value->male_ ) );
    female_->setText( locale().toString( value->female_ ) );
    children_->setText( locale().toString( value->children_ ) );
    OnValueChanged();
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::GetValue
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
PopulationRepartition* PopulationRepartitionEditor::GetValue()
{
    return *value_;
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::OnValueChanged
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void PopulationRepartitionEditor::OnValueChanged()
{
    bool valid = ( male_->text().toInt() + female_->text().toInt() + children_->text().toInt() == 100 );
    okBtn_->setEnabled( valid );
    warning_->setText( valid ? QString() : tr( "<font color=\"#ff0000\">Total must be equal to 100</font>" ) );
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::OnAccept
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void PopulationRepartitionEditor::OnAccept()
{
    ( *value_ )->male_ = male_->text().toUInt();
    ( *value_ )->female_ = female_->text().toUInt();
    ( *value_ )->children_ = children_->text().toUInt();
    controller_.Update( **value_ );
    accept();
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::OnReject
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
void PopulationRepartitionEditor::OnReject()
{
    reject();
}

// -----------------------------------------------------------------------------
// Name: PopulationRepartitionEditor::sizeHint
// Created: JSR 2011-03-08
// -----------------------------------------------------------------------------
QSize PopulationRepartitionEditor::sizeHint() const
{
    return QSize( 250, 150 );
}
