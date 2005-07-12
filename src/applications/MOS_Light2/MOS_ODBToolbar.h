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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ODBToolbar.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:28 $
// $Revision: 2 $
// $Workfile: MOS_ODBToolbar.h $
//
// *****************************************************************************

#ifndef __MOS_ODBToolbar_h_
#define __MOS_ODBToolbar_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MOS_ODBToolbar
*/
// Created: APE 2004-08-30
// =============================================================================
class MOS_ODBToolbar : public QToolBar
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ODBToolbar );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ODBToolbar( QMainWindow* pParent );
    ~MOS_ODBToolbar();
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

#include "MOS_ODBToolbar.inl"

#endif // __MOS_ODBToolbar_h_
