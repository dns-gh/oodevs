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

class TristateOption;
class QStringList;

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
    static const char intPrefix      = 'I';
    static const char boolPrefix     = 'B';
    static const char floatPrefix    = 'F';
    static const char tristatePrefix = 'T';
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Settings_ABC() {};
    virtual ~Settings_ABC() {};
    //@}

    //! @name Operations
    //@{
    virtual QStringList EntryList( const char* path ) = 0;

    virtual void Save( const std::string& name, int value ) = 0;
    virtual void Save( const std::string& name, bool value ) = 0;
    virtual void Save( const std::string& name, float value ) = 0;
    virtual void Save( const std::string& name, const TristateOption& value ) = 0;

    template< typename T >
    void Save( const std::string& , const T& )
    {
    }

    virtual int            Load( const std::string& name, int defaultValue ) = 0;
    virtual bool           Load( const std::string& name, bool defaultValue ) = 0;
    virtual float          Load( const std::string& name, float defaultValue ) = 0;
    virtual TristateOption Load( const std::string& name, const TristateOption& defaultValue ) = 0;

    template< typename T >
    T Load( const std::string& , const T& defaultValue )
    {
        return defaultValue;
    }
    //@}
};

#endif // __Settings_ABC_h_
