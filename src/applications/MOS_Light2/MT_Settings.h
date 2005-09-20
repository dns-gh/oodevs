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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_Settings.h $
// $Author: Ape $
// $Modtime: 10/09/04 17:53 $
// $Revision: 2 $
// $Workfile: MT_Settings.h $
//
// *****************************************************************************

#ifndef __MT_Settings_h_
#define __MT_Settings_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include <qsettings.h>

class QDockWindow;
class QMainWindow;

// =============================================================================
/** @class  MT_Settings
    @brief  MT_Settings
    @par    Using example
    @code
    MT_Settings;
    @endcode
*/
// Created: APE 2004-06-01
// =============================================================================
class MT_Settings : public QSettings
{
    MT_COPYNOTALLOWED( MT_Settings );

public:
    //! @name Constructors/Destructor
    //@{
    MT_Settings();
    MT_Settings( QSettings::Format nFormat );
    ~MT_Settings();
    //@}

    //! @name Operations
    //@{
    void WriteEntry( const QString& field, QWidget& widget );
    void ReadEntry( const QString& field, QWidget& widget, int nW, int nH, int nX, int nY, bool bVisible );
    //@}
};

#   include "MT_Settings.inl"

#endif // __MT_Settings_h_
