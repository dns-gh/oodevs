// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "gaming_app_pch.h"
#include "LoginDialog.h"
#include "moc_LoginDialog.cpp"
#include "gaming/Network.h"
#include "gaming/Profile.h"
#include "gaming/Tools.h"
#include "tools/ExerciseConfig.h"
#include <xeumeuleu/xml.hpp>

namespace
{
    class UserItem : public QIconViewItem
    {
    public:
        UserItem( QIconView* parent, xml::xistream& xis )
            : QIconViewItem( parent )
            , login_( xml::attribute< std::string >( xis, "name" ).c_str() )
            , supervisor_( xml::attribute< bool >( xis, "supervision", false ) )
            , password_( ! xml::attribute< std::string >( xis, "password", "" ).empty() )
        {
            setText( login_.isEmpty() ? tools::translate( "LoginDialog", "Anonymous" ) : login_ );
            const QString pixmap = QString( "images/gaming/profile/%1%2.png" ).arg( supervisor_ ? "supervisor" : "standard" )
                                                                              .arg( password_ ? "_password" : "" );
            setPixmap( QPixmap( tools::ExerciseConfig::BuildResourceChildFile( pixmap.ascii() ).c_str() ) );
        }

        bool RequiresPassword() const { return password_; }
        QString Login() const { return login_; }

    private:
        UserItem( const UserItem& );
        UserItem& operator=( const UserItem& );

        const QString login_;
        const bool supervisor_;
        const bool password_;
    };
}

// -----------------------------------------------------------------------------
// Name: LoginDialog constructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
LoginDialog::LoginDialog( QWidget* pParent, const Profile& profile, Network& network, const tools::ExerciseConfig& config )
    : QDialog( pParent, 0, true, WStyle_Customize | WStyle_NormalBorder | WStyle_Title )
    , profile_( profile )
    , network_( network )
{
    setCaption( tools::translate( "LoginDialog", "Select user profile" ) );

    QGridLayout* grid = new QGridLayout( this, 3, 2, 0, 5 );
    grid->setMargin( 5 );
    grid->setRowStretch( 0, 10 );
    grid->setColStretch( 0, 3 );

    {
        users_ = new QIconView( this );
        users_->setGridX( 60 );
        users_->setGridY( 60 );
        users_->setFixedSize( 345, 200 );
        users_->setWordWrapIconText( false );
        users_->setSorting( true );
        users_->setItemsMovable( false );
        users_->setHScrollBarMode( QScrollView::AlwaysOff );
        connect( users_, SIGNAL( selectionChanged( QIconViewItem* ) ), SLOT( OnSelectItem( QIconViewItem* ) ) );
        connect( users_, SIGNAL( doubleClicked( QIconViewItem* ) ), SLOT( OnAccept() ) );
        grid->addMultiCellWidget( users_, 0, 0, 0, 2 );
        FillUsers( config );
    }
    {
        passwordBox_ = new QHBox( this );
        QLabel* label = new QLabel( tools::translate( "LoginDialog", "Password: " ), passwordBox_ );
        password_ = new QLineEdit( passwordBox_ );
        password_->setEchoMode( QLineEdit::Password );
        label->setBuddy( password_ );
        grid->addWidget( passwordBox_, 1, 0, Qt::AlignLeft );
        passwordBox_->hide();
    }
    {
        QButton* ok     = new QPushButton( tools::translate( "LoginDialog", "Ok" ), this );
        QButton* cancel = new QPushButton( tools::translate( "LoginDialog", "Cancel" ), this );
        grid->addWidget( ok, 1, 1 );
        grid->addWidget( cancel, 1, 2 );
        connect( ok, SIGNAL( clicked() ), SLOT( OnAccept() ) );
        connect( cancel, SIGNAL( clicked() ), SLOT( OnReject() ) );
    }
}

// -----------------------------------------------------------------------------
// Name: LoginDialog destructor
// Created: AGE 2006-10-11
// -----------------------------------------------------------------------------
LoginDialog::~LoginDialog()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::OnAccept
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
void LoginDialog::OnAccept()
{
    if( passwordBox_->isShown() && password_->text().isEmpty() )
        return;
    if( UserItem* item = static_cast< UserItem* >( users_->currentItem() ) )
    {
        profile_.Login( item->Login().ascii(), password_->text().ascii() );
        accept();
    }
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::OnReject
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
void LoginDialog::OnReject()
{
    network_.Disconnect();
    reject();
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::OnSelectItem
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
void LoginDialog::OnSelectItem( QIconViewItem* item )
{
    UserItem* user = static_cast< UserItem* >( item );
    passwordBox_->setShown( user->RequiresPassword() );
    password_->clear();
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::FillUsers
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
void LoginDialog::FillUsers( const tools::ExerciseConfig& config )
{
    xml::xifstream xis( config.GetProfilesFile() );
    xis >> xml::start( "profiles" )
            >> xml::list( "profile", *this, &LoginDialog::ReadUser );
}

// -----------------------------------------------------------------------------
// Name: LoginDialog::ReadUser
// Created: SBO 2009-06-10
// -----------------------------------------------------------------------------
void LoginDialog::ReadUser( xml::xistream& xis )
{
    new UserItem( users_, xis );
}
