// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "DebugConfigPanel.h"
#include "moc_DebugConfigPanel.cpp"
#include "clients_gui/tools.h"

namespace
{
    const int maxIntegrationDir = 5;

    bool ReadBoolRegistry( const std::string& key )
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( ( "/sword/" + key ).c_str() ).toBool();
    }
    void WriteBoolRegistry( const std::string& key, bool value )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.setValue( ( "/sword/" + key ).c_str(), value );
    }

    QString ReadStringRegistryValue( const std::string& key )
    {
        QSettings settings( "MASA Group", "SWORD" );
        return settings.value( ( "/sword/" + key ).c_str(), "" ).toString();
    }

    void WriteStringRegistryValue( const std::string& key, QString stringValue )
    {
        QSettings settings( "MASA Group", "SWORD" );
        settings.setValue( ( "/sword/" + key ).c_str(), stringValue );
    }
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel constructor
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
DebugConfigPanel::DebugConfigPanel()
    : pathList_( new QStringList() )
{
    //legacy box
    legacyLabel_ = new QLabel();
    legacyLabel_->setText( tools::translate( "DebugConfigPanel", "Enable Legacy Mode" ) );

    legacyCheckBox_ = new QCheckBox();
    legacyCheckBox_->setCheckable( true );
    legacyCheckBox_->setChecked( ReadBoolRegistry( "IsLegacy" ) );
    connect( legacyCheckBox_, SIGNAL( clicked ( bool ) ), SLOT( SwordVersionChecked( bool ) ) );

    legacyBox_ = new QGroupBox();
    QHBoxLayout* commentBoxLayout = new QHBoxLayout( legacyBox_ );
    commentBoxLayout->addWidget( legacyLabel_ );
    commentBoxLayout->addWidget( legacyCheckBox_ );
    commentBoxLayout->setMargin( 5 );

    //integration level label
    integrationLabel_ = new QLabel();
    integrationLabel_->setText( tools::translate( "DebugConfigPanel", "Integration layer directory" ) );

    //integration level comobobox
    integrationComboBox_ = new QComboBox();
    integrationComboBox_->setEditable( true );
    QString pathValue = ReadStringRegistryValue( "IntegrationLayerPaths" );
    if( !pathValue.isEmpty() )
    {
        pathList_ = new QStringList( QStringList::split( ";", pathValue ) );
        integrationComboBox_->addItems( *pathList_ );
    }
    connect( integrationComboBox_, SIGNAL( editTextChanged( const QString& ) ), SLOT( OnEditIntegrationDirectory( const QString& ) ) );

    //integration level button
    integrationButton_ = new QPushButton();
    integrationButton_->setText( "..." );
    connect( integrationButton_, SIGNAL( clicked() ), SLOT( OnChangeIntegrationDirectory() ) );

    //integraiton level group box
    integrationBox_ = new QGroupBox();
    QHBoxLayout* integrationBoxLayout = new QHBoxLayout( integrationBox_ );
    integrationBoxLayout->addWidget( integrationLabel_ );
    integrationBoxLayout->addWidget( integrationComboBox_ );
    integrationBoxLayout->addWidget( integrationButton_ );
    integrationBoxLayout->setStretch( 0, 2 );
    integrationBoxLayout->setStretch( 1, 10 );
    integrationBoxLayout->setStretch( 2, 1 );
    integrationBoxLayout->setMargin( 5 );


    //general Layout
    QVBoxLayout* mainLayout = new QVBoxLayout( this );
    mainLayout->addWidget( legacyBox_ );
    mainLayout->addWidget( integrationBox_ );
    mainLayout->setAlignment( Qt::AlignTop );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel destructor
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
DebugConfigPanel::~DebugConfigPanel()
{
        // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::SwordVersionChecked
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
void DebugConfigPanel::SwordVersionChecked( bool state )
{
    WriteBoolRegistry( "IsLegacy", state );
    emit SwordVersionSelected( state );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnChangeIntegrationDirectory
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnChangeIntegrationDirectory()
{
    const QString directory = QDir::convertSeparators( QFileDialog::getExistingDirectory( this , "" ) );
    integrationComboBox_->setCurrentText( directory );
}

// -----------------------------------------------------------------------------
// Name: DebugConfigPanel::OnEditIntegrationDirectory
// Created: NPT 2013-01-03
// -----------------------------------------------------------------------------
void DebugConfigPanel::OnEditIntegrationDirectory( const QString& directory )
{
    if( boost::filesystem::is_directory( directory.toStdString() ) )
    {
        if ( !pathList_->contains( directory, Qt::CaseSensitive ) )
        {
            //maj of combobox
            pathList_->push_front( directory );
            if( pathList_->count() > maxIntegrationDir )
                pathList_->removeLast();
            integrationComboBox_->clear();
            integrationComboBox_->addItems( *pathList_ );

            //save in registry
            WriteStringRegistryValue( "IntegrationLayerPaths", pathList_->join( ";" ) );
        }
    }
    emit IntegrationPathSelected( directory );
}

