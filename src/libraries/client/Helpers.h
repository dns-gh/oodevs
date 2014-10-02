// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef HELPERS_H__
#define HELPERS_H__

#define COUNT_OF(X) (sizeof(X)/sizeof*(X))

namespace gui
{
    inline QString QUtf8( const std::string& text )
    {
        return QString::fromUtf8( text.c_str(), static_cast< int >( text.size() ) );
    }

    inline std::string QUtf8( const QString& text )
    {
        const QByteArray data = text.toUtf8();
        return std::string( data.constData(), data.size() );
    }
}

#endif // HELPERS_H__