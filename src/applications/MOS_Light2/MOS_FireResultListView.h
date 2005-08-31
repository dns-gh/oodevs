// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: SBO 2005-08-30 $
// $Archive: $
// $Author: $
// $Modtime: $
// $Revision: $
// $Workfile: $
//
// *****************************************************************************

#ifndef __MOS_FireResultListView_h_
#define __MOS_FireResultListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_ValuedRichListViewItem.h"

class MOS_Agent;

// =============================================================================
/** @class  MOS_FireResultListView
    @brief  MOS_FireResultListView
    @par    Using example
    @code
    MOS_FireResultListView;
    @endcode
*/
// Created: SBO 2005-08-30
// =============================================================================
class MOS_FireResultListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_FireResultListView );

public:
    //! @name Constructors/Destructor
    //@{
             MOS_FireResultListView( QWidget* pParent );
    virtual ~MOS_FireResultListView();
    //@}

    //! @name Operations
    //@{
    void SetAgent( MOS_Agent* pAgent );
    //@}

private slots:
    //! @name slots
    //@{
    void OnFireResultCreated( MOS_Agent& agent );
    //@}

private:
    MOS_Agent*     pAgent_;
    QListViewItem* pPopupItem_;
};

#include "MOS_FireResultListView.inl"

#endif // __MOS_FireResultListView_h_
