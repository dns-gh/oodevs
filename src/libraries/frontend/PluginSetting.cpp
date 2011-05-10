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
#include <qapplication.h>
#include <qcheckbox.h>
#include <qdatetimeedit.h>
#include <qlabel.h>
#include <qlineedit.h>
#include <qsettings.h>
#include <qspinbox.h>
#include <qtextcodec.h>
#include <qtooltip.h>
#include <xeumeuleu/xml.hpp>

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

// -----------------------------------------------------------------------------
// Name: PluginSetting constructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginSetting::PluginSetting( QWidget* parent, xml::xistream& xis )
    : attribute_( xis.attribute< std::string >( "attribute" ) )
    , type_( xis.attribute< std::string >( "type" ) )
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
}
