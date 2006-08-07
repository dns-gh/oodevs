// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-01 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/Settings.h $
// $Author: Ape $
// $Modtime: 10/09/04 17:53 $
// $Revision: 2 $
// $Workfile: Settings.h $
//
// *****************************************************************************

#ifndef __Settings_h_
#define __Settings_h_

#include <qsettings.h>

class QDockWindow;
class QMainWindow;
class QWidget;
class TristateOption;

// =============================================================================
// Created: APE 2004-06-01
// =============================================================================
class Settings : public QSettings
{

public:
    //! @name Constants
    //@{
    static const char intPrefix      = 'I';
    static const char boolPrefix     = 'B';
    static const char floatPrefix    = 'F';
    static const char tristatePrefix = 'T';
    //@}

    //! @name Constructors/Destructor
    //@{
             Settings();
    explicit Settings( QSettings::Format nFormat );
    virtual ~Settings();
    //@}

    //! @name Helpers
    //@{
    void Save( const std::string& name, int value );
    void Save( const std::string& name, bool value );
    void Save( const std::string& name, float value );
    void Save( const std::string& name, const TristateOption& value );

    int            Load( const std::string& name, int defaultValue );
    bool           Load( const std::string& name, bool defaultValue );
    float          Load( const std::string& name, float defaultValue );
    TristateOption Load( const std::string& name, const TristateOption& defaultValue );

    template< typename T >
    void Save( const std::string&, T ) {};
    template< typename T >
    T Load( const std::string&, T defaultValue ) { return defaultValue; };
    //@}

    //! @name Operations
    //@{
    void WriteEntry( const QString& field, QWidget& widget );
    void ReadEntry( const QString& field, QWidget& widget, int nW, int nH, int nX, int nY, bool bVisible );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Settings( const Settings& );
    Settings& operator=( const Settings& );
    //@}
};

#endif // __Settings_h_
