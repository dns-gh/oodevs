// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#include "clients_gui_pch.h"
#include "Settings.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/FourStateOption.h"

#include <qmainwindow.h>
#include <qdockwindow.h>

using namespace kernel;
using namespace gui;

// -----------------------------------------------------------------------------
// Name: Settings constructor
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
Settings::Settings()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: Settings constructor
/** @param  nFormat
*/
// Created: APE 2004-06-01
// -----------------------------------------------------------------------------
Settings::Settings( QSettings::Format nFormat )
    : QSettings( nFormat )
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
void Settings::WriteEntry( const QString& field, QWidget& widget )
{
    beginGroup( field );
    writeEntry( "/width",  widget.width() );
    writeEntry( "/height", widget.height() );
    writeEntry( "/x", widget.x() );
    writeEntry( "/y", widget.y() );
    writeEntry( "/visible", widget.isVisible() );
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
    widget.move( readNumEntry( "/x", nX ), readNumEntry( "/y", nY ) );
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
        unsigned int i = path.find_last_of( '/' );
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
    return TristateOption( value.ascii() );
}

// -----------------------------------------------------------------------------
// Name: Settings::Load
// Created: AGE 2007-05-31
// -----------------------------------------------------------------------------
FourStateOption Settings::Load( const std::string& name, const kernel::FourStateOption& defaultValue )
{
    QString value = readEntry( ( "/" + AddType( name, fourstatePrefix ) ).c_str(), QString( defaultValue ) );
    return FourStateOption( value.ascii() );
}

// -----------------------------------------------------------------------------
// Name: Settings::Load
// Created: SBO 2007-03-27
// -----------------------------------------------------------------------------
QString Settings::Load( const std::string& name, const QString& defaultValue )
{
    return readEntry( ( "/" + AddType( name, stringPrefix ) ).c_str(), defaultValue );
}
