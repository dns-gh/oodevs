// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ADN_SplashScreen_h_
#define __ADN_SplashScreen_h_

#include <QtGui/qsplashscreen.h>
#include "ADN_ProgressIndicator_ABC.h"

// =============================================================================
/** @class  ADN_SplashScreen
    @brief  ADN_SplashScreen
*/
// Created: APE 2005-03-18
// =============================================================================
class ADN_SplashScreen : public QSplashScreen
                       , public ADN_ProgressIndicator_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit ADN_SplashScreen( const QPixmap& pixmap, Qt::WFlags f = 0 );
             ADN_SplashScreen();
    virtual ~ADN_SplashScreen();
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

    //! @name Accessors
    //@{
    bool IsValid() const;
    //@}

private:
    //! @name Member data
    //@{
    QColor textColor_;
    //@}
};


#endif // __ADN_SplashScreen_h_
