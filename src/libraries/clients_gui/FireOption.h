// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 MASA Group
//
// *****************************************************************************

#ifndef FireOption_h
#define FireOption_h

#include <clients_kernel/Karma.h>
#include <QtGui/QColor>
#include <deque>

namespace xml
{
    class xistream;
    class xostream;
}

namespace kernel
{
    class Entity_ABC;
    class DotationType;
    class Options;
    class OptionVariant;
}

namespace gui
{
    class GLView_ABC;

    enum FireGroup
    {
        FIRE_GROUP_DIRECT,
        FIRE_GROUP_INDIRECT,
        FIRE_GROUP_COUNT,
    };

    enum FireIndicators
    {
        FIRE_INDICATORS_DEFAULT,
        FIRE_INDICATORS_SIDE,
        FIRE_INDICATORS_UNIT,
        FIRE_INDICATORS_RULE,
        FIRE_INDICATORS_COUNT,
    };

    const std::string& GetFireIndicatorsOptionName();

    struct FireOption
    {
        FireOption();
        bool operator==( const FireOption& other ) const;

        QString name;
        kernel::Karma karma;
        QColor color;

        static const QString& GetAny();
        static std::string GetOptionName( FireGroup );
    };

    typedef std::deque< FireOption > T_FireOptions;

    T_FireOptions LoadFireOptions( const QString& data );
    QString SaveFireOptions( const T_FireOptions& options );

    void LoadFromOptions( std::vector< T_FireOptions >&, const kernel::Options& options );

    struct FireColor;
    std::shared_ptr< FireColor > GetDirectFireColor( const GLView_ABC& view,
                                                     const kernel::Entity_ABC& entity );
    std::shared_ptr< FireColor > GetIndirectFireColor( const GLView_ABC& view,
                                                       const kernel::Entity_ABC& entity,
                                                       const kernel::DotationType* ammo );
}

#endif // FireOption_h
