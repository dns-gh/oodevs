// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#include "selftraining_app_pch.h"
#include "Registry.h"
#include <QSettings>

namespace
{
    const QString organization = "MASA Group";
    const QString application = "SWORD";
    const QString root = "sword/";

    QVariant GetValue( const QString& key, const QVariant& value )
    {
        QSettings settings( organization, application );
        return settings.value( root + key, value );
    }

    void SetValue( const QString& key, const QVariant& value )
    {
        QSettings settings( organization, application );
        settings.setValue( root + key, value );
    }
}

bool registry::ReadBool( const QString& key, bool defaultValue )
{
    return GetValue( key, defaultValue ).toBool();
}

void registry::WriteBool( const QString& key, bool value )
{
    SetValue( key, value );
}

QString registry::ReadString( const QString& key )
{
    return GetValue( key, QString() ).toString();
}

void registry::WriteString( const QString& key, const QString& value )
{
    SetValue( key, value );
}

int registry::ReadInt( const QString& key, int defaultValue )
{
    return GetValue( key, defaultValue ).toInt();
}

void registry::WriteInt( const QString& key, int value )
{
    SetValue( key, value );
}