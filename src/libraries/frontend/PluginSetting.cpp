// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "PluginSetting.h"
#include "moc_PluginSetting.cpp"
#include "PluginSettingVisitor_ABC.h"
#include "FileList.h"

#include "clients_gui/FileDialog.h"
#include "clients_kernel/Tools.h"
#include "tools/GeneralConfig.h"
#include "tools/Language.h"

#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>

#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

using namespace frontend;

FileButtonEvent::FileButtonEvent( PluginSetting& plugins )
    : QPushButton()
    , plugins_ ( plugins )
    , updated_( false )
{
    connect( this, SIGNAL( clicked() ), this, SLOT( OnFileClicked() ) );
}

void FileButtonEvent::SetText( const std::string& value )
{
    updated_ = true;
    QPushButton::setText( value.c_str() );
}

void FileButtonEvent::OnFileClicked()
{
    plugins_.OnFileClicked();
}

bool FileButtonEvent::HasBeenUpdated() const
{
    return updated_;
}

// -----------------------------------------------------------------------------
// Name: PluginSetting constructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginSetting::PluginSetting( QWidget* parent, const tools::GeneralConfig& config, xml::xistream& xis )
    : attribute_ ( xis.attribute< std::string >( "attribute" ) )
    , type_ ( xis.attribute< std::string >( "type" ) )
    , config_ ( config )
    , description_( xis, tools::Language::Current() )
    , label_()
    , stringValue_()
    , integerValue_()
    , booleanValue_()
    , timeValue_()
    , enumerationValue_()
    , fileList_()
{
    bool display = xis.attribute< bool >( "display", true );

    QHBoxLayout* propertyLayout = new QHBoxLayout( parent->layout() );
    label_ = new QLabel();
    propertyLayout->addWidget( label_ );
    label_->setVisible( display );

    if( type_ == "string" )
    {
        stringValue_ = new QLineEdit( xis.attribute< std::string >( "default", "" ).c_str() );
        propertyLayout->addWidget( stringValue_ );
        stringValue_->setVisible( display );
    }
    else if( type_ == "integer" )
    {
        integerValue_ = new QSpinBox();
        if( xis.has_attribute( "min" ) )
            integerValue_->setMinimum( xis.attribute< int >( "min" ) );
        if( xis.has_attribute( "max" ) )
            integerValue_->setMaximum( xis.attribute< int >( "max" ) );
        integerValue_->setValue( xis.attribute< int >( "default", 0 ) );
        propertyLayout->addWidget( integerValue_ );
        integerValue_->setVisible( display );
    }
    else if( type_ == "boolean" )
    {
        booleanValue_ = new QCheckBox();
        booleanValue_->setChecked( xis.attribute< bool >( "default", false ) );
        propertyLayout->addWidget( booleanValue_ );
        booleanValue_->setVisible( display );
    }
    else if( type_ == "time" )
    {
        timeValue_ = new QTimeEdit();
        timeValue_->setDisplayFormat( "hh:mm:ss" );
        timeValue_->setTime( QTime().addSecs( xis.attribute< int >( "default", 0 ) ) );
        propertyLayout->addWidget( timeValue_ );
        timeValue_->setVisible( display );

    }
    else if( type_ == "file" )
    {
        fileValue_.reset( new FileButtonEvent( *this ) );
        propertyLayout->addWidget( &*fileValue_ );
    }
    else if( type_ == "file_list" )
    {
        fileList_ = new FileList( tools::translate( "PluginSetting", "Files" ), parent, "?", "");
        fileList_->SetFilesDelimited(xis.attribute< std::string >( "default" ) );
        propertyLayout->addWidget( fileList_ );
    }
    else if( type_ == "enumeration" )
    {
        enumerationValue_ = new QComboBox();
        const std::string value = xis.attribute< std::string >( "default", "" );
        std::vector< std::string > enumerations;
        boost::split( enumerations, value, boost::is_any_of( ";" ) );
        BOOST_FOREACH( const std::string& enumeration , enumerations )
            enumerationValue_->addItem( enumeration.c_str() );
        enumerationValue_->setCurrentIndex( 0u );
        propertyLayout->addWidget( enumerationValue_ );
        enumerationValue_->setVisible( display );
    }
    propertyLayout->setStretch( 0, 1 );
    propertyLayout->setStretch( 1, 3 );
}

// -----------------------------------------------------------------------------
// Name: PluginSetting destructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginSetting::~PluginSetting()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginSetting::OnLanguageChanged
// Created: ABR 2011-11-10
// -----------------------------------------------------------------------------
void PluginSetting::OnLanguageChanged()
{
    description_.SetCurrentLanguage( tools::Language::Current() );
    label_->setText( description_.GetName().c_str() );
    label_->setToolTip( description_.GetDescription().c_str() );

    if( fileValue_.get() && !fileValue_->HasBeenUpdated() )
        fileValue_->setText( description_.GetDescription().empty() ? tools::translate( "PluginSetting", "Select a file..." ) : description_.GetDescription().c_str() );
    if( fileList_ != NULL )
        fileList_->OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: PluginSetting::Accept
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
void PluginSetting::Accept( PluginSettingVisitor_ABC& visitor )
{
    if( type_ == "string" )
        visitor.Visit( attribute_, std::string( stringValue_->text().toStdString() ) );
    else if( type_ == "integer" )
        visitor.Visit( attribute_, integerValue_->value() );
    else if( type_ == "boolean" )
        visitor.Visit( attribute_, booleanValue_->isChecked() );
    else if( type_ == "time" )
        visitor.Visit( attribute_, std::string( QString( "%1s" ).arg( QTime().secsTo( timeValue_->time() ) ).toStdString() ) );
    else if( type_ == "file" )
        visitor.Visit( attribute_, fileName_ );
    else if( type_ == "file_list" )
        visitor.Visit( attribute_,  fileList_->GetFilesDelimited());
    else if( type_ == "enumeration" )
        visitor.Visit( attribute_, std::string( enumerationValue_->currentText().toStdString() ) );
}

// -----------------------------------------------------------------------------
// Name: PluginSetting::OnFileClicked
// Created: JCR 2011-07-11
// -----------------------------------------------------------------------------
void PluginSetting::OnFileClicked()
{
    const std::string::size_type max_size = 20;

    tools::Path fileName = gui::FileDialog::getOpenFileName( 0, "Select recorded orders", config_.GetExercisesDir(), "Order File (*.ord)" );
    if( !fileName.IsEmpty() )
    {
        fileName_ = fileName.ToUTF8();
        std::string::size_type size = fileName_.length() - std::min( max_size, fileName_.length() );
        std::string msg( fileName_, size, size );
        if( fileName_.size() > max_size + 2 )
            fileValue_->SetText( ".." + msg );
        else
            fileValue_->SetText( msg );
    }
}
