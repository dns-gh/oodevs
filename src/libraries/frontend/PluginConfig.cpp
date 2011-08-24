// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "PluginConfig.h"
#include "CreateSession.h"
#include "PluginSetting.h"
#include "PluginSettingVisitor_ABC.h"
#include "clients_kernel/Tools.h"
#include "tools/GeneralConfig.h"
#include <algorithm>
#include <boost/bind.hpp>
#include <boost/filesystem/convenience.hpp>
#include <Qt/qapplication.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3scrollview.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtextcodec.h>
#include <QtGui/qtooltip.h>
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
// Name: PluginConfig constructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginConfig::PluginConfig( QWidget* parent, const tools::GeneralConfig& config, xml::xistream& xis )
    : PluginConfig_ABC( parent )
    , config_         ( config )
    , name_           ( xis.attribute< std::string >( "name" ) )
    , library_        ( xis.attribute< std::string >( "library", "" ) )
    , version_        ( xis.attribute< std::string >( "version" ) )
{
    DescriptionReader reader( xis, ReadLang() );
    setMargin( 5 );
    setBackgroundOrigin( QWidget::WindowOrigin );
    view_ = Style( new Q3ScrollView( this ) );
    view_->setHScrollBarMode( Q3ScrollView::AlwaysOff );
    view_->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    view_->setResizePolicy( Q3ScrollView::AutoOneFit );
    view_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    box_ = Style( new Q3GroupBox( 2, Qt::Horizontal, tools::translate( "PluginConfig", "Enable %1 plugin (v%2)" )
                                                                    .arg( reader.name_.c_str() )
                                                                    .arg( version_.c_str() ), view_->viewport() ) );
    label_ = reader.name_.c_str();
    QToolTip::add( box_, reader.description_.c_str() );
    box_->setCheckable( true );
    box_->setChecked( false );
    xis >> xml::start( "settings" )
            >> xml::list( "setting", *this, &PluginConfig::ReadSetting, box_ )
                >> xml::list( "group", *this, &PluginConfig::ReadGroup, box_ );
    view_->addChild( box_ );
}

// -----------------------------------------------------------------------------
// Name: PluginConfig destructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginConfig::~PluginConfig()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString PluginConfig::GetName() const
{
    return label_;
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::IsAvailable
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
bool PluginConfig::IsAvailable() const
{
    using namespace boost::filesystem;
    return library_.empty() || exists( path( config_.BuildPluginDirectory( name_ ) ) / library_ );
}

namespace
{
    class SettingVisitor : public PluginSettingVisitor_ABC
    {
    public:
        SettingVisitor( frontend::CreateSession& action, const std::string& root )
            : action_( action ), root_( root ) {}
        virtual void Visit( const std::string& attribute, const std::string& value )
        {
            SetOption( attribute, value );
        }
        virtual void Visit( const std::string& attribute, int value )
        {
            SetOption( attribute, value );
        }
        virtual void Visit( const std::string& attribute, bool value )
        {
            SetOption( attribute, value );
        }
        template< typename T >
        void SetOption( const std::string& attribute, T value )
        {
            action_.SetOption( root_ + attribute, value );
        }
        frontend::CreateSession& action_;
        const std::string root_;
    };
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::Commit
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
void PluginConfig::Commit( const std::string& exercise, const std::string& session )
{
    if( box_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        const std::string root( "session/config/dispatcher/plugins/" + name_ + "/" );
        if( !library_.empty() )
            action.SetOption( root + "@library", library_ );
        SettingVisitor visitor( action, root );
        std::for_each( settings_.begin(), settings_.end(), boost::bind( &PluginSetting::Accept, _1, boost::ref( visitor ) ) );
        action.Commit();
    }
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::ReadSetting
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
void PluginConfig::ReadSetting( xml::xistream& xis, QWidget* parent )
{
    settings_.push_back( boost::shared_ptr< PluginSetting >( new PluginSetting( parent, config_, xis ) ) );
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::ReadGroup
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
void PluginConfig::ReadGroup( xml::xistream& xis, QWidget* parent )
{
    DescriptionReader reader( xis, ReadLang() );
    Q3GroupBox* box = Style( new Q3GroupBox( 2, Qt::Horizontal, reader.name_.c_str(), parent ) );
    box->setMargin( 5 );
    static_cast< Q3GroupBox* >( parent )->setColumns( 1 );
    QToolTip::add( box_, reader.description_.c_str() );
    xis >> xml::start( "settings" )
            >> xml::list( "setting", *this, &PluginConfig::ReadSetting, box )
            >> xml::list( "group", *this, &PluginConfig::ReadGroup, box );
}
