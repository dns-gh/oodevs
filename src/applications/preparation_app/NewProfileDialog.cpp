// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "preparation_app_pch.h"
#include "NewProfileDialog.h"
#include "moc_NewProfileDialog.cpp"
#include "ControlsChecker_ABC.h"
#include "preparation/ProfilesModel.h"
#include "clients_kernel/ExtensionType.h"
#include "clients_kernel/ExtensionTypes.h"
#include "clients_kernel/DictionaryType.h"

// -----------------------------------------------------------------------------
// Name: NewProfileDialog constructor
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
NewProfileDialog::NewProfileDialog( QWidget* parent, ProfilesModel& model, ControlsChecker_ABC& checker,
                                    const kernel::ExtensionTypes& extensions )
    : QDialog( parent )
    , model_       ( model )
    , checker_     ( checker )
    , extensions_  ( extensions )
    , userRoleDico_( 0 )
{
    setCaption( tr( "New profile") );
    QGridLayout* mainLayout = new QGridLayout( this );
    value_ = new QLineEdit();
    connect( value_, SIGNAL( textChanged( const QString& ) ), this, SLOT( UpdateText() ) );
    mainLayout->addWidget( new QLabel( tr( "Profile name:" ) ), 0, 0 );
    mainLayout->addWidget( value_, 0, 1 );
    userRole_ = new QComboBox();
    mainLayout->addWidget( new QLabel( tr( "Role:" ) ), 1, 0 );
    mainLayout->addWidget( userRole_, 1, 1 );
    warningLabel_ = new QLabel();
    mainLayout->addWidget( warningLabel_, 2, 0, 1, 2, Qt::AlignCenter );
    okButton_ = new QDialogButtonBox( QDialogButtonBox::Ok );
    connect( okButton_, SIGNAL( accepted() ), this, SLOT( Validate() ) );
    okButton_->setFocus();
    QDialogButtonBox* cancelButton = new QDialogButtonBox( QDialogButtonBox::Cancel );
    connect( cancelButton, SIGNAL( rejected() ), this, SLOT( reject() ) );
    mainLayout->addWidget( okButton_, 3, 0, Qt::AlignCenter );
    mainLayout->addWidget( cancelButton, 3, 1, Qt::AlignCenter );
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog destructor
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
NewProfileDialog::~NewProfileDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog::UpdateText
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
void NewProfileDialog::UpdateText()
{
    QString name= value_->text();
    if( name.isEmpty() )
    {
        warningLabel_->setText( "<font color=\"#FF0000\">" + tr( "Profile name is empty." ) + "</font>" );
        okButton_->setEnabled( false );
    }
    else
    {
        
        if( model_.Exists( name ) || checker_.Exists( name ) )
        {
            warningLabel_->setText( "<font color=\"#FF0000\">" + tr( "Profile '%1' already exists." ).arg( name ) + "</font>" );
            okButton_->setEnabled( false );
        }
        else
        {
            if( !userRoleDico_ || userRole_->currentText().isEmpty() )
            {
                warningLabel_->setText( "<font color=\"#FF0000\">" + tr( "No role defined." ) + "</font>" );
                okButton_->setEnabled( false );
            }
            else
            {
                warningLabel_->setText( "" );
                okButton_->setEnabled( true );
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog::Exec
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
void NewProfileDialog::Exec()
{
    value_->setText( BuildUniqueLogin() );
    exec();
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog::BuildUniqueLogin
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
QString NewProfileDialog::BuildUniqueLogin() const
{
    static const QString defaultName = tr( "New profile" );
    QString name;
    int i = 0;
    while( model_.Exists( name = defaultName + ( i > 0 ? " (" + QString::number( i ) + ")" : "" ) ) )
        ++i;
    return name;
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog::Validate
// Created: LGY 2011-12-08
// -----------------------------------------------------------------------------
void NewProfileDialog::Validate()
{
    if( userRoleDico_ )
    {
        model_.CreateProfile( value_->text(), userRoleDico_->GetKey( userRole_->currentText().ascii(), dicoKind_, dicoLanguage_ ) );
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog::Load
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void NewProfileDialog::Load()
{
    userRole_->clear();
    kernel::ExtensionType* type = extensions_.tools::StringResolver< kernel::ExtensionType >::Find( "profile-attributes" );
    if( type )
    {
        tools::Iterator< const kernel::AttributeType& > it = type->CreateIterator();
        while( it.HasMoreElements() )
        {
            const kernel::AttributeType& attribute = it.NextElement();
            if( attribute.GetType() == kernel::AttributeType::ETypeDictionary )
            {
                std::string dictionary;
                attribute.GetDictionaryValues( dictionary, dicoKind_, dicoLanguage_ );
                userRoleDico_ = extensions_.tools::StringResolver< kernel::DictionaryType >::Find( dictionary );
                if( userRoleDico_ )
                {
                    QStringList list;
                    userRoleDico_->GetStringList( list, dicoKind_, dicoLanguage_ );
                    userRole_->insertStringList( list );
                }
            }
        }
    }
}

// -----------------------------------------------------------------------------
// Name: NewProfileDialog::Unload
// Created: LGY 2011-12-12
// -----------------------------------------------------------------------------
void NewProfileDialog::Unload()
{
    userRole_->clear();
}
