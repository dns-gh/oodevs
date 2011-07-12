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
#include <qapplication.h>
#include <qcheckbox.h>
#include <qdatetimeedit.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qsettings.h>
#include <qspinbox.h>
#include <qtextcodec.h>
#include <qtooltip.h>
#include <qcombobox.h>
#include <qpushbutton.h>
#include <qfiledialog.h>
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

    template< typename T >
    T* Style( T* widget )
    {
        widget->setBackgroundOrigin( QWidget::WindowOrigin );
        return widget;
    }

    struct DescriptionReader
    {
        DescriptionReader( xml::xisubstream xis, const std::string& currentLanguage )
        {
            xis >> xml::start( "descriptions" )
                    >> xml::list( "description", *this, &DescriptionReader::ReadDescription, currentLanguage );
        }
        void ReadDescription( xml::xistream& xis, const std::string& currentLanguage )
        {
            const std::string lang = xis.attribute< std::string >( "lang", "en" );
            if( lang == currentLanguage || name_.empty() )
            {
                name_ = xis.attribute< std::string >( "name" );
                xis >> description_;
            }
        }
        std::string name_;
        std::string description_;
    };
}

FileButtonEvent::FileButtonEvent( PluginSetting& plugins, QWidget* parent, const std::string& description ) 
    : QObject( parent )
    , plugins_ ( plugins )
    , fileValue_( Style( new QPushButton( parent ) ) )
{
    fileValue_->setText( description.empty() ? "Select an order file..." : description.c_str() );
    connect( fileValue_, SIGNAL( clicked() ), this, SLOT( OnFileClicked() ) );
}

void FileButtonEvent::setText( const std::string& value )
{
    fileValue_->setText( value.c_str() );
}

void FileButtonEvent::OnFileClicked()
{
    plugins_.OnFileClicked();
}

// -----------------------------------------------------------------------------
// Name: PluginSetting constructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginSetting::PluginSetting( QWidget* parent, const tools::GeneralConfig& config, xml::xistream& xis )
    : attribute_ ( xis.attribute< std::string >( "attribute" ) )
    , type_ ( xis.attribute< std::string >( "type" ) )
    , config_ ( config )
{
    DescriptionReader reader( xis, ReadLang() );
    QToolTip::add( Style( new QLabel( reader.name_.c_str(), parent ) ), reader.description_.c_str() );
    if( type_ == "string" )
        stringValue_ = Style( new QLineEdit( xis.attribute< std::string >( "default", "" ).c_str(), parent ) );
    else if( type_ == "integer" )
    {
        integerValue_ = Style( new QSpinBox( parent ) );
        if( xis.has_attribute( "min" ) )
            integerValue_->setMinValue( xis.attribute< int >( "min" ) );
        if( xis.has_attribute( "max" ) )
            integerValue_->setMaxValue( xis.attribute< int >( "max" ) );
        integerValue_->setValue( xis.attribute< int >( "default", 0 ) );
    }
    else if( type_ == "boolean" )
    {
        booleanValue_ = Style( new QCheckBox( parent ) );
        booleanValue_->setChecked( xis.attribute< bool >( "default", false ) );
    }
    else if( type_ == "time" )
    {
        timeValue_ = Style( new QTimeEdit( parent ) );
        timeValue_->setDisplay ( QTimeEdit::Hours | QTimeEdit::Minutes | QTimeEdit::Seconds );
        timeValue_->setTime( QTime().addSecs( xis.attribute< int >( "default", 0 ) ) );
    }
    else if( type_ == "file" )
    {
        fileValue_.reset( new FileButtonEvent( *this, parent, reader.description_ ) );
    }
    else if( type_ == "enumeration" )
    {
        enumerationValue_ = Style( new QComboBox( parent ) );
        const std::string value = xis.attribute< std::string >( "default", "" );
        std::vector< std::string > enumerations;
        boost::split( enumerations, value, boost::is_any_of( ";" ) );
        BOOST_FOREACH( const std::string& enumeration , enumerations )
            enumerationValue_->insertItem( enumeration.c_str() );
        enumerationValue_->setCurrentItem( 0u );
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

    QString fileName = QFileDialog::getOpenFileName(
                                QString( config_.GetExercisesDir().c_str() ), "Order File (*.ord)",
                                0, "Select", "Select recorded orders" );
    if( !fileName.isNull() && !fileName.isEmpty() )
    {
        fileName_ = fileName;
        std::string::size_type size = fileName_.length() - std::min( max_size, (int)fileName_.length() );
        std::string msg( fileName_, size, size );
        if ( fileName_.size() > max_size + 2 )
            fileValue_->setText( ".." + msg );
        else
            fileValue_->setText( msg );
    }
}
