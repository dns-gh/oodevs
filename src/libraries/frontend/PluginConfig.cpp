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
#include "tools/Language.h"
#pragma warning( push, 0 )
#include <Qt/qapplication.h>
#include <QtCore/qsettings.h>
#include <QtCore/qstring.h>
#include <QtGui/qtooltip.h>
#include <QtGui/qlabel.h>
#pragma warning( pop )
#include <xeumeuleu/xml.hpp>
#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <algorithm>

using namespace frontend;

#ifndef PLATFORM
#error PLATFORM must be defined (for instance vc80 or vc100_x64) for masalife and directia plugins
#endif

#ifndef PLUGIN

#ifdef _DEBUG
    #define PLUGIN_EXTENSION_DLL "-mt-gd.dll"
#else
    #define PLUGIN_EXTENSION_DLL "-mt.dll"
#endif

#define PLUGIN( plugin ) ( std::string( plugin ) + std::string(  "-" BOOST_PP_STRINGIZE( PLATFORM ) PLUGIN_EXTENSION_DLL ) )
#endif

// -----------------------------------------------------------------------------
// Name: PluginConfig constructor
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
PluginConfig::PluginConfig( QWidget* parent, const tools::GeneralConfig& config, xml::xistream& xis )
    : PluginConfig_ABC( parent )
    , config_         ( config )
    , name_           ( xis.attribute< tools::Path >( "name" ) )
    , library_        ( tools::Path::FromUTF8( xis.has_attribute( "library" ) ? PLUGIN( xis.attribute< std::string >( "library", "" ) ) : "" ) )
    , version_        ( xis.attribute< std::string >( "version" ) )
    , description_    ( xis, tools::Language::Current() )
{
    box_ = new QGroupBox();
    QVBoxLayout* boxLayout = new QVBoxLayout( box_ );
    boxLayout->setMargin( 5 );
    boxLayout->setSizeConstraint( QLayout::SetMinAndMaxSize );
    box_->setCheckable( true );
    box_->setChecked( false );
    box_->setFlat( true );
    xis >> xml::start( "settings" )
            >> xml::list( "setting", *this, &PluginConfig::ReadSetting, box_ )
            >> xml::list( "group", *this, &PluginConfig::ReadGroup, box_ )
        >> xml::end;

    view_ = new QScrollArea();
    view_->setHorizontalScrollBarPolicy( Qt::ScrollBarAlwaysOff );
    view_->setSizePolicy( QSizePolicy::Preferred, QSizePolicy::Preferred );
    view_->setWidgetResizable( true );
    view_->setFrameStyle( QFrame::Box | QFrame::Sunken );
    view_->setWidget( box_ );

    QVBoxLayout* layout = new QVBoxLayout( this );
    layout->addWidget( view_ );
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
    box_->setTitle( tools::translate( "PluginConfig", "Enable %1 plugin v%2 " ).arg( label_ ).arg( version_.c_str() ) );

    const std::string currentLanguage = tools::Language::Current();
    description_.SetCurrentLanguage( currentLanguage );
    box_->setToolTip( description_.GetDescription().c_str() );

    for( IT_GroupBoxDescription it = groupBoxs_.begin(); it != groupBoxs_.end(); ++it )
    {
        kernel::XmlDescription* desc = it->second;
        assert( it->first && desc );
        desc->SetCurrentLanguage( currentLanguage );
        it->first->setTitle( desc->GetName().c_str() );
        it->first->setToolTip( desc->GetDescription().c_str() );
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
    return library_.IsEmpty() || ( config_.BuildPluginDirectory( name_ ) / library_ ).Exists();
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
void PluginConfig::Commit( const tools::Path& exercise, const tools::Path& session )
{
    if( box_->isChecked() )
    {
        frontend::CreateSession action( config_, exercise, session );
        const std::string root( "session/config/dispatcher/plugins/" + name_.ToUTF8() + "/" );
        if( !library_.IsEmpty() )
            action.SetOption( root + "@library", library_.ToUTF8() );
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
    settings_.push_back( boost::make_shared< PluginSetting >( parent, config_, xis ) );
}

// -----------------------------------------------------------------------------
// Name: PluginConfig::ReadGroup
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
void PluginConfig::ReadGroup( xml::xistream& xis, QWidget* parent )
{
    QGroupBox* box = new QGroupBox();
    QVBoxLayout* boxLayout = new QVBoxLayout( box );
    boxLayout->setSizeConstraint( QLayout::SetMinAndMaxSize );
    parent->layout()->addWidget( box );

    groupBoxs_[ box ] = new kernel::XmlDescription( xis, tools::Language::Current() );
    boxLayout->setMargin( 5 );

    xis >> xml::start( "settings" )
            >> xml::list( "setting", *this, &PluginConfig::ReadSetting, box )
            >> xml::list( "group", *this, &PluginConfig::ReadGroup, box )
        >> xml::end;
}
