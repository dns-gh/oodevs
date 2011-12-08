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
#include "PluginSettingVisitor_ABC.h"
#include "moc_PluginSetting.cpp"
#include "tools/GeneralConfig.h"

#include "clients_kernel/Tools.h"

#pragma warning( push, 0 )
#include <Qt/qapplication.h>
#include <QtGui/qcheckbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtCore/qsettings.h>
#include <QtGui/qspinbox.h>
#include <QtCore/qtextcodec.h>
#include <QtGui/qtooltip.h>
#include <QtGui/qcombobox.h>
#include <QtGui/qpushbutton.h>
#include <Qt3Support/q3filedialog.h>
#include <Qt3Support/q3datetimeedit.h>
#pragma warning( pop )

#include <xeumeuleu/xml.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/foreach.hpp>

using namespace frontend;

namespace
{
    std::string ReadLang()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readEntry( "/Common/Language", QTextCodec::locale() ).ascii();
    }
}

FileButtonEvent::FileButtonEvent( PluginSetting& plugins, QWidget* parent ) 
    : QPushButton( parent )
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
    , description_( xis, ReadLang() )
{
    bool display = xis.attribute< bool >( "display", true ); 
    
    label_ = new QLabel( parent );
    label_->setHidden( !display );
    if( type_ == "string" )
    {
        stringValue_ = new QLineEdit( xis.attribute< std::string >( "default", "" ).c_str(), parent );
        stringValue_->setHidden( !display );
    }
    else if( type_ == "integer" )
    {
        integerValue_ = new QSpinBox( parent );
        if( xis.has_attribute( "min" ) )
            integerValue_->setMinValue( xis.attribute< int >( "min" ) );
        if( xis.has_attribute( "max" ) )
            integerValue_->setMaxValue( xis.attribute< int >( "max" ) );
        integerValue_->setValue( xis.attribute< int >( "default", 0 ) );
        integerValue_->setHidden( !display );
    }
    else if( type_ == "boolean" )
    {
        booleanValue_ = new QCheckBox( parent );
        booleanValue_->setChecked( xis.attribute< bool >( "default", false ) );
        booleanValue_->setHidden( !display );
    }
    else if( type_ == "time" )
    {
        timeValue_ = new Q3TimeEdit( parent );
        timeValue_->setDisplay ( Q3TimeEdit::Hours | Q3TimeEdit::Minutes | Q3TimeEdit::Seconds );
        timeValue_->setTime( QTime().addSecs( xis.attribute< int >( "default", 0 ) ) );
        timeValue_->setHidden( !display );
    }
    else if( type_ == "file" )
    {
        fileValue_.reset( new FileButtonEvent( *this, parent ) );
    }
    else if( type_ == "enumeration" )
    {
        enumerationValue_ = new QComboBox( parent );
        const std::string value = xis.attribute< std::string >( "default", "" );
        std::vector< std::string > enumerations;
        boost::split( enumerations, value, boost::is_any_of( ";" ) );
        BOOST_FOREACH( const std::string& enumeration , enumerations )
            enumerationValue_->insertItem( enumeration.c_str() );
        enumerationValue_->setCurrentItem( 0u );
        enumerationValue_->setHidden( !display );
    }
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
    description_.SetCurrentLanguage( ReadLang() );
    label_->setText( description_.GetName().c_str() );
    QToolTip::add( label_, description_.GetDescription().c_str() );

    if( fileValue_.get() && !fileValue_->HasBeenUpdated() )
        fileValue_->setText( description_.GetDescription().empty() ? tools::translate( "PluginSetting", "Select a file..." ) : description_.GetDescription().c_str() );
}

// -----------------------------------------------------------------------------
// Name: PluginSetting::Accept
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
void PluginSetting::Accept( PluginSettingVisitor_ABC& visitor )
{
    if( type_ == "string" )
        visitor.Visit( attribute_, std::string( stringValue_->text().ascii() ) );
    else if( type_ == "integer" )
        visitor.Visit( attribute_, integerValue_->value() );
    else if( type_ == "boolean" )
        visitor.Visit( attribute_, booleanValue_->isChecked() );
    else if( type_ == "time" )
        visitor.Visit( attribute_, std::string( QString( "%1s" ).arg( QTime().secsTo( timeValue_->time() ) ).ascii() ) );
    else if( type_ == "file" )
    {
        // TODO:
        // const bfs::path exerciseDir = bfs::path( config_.GetExerciseDir( exercise ), bfs::native );
        // const bfs::path orderDir = bfs::path( orderFile_, bfs::native );
        // if( exerciseDir.string() == std::string( orderDir.string(), 0, exerciseDir.string().size() ) )
        //    action.SetOption( "session/config/dispatcher/plugins/timeline/orders/@file", std::string( orderFile_, exerciseDir.string().size() + 1, orderFile_.size() - exerciseDir.string().size() - 1 ) );
        visitor.Visit( attribute_, fileName_ );
    }
    else if( type_ == "enumeration" )
        visitor.Visit( attribute_, std::string( enumerationValue_->currentText().ascii() ) );
}

// -----------------------------------------------------------------------------
// Name: PluginSetting::OnFileClicked
// Created: JCR 2011-07-11
// -----------------------------------------------------------------------------
void PluginSetting::OnFileClicked()
{
    const int max_size = 20;

    QString fileName = Q3FileDialog::getOpenFileName(
                                QString( config_.GetExercisesDir().c_str() ), "Order File (*.ord)",
                                0, "Select", "Select recorded orders" );
    if( !fileName.isNull() && !fileName.isEmpty() )
    {
        fileName_ = fileName;
        std::string::size_type size = fileName_.length() - std::min( max_size, (int)fileName_.length() );
        std::string msg( fileName_, size, size );
        if ( fileName_.size() > max_size + 2 )
            fileValue_->SetText( ".." + msg );
        else
            fileValue_->SetText( msg );
    }
}
