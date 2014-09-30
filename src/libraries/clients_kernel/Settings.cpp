// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "Settings.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/FourStateOption.h"
#include <qtgui/qdesktopwidget.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: Settings constructor
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
Settings::Settings( const QString & organization, const QString & application )
    : QSettings( organization, application )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Settings destructor
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
Settings::~Settings()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Settings::BeginGroup
// Created: JSR 2014-09-24
// -----------------------------------------------------------------------------
void Settings::BeginGroup( const QString& prefix )
{
    beginGroup( prefix );
}

// -----------------------------------------------------------------------------
// Name: Settings::EndGroup
// Created: JSR 2014-09-24
// -----------------------------------------------------------------------------
void Settings::EndGroup()
{
    endGroup();
}

// -----------------------------------------------------------------------------
// Name: Settings::RemoveKey
// Created: JSR 2014-09-24
// -----------------------------------------------------------------------------
void Settings::RemoveKey( const QString& key )
{
    remove( key );
}

// -----------------------------------------------------------------------------
// Name: Settings::EntryList
// Created: AGE 2006-08-08
// -----------------------------------------------------------------------------
QStringList Settings::EntryList( const char* path )
{
    return entryList( path );
}

// -----------------------------------------------------------------------------
// Name: Settings::SubEntriesList
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
QStringList Settings::SubEntriesList( const char* path )
{
    return subkeyList( path );
}

// -----------------------------------------------------------------------------
// Name: Settings::WriteEntry
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void Settings::WriteMainWindowEntries( const QString& field, QWidget& widget )
{
    beginGroup( field );
    writeEntry( "/width", widget.width() );
    writeEntry( "/height", widget.height() );
    writeEntry( "/x", widget.x() );
    writeEntry( "/y", widget.y() );
    writeEntry( "/visible", widget.isVisible() );
    endGroup();
}

// -----------------------------------------------------------------------------
// Name: Settings::RemoveEntry
// Created: ABR 2011-05-27
// -----------------------------------------------------------------------------
void Settings::RemoveMainWindowEntries( const QString& field )
{
    beginGroup( field );
    removeEntry( "/width" );
    removeEntry( "/height" );
    removeEntry( "/x" );
    removeEntry( "/y" );
    removeEntry( "/visible" );
    endGroup();
}

// -----------------------------------------------------------------------------
// Name: Settings::ReadEntry
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
void Settings::ReadEntry( const QString& field, QWidget& widget, int nW, int nH, int nX, int nY, bool bVisible )
{
    beginGroup( field );
    widget.resize( readNumEntry( "/width", nW ), readNumEntry( "/height", nH ) );
    QPoint pos( readNumEntry( "/x", nX ), readNumEntry( "/y", nY ) );
    if( QApplication::desktop()->screenNumber( pos ) != -1 )
        widget.move( pos );
    if( readBoolEntry( "/visible", bVisible ) )
        widget.show();
    else
        widget.hide();
    endGroup();
}

namespace
{
    std::string AddType( const std::string& path, const char type )
    {
        std::size_t i = path.find_last_of( '/' );
        if( i == path.npos )
            return type + path;
        return path.substr( 0, i + 1 ) + type + path.substr( i + 1 );
    }
}

// -----------------------------------------------------------------------------
// Name: Settings::Save
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void Settings::Save( const std::string& name, int value )
{
    writeEntry( ( "/" + AddType( name, intPrefix ) ).c_str(), value );
}

// -----------------------------------------------------------------------------
// Name: Settings::Save
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void Settings::Save( const std::string& name, bool value )
{
    writeEntry( ( "/" + AddType( name, boolPrefix ) ).c_str(), value );
}

// -----------------------------------------------------------------------------
// Name: Settings::Save
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void Settings::Save( const std::string& name, float value )
{
    writeEntry( ( "/" + AddType( name, floatPrefix ) ).c_str(), value );
}

// -----------------------------------------------------------------------------
// Name: Settings::Save
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
void Settings::Save( const std::string& name, const TristateOption& value )
{
    writeEntry( ( "/" + AddType( name, tristatePrefix ) ).c_str(), (QString)value );
}

// -----------------------------------------------------------------------------
// Name: Settings::Save
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
void Settings::Save( const std::string& name, const kernel::FourStateOption& value )
{
    writeEntry( ( "/" + AddType( name, fourstatePrefix ) ).c_str(), (QString)value );
}

// -----------------------------------------------------------------------------
// Name: Settings::Save
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
void Settings::Save( const std::string& name, const QString& value )
{
    writeEntry( ( "/" + AddType( name, stringPrefix ) ).c_str(), value );
}

// -----------------------------------------------------------------------------
// Name: Settings::Remove
// Created: LGY 2010-09-23
// -----------------------------------------------------------------------------
bool Settings::Remove( const std::string& name )
{
    return removeEntry( ( "/" + AddType( name, stringPrefix ) ).c_str() );
}

// -----------------------------------------------------------------------------
// Name: Settings::Load
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
int Settings::Load( const std::string& name, int defaultValue )
{
    return readNumEntry( ( "/" + AddType( name, intPrefix ) ).c_str(), defaultValue );
}

// -----------------------------------------------------------------------------
// Name: Settings::Load
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
bool Settings::Load( const std::string& name, bool defaultValue )
{
    return readBoolEntry( ( "/" + AddType( name, boolPrefix ) ).c_str(), defaultValue );
}

// -----------------------------------------------------------------------------
// Name: Settings::Load
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
float Settings::Load( const std::string& name, float defaultValue )
{
    return (float)readDoubleEntry( ( "/" + AddType( name, floatPrefix ) ).c_str(), defaultValue );
}

// -----------------------------------------------------------------------------
// Name: Settings::Load
// Created: AGE 2006-04-19
// -----------------------------------------------------------------------------
TristateOption Settings::Load( const std::string& name, const TristateOption& defaultValue )
{
    QString value = readEntry( ( "/" + AddType( name, tristatePrefix ) ).c_str(), QString( defaultValue ) );
    return TristateOption( value );
}

// -----------------------------------------------------------------------------
// Name: Settings::Load
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FourStateOption Settings::Load( const std::string& name, const kernel::FourStateOption& defaultValue )
{
    QString value = readEntry( ( "/" + AddType( name, fourstatePrefix ) ).c_str(), QString( defaultValue ) );
    return FourStateOption( value );
}

// -----------------------------------------------------------------------------
// Name: Settings::Load
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
QString Settings::Load( const std::string& name, const QString& defaultValue )
{
    return readEntry( ( "/" + AddType( name, stringPrefix ) ).c_str(), defaultValue );
}
