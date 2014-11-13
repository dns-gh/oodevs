// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "FireOption.h"
#include "clients_kernel/Agent_ABC.h"
#include "clients_kernel/AgentType.h"
#include "clients_kernel/TacticalHierarchies.h"
#include "clients_kernel/Diplomacies_ABC.h"
#include "clients_kernel/DotationType.h"
#include "clients_kernel/OptionVariant.h"
#include "clients_kernel/Options.h"
#include "clients_kernel/Tools.h"
#include "clients_gui/ColorStrategy_ABC.h"
#include "clients_gui/GLView_ABC.h"
#include "clients_gui/GLOptions.h"
#include <xeumeuleu/xml.hpp>
#include <boost/optional.hpp>
#include <boost/static_assert.hpp>

using namespace gui;

FireOption::FireOption()
    : name( FireOption::GetAny() )
    , color( Qt::red )
{
    // NOTHING
}

bool FireOption::operator==( const FireOption& other ) const
{
    return name == other.name &&
        karma == other.karma &&
        color == other.color;
}

namespace
{
    std::string RemoveProlog( const std::string& s )
    {
        const auto pos = s.find( "?>" );
        if( pos == std::string::npos )
            return s;
        return s.substr( pos + 2 );
    }
}

QString gui::SaveFireOptions( const T_FireOptions& options )
{
    xml::xostringstream xos;
    xos << xml::start( "data" );
    for( auto it = options.begin(); it != options.end(); ++it )
        xos << xml::start( "item" )
                << xml::attribute( "name", it->name.toStdString() )
                << xml::attribute( "karma", it->karma.GetName().toStdString() )
                << xml::attribute( "color", it->color.name() )
            << xml::end;
    return QString::fromStdString( RemoveProlog( xos.str() ) );
}

T_FireOptions gui::LoadFireOptions( const QString& data )
{
    T_FireOptions options;
    if( data.isEmpty() )
        return options;
    xml::xistringstream xis( data.toStdString() );
    options.clear();
    xis >> xml::start( "data" )
            >> xml::list( "item",
                [&]( xml::xistream& xis )
                {
                    std::string name, karma, color;
                    xis >> xml::attribute( "name", name )
                        >> xml::attribute( "karma", karma )
                        >> xml::attribute( "color", color );
                    FireOption fire;
                    fire.name = QString::fromStdString( name );
                    fire.karma = kernel::Karma::ResolveName( QString::fromStdString( karma ) );
                    fire.color = QColor( QString::fromStdString( color ) );
                    options.push_back( fire );
                } );
    return options;
}

void gui::LoadFromOptions( std::vector< T_FireOptions >& fireOptions, const kernel::Options& options )
{
    options.Apply( [&fireOptions]( const std::string& name, const kernel::OptionVariant& value, bool ){
        if( name.find_first_of( "FireRules/" ) == 0 )
            for( int i = 0; i < FIRE_GROUP_COUNT; ++i )
                if( name == FireOption::GetOptionName( static_cast< FireGroup >( i ) ) )
                    fireOptions[i] = LoadFireOptions( value.To< QString >() );
    } );
}

const QString& FireOption::GetAny()
{
    static const QString any = tools::translate( "FireOption", "Any" );
    return any;
}

std::string FireOption::GetOptionName( FireGroup group )
{
    static const std::string names[] = {
        "direct",
        "indirect",
    };
#define COUNT_OF(X) (sizeof(X)/sizeof*(X))
    BOOST_STATIC_ASSERT( COUNT_OF( names ) == FIRE_GROUP_COUNT );
    return "FireRules/" + names[group];
}

const std::string& gui::GetFireIndicatorsOptionName()
{
    static const std::string name = "FireRules/indicators";
    return name;
}

struct gui::FireColor
{
    explicit FireColor( QColor color, const GLView_ABC& view )
    {
        qreal r, g, b;
        color.getRgbF( &r, &g, &b );
        glPushAttrib( GL_LINE_BIT | GL_CURRENT_BIT );
        glColor4f( static_cast< float >( r ),
                   static_cast< float >( g ),
                   static_cast< float >( b ),
                   view.GetCurrentAlpha() );
    }

    ~FireColor()
    {
        glPopAttrib();
    }
};

namespace
{
    kernel::Karma GetKarma( const kernel::Entity_ABC& entity )
    {
        if( auto hierarchy = entity.Retrieve< kernel::TacticalHierarchies >() )
            if( auto diplomacy = hierarchy->GetTop().Retrieve< kernel::Diplomacies_ABC >() )
                return diplomacy->GetKarma();
        return kernel::Karma::unknown_;
    }

    QColor SelectColor( const T_FireOptions& options, const QString& name, kernel::Karma karma )
    {
        std::vector< QColor > data( 3 );
        for( auto it = options.begin(); it != options.end(); ++it )
            if( it->name == name )
            {
                if( it->karma == karma )
                    return it->color;
                else if( it->karma == kernel::Karma::unknown_ )
                    data[0] = it->color;
            }
            else if( it->name == FireOption::GetAny() )
            {
                if( it->karma == karma )
                    data[1] = it->color;
                else if( it->karma == kernel::Karma::unknown_ )
                    data[2] = it->color;
            }
        for( auto it = data.begin(); it != data.end(); ++it )
            if( it->isValid() )
                return *it;
        return Qt::red;
    }

    boost::optional< QColor > GetFireColor( const GLOptions& options,
                                            ColorStrategy_ABC& colorStrategy,
                                            const kernel::Entity_ABC& entity )
    {
        const auto option = options.Has( GetFireIndicatorsOptionName() )
            ? static_cast< FireIndicators >( options.Get( GetFireIndicatorsOptionName() ).To< int >() )
            : FIRE_INDICATORS_DEFAULT;
        switch( option )
        {
            case FIRE_INDICATORS_SIDE: return colorStrategy.FindBaseColor( entity );
            case FIRE_INDICATORS_UNIT: return colorStrategy.FindColorWithModifiers( entity );
            case FIRE_INDICATORS_RULE: return boost::none;
        }
        return Qt::red;
    }

    std::shared_ptr< FireColor > GetColor( const GLView_ABC& view,
                                           FireGroup group,
                                           const kernel::Entity_ABC& entity,
                                           const QString& name )
    {
        const auto& gl = view.GetCurrentOptions();
        if( const auto color = GetFireColor( gl, view.GetColorStrategy(), entity ) )
            return std::make_shared< FireColor >( *color, view );
        const auto color = SelectColor( gl.GetFireOptions( group ), name, GetKarma( entity ) );
        return std::make_shared< FireColor >( color, view );
    }

    QString GetAgentType( const kernel::Entity_ABC& entity )
    {
        if( const auto* agent = dynamic_cast< const kernel::Agent_ABC* >( &entity ) )
            return QString::fromStdString( agent->GetType().GetLocalizedName() );
        return QString();
    }
}

std::shared_ptr< FireColor > gui::GetDirectFireColor( const GLView_ABC& view,
                                                      const kernel::Entity_ABC& entity )
{
    return GetColor( view, FIRE_GROUP_DIRECT, entity, GetAgentType( entity ) );
}

std::shared_ptr< FireColor > gui::GetIndirectFireColor( const GLView_ABC& view,
                                                        const kernel::Entity_ABC& entity,
                                                        const kernel::DotationType* dotation )
{
    const QString name = dotation ? QString::fromStdString( dotation->GetName() ) : QString();
    return GetColor( view, FIRE_GROUP_INDIRECT, entity, name );
}
