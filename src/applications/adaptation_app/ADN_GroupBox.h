// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-01-03 $
// $Archive: /MVW_v10/Build/SDK/Adn2/src/ADN_GroupBox.h $
// $Author: Ape $
// $Modtime: 3/01/05 17:01 $
// $Revision: 1 $
// $Workfile: ADN_GroupBox.h $
//
// *****************************************************************************

#ifndef __ADN_GroupBox_h_
#define __ADN_GroupBox_h_

#include "ADN_Gfx_ABC.h"

#include <Qt3Support/q3groupbox.h>

// =============================================================================
/** @class  ADN_GroupBox
    @brief  ADN_GroupBox
    @par    Using example
    @code
    ADN_GroupBox;
    @endcode
*/
// Created: APE 2005-01-03
// =============================================================================
class ADN_GroupBox
: public Q3GroupBox
, public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    ADN_GroupBox( QWidget * parent = 0, const char * name = 0 );
    ADN_GroupBox( const QString & title, QWidget * parent = 0, const char * name = 0 );
    ADN_GroupBox( int strips, Qt::Orientation orientation, QWidget * parent = 0, const char * name = 0 );
    ADN_GroupBox( int strips, Qt::Orientation orientation, const QString & title, QWidget * parent = 0, const char * name = 0 );
    virtual ~ADN_GroupBox();
    //@}

private slots:
    //! @name Internal slots
    //@{
    void BoolChanged(bool b);
    //@}
};

// =============================================================================
/** @class  ADN_GroupBox2
    @brief  ADN_GroupBox2
*/
// Created: ABR 2012-01-18
// =============================================================================
class ADN_GroupBox2
    : public QGroupBox // $$$$ ABR 2012-01-23: QT4 allows layout control
    , public ADN_Gfx_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    ADN_GroupBox2( QWidget * parent = 0, const char * name = 0 );
    ADN_GroupBox2( const QString & title, QWidget * parent = 0, const char * name = 0 );
    virtual ~ADN_GroupBox2();
    //@}

private slots:
    //! @name Internal slots
    //@{
    void BoolChanged(bool b);
    //@}
};

#endif // __ADN_GroupBox_h_
