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
#pragma warning( push, 0 )
#include <Qt/qapplication.h>
#include <Qt3Support/q3groupbox.h>
#include <Qt3Support/q3scrollview.h>
#include <QtCore/qsettings.h>
#include <QtCore/qtextcodec.h>
#include <QtCore/qstring.h>
#include <QtGui/qtooltip.h>
#include <QtGui/qlabel.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <boost/filesystem/convenience.hpp>
#include <algorithm>

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
    , description_    ( xis, ReadLang() )
{
    setMargin( 5 );
    view_ = new Q3ScrollView( this );
    view_->setHScrollBarMode( Q3ScrollView::AlwaysOff );
    view_->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    view_->setResizePolicy( Q3ScrollView::AutoOneFit );
    view_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    view_->enableClipper( true );

	box_ = new Q3GroupBox( 2, Qt::Horizontal, view_->viewport() );
    box_->setCheckable( true );
    box_->setChecked( false );
	box_->setFlat( true );
	xis >> xml::start( "settings" )
            >> xml::list( "setting", *this, &PluginConfig::ReadSetting, box_ )
            >> xml::list( "group", *this, &PluginConfig::ReadGroup, box_ );
	// Add empty label to fix viewport bug of not displaying the overall content
	QLabel* label = new QLabel( QString( ""), box_ );
	label->setMinimumHeight( 15 );
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
// Name: PluginConfig::OnLanguageChanged
// Created: ABR 2011-11-10
// -----------------------------------------------------------------------------
void PluginConfig::OnLanguageChanged()
{
    box_->setTitle( tools::translate( "PluginConfig", "Enable %1 plugin (v%2)" ).arg( label_ ).arg( version_.c_str() ) );

    const std::string currentLanguage = ReadLang();
    description_.SetCurrentLanguage( currentLanguage );
    QToolTip::add( box_, description_.GetDescription().c_str() );

    for( IT_GroupBoxDescription it = groupBoxs_.begin(); it != groupBoxs_.end(); ++it )
    {
        kernel::XmlDescription* desc = it->second;
        assert( it->first && desc );
        desc->SetCurrentLanguage( currentLanguage );
        it->first->setTitle( desc->GetName().c_str() );
        QToolTip::add( it->first, desc->GetDescription().c_str() );
    }

    for( IT_Settings it = settings_.begin(); it != settings_.end(); ++it )
        ( *it )->OnLanguageChanged();
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString PluginConfig::GetName() const
{
    return description_.GetName().c_str();
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
    Q3GroupBox* box = new Q3GroupBox( 2, Qt::Horizontal, parent );
    groupBoxs_[ box ] = new kernel::XmlDescription( xis, ReadLang() );
    box->setMargin( 5 );
    static_cast< Q3GroupBox* >( parent )->setColumns( 1 );
    xis >> xml::start( "settings" )
            >> xml::list( "setting", *this, &PluginConfig::ReadSetting, box )
            >> xml::list( "group", *this, &PluginConfig::ReadGroup, box );
}
