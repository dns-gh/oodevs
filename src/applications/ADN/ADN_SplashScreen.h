// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-18 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_SplashScreen.h $
// $Author: Ape $
// $Modtime: 1/04/05 18:16 $
// $Revision: 2 $
// $Workfile: ADN_SplashScreen.h $
//
// *****************************************************************************

#ifndef __ADN_SplashScreen_h_
#define __ADN_SplashScreen_h_

#include <qsplashscreen.h>
#include "ADN_ProgressIndicator_ABC.h"


// =============================================================================
/** @class  ADN_SplashScreen
    @brief  ADN_SplashScreen
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_SplashScreen
: public QSplashScreen
, public ADN_ProgressIndicator_ABC
{
    MT_COPYNOTALLOWED( ADN_SplashScreen )

public:
    //! @name Constructors/Destructor
    //@{
     ADN_SplashScreen( const QPixmap& pixmap = QPixmap(), WFlags f = 0 );
    ~ADN_SplashScreen();
    //@}

    //! @name Modifiers
    //@{
    void SetNbrOfSteps( int n );
    void SetTextColor( const QColor& color );
    //@}

    //! @name Operations
    //@{
    void Increment( int n = 1 );
    void Increment( const char* szText, int n = 1 );
    void Reset( const char* szMsg = 0 );
    //@}

private:
    //! @name Member data
    //@{
    QColor textColor_;
    //@}
};


#endif // __ADN_SplashScreen_h_
