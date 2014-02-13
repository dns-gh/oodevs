// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef SELFTRAINING_REGISTRY_H__
#define SELFTRAINING_REGISTRY_H__

namespace registry
{
    bool    ReadBool( const QString& key, bool defaultValue = false );
    void    WriteBool( const QString& key, bool value );
    QString ReadString( const QString& key );
    void    WriteString( const QString& key, const QString& value );
    int     ReadInt( const QString& key, int defaultValue = 0 );
    void    WriteInt( const QString& key, int value );
    QString ReadFeatures();
}

#endif // SELFTRAINING_REGISTRY_H__
