// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-30 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ODBToolbar.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:28 $
// $Revision: 2 $
// $Workfile: ODBToolbar.h $
//
// *****************************************************************************

#ifndef __ODBToolbar_h_
#define __ODBToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  ODBToolbar
*/
// Created: APE 2004-08-30
// =============================================================================
class ODBToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ODBToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     ODBToolbar( QMainWindow* pParent );
    ~ODBToolbar();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnOpen();
    void OnSave();
    void OnSaveAs();
    //@}

signals:
    void ODBLoaded();
};

#endif // __ODBToolbar_h_
