// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Settings_ABC_h_
#define __Settings_ABC_h_

#include <QtCore/qstringlist.h>

namespace kernel
{
    class TristateOption;
    class FourStateOption;

// =============================================================================
/** @class  Settings_ABC
    @brief  Settings definition
*/
// Created: AGE 2006-08-08
// =============================================================================
class Settings_ABC
{
public:
    //! @name Constants
    //@{
    static const char intPrefix       = 'I';
    static const char boolPrefix      = 'B';
    static const char floatPrefix     = 'F';
    static const char tristatePrefix  = 'T';
    static const char fourstatePrefix = '4';
    static const char stringPrefix    = 'S';
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Settings_ABC() {};
    virtual ~Settings_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual void BeginGroup( const QString& prefix ) = 0;
    virtual void EndGroup() = 0;
    virtual void RemoveKey( const QString& key ) = 0;

    virtual QStringList EntryList( const char* path ) = 0;
    virtual QStringList SubEntriesList( const char* path ) = 0;

    virtual void Save( const std::string& name, int value ) = 0;
    virtual void Save( const std::string& name, bool value ) = 0;
    virtual void Save( const std::string& name, float value ) = 0;
    virtual void Save( const std::string& name, const TristateOption& value ) = 0;
    virtual void Save( const std::string& name, const FourStateOption& value ) = 0;
    virtual void Save( const std::string& name, const QString& value ) = 0;

    virtual bool Remove( const std::string& name ) = 0;

    template< typename T >
    void Save( const std::string& , const T& )
    {
    }

    virtual int            Load( const std::string& name, int defaultValue ) = 0;
    virtual bool           Load( const std::string& name, bool defaultValue ) = 0;
    virtual float          Load( const std::string& name, float defaultValue ) = 0;
    virtual TristateOption Load( const std::string& name, const TristateOption& defaultValue ) = 0;
    virtual FourStateOption Load( const std::string& name, const FourStateOption& defaultValue ) = 0;
    virtual QString        Load( const std::string& name, const QString& defaultValue ) = 0;

    template< typename T >
    T Load( const std::string& , const T& defaultValue )
    {
        return defaultValue;
    }
    //@}
};

}

#endif // __Settings_ABC_h_
