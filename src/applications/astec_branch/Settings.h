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

// =============================================================================
// Created: APE 2004-06-01
// =============================================================================
class Settings : public QSettings
{

public:
    //! @name Constructors/Destructor
    //@{
             Settings();
    explicit Settings( QSettings::Format nFormat );
    virtual ~Settings();
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
