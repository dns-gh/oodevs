// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "clients_kernel_pch.h"
#include "DataDictionary.h"
#include <qstringlist.h>

using namespace kernel;

// -----------------------------------------------------------------------------
// Name: DataDictionary constructor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
DataDictionary::DataDictionary()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: DataDictionary destructor
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
DataDictionary::~DataDictionary()
{
    for( IT_Data it = data_.begin(); it != data_.end(); ++it )
        delete it->second;
}

// -----------------------------------------------------------------------------
// Name: DataDictionary::Display
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
void DataDictionary::Display( const QString& name, Displayer_ABC& displayer ) const
{
    CIT_Data it = data_.find( name );
    if( it != data_.end() )
        it->second->Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: DataDictionary::Display
// Created: SBO 2006-10-11
// -----------------------------------------------------------------------------
void DataDictionary::Display( Displayer_ABC& displayer ) const
{
    for( CIT_Data it = data_.begin(); it != data_.end(); ++it )
        it->second->Display( displayer );
}

// -----------------------------------------------------------------------------
// Name: QStringList DataDictionary::FindKey
// Created: AGE 2006-06-22
// -----------------------------------------------------------------------------
QStringList DataDictionary::FindKey( QString name ) const
{
    QStringList result;
    for( CIT_Data it = data_.begin(); it != data_.end(); ++it )
    {
        if( it->first.find( name, 0, false ) != -1 )
            result.append( it->first );
    }
    return result;
}
