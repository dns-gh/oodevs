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
#include "MOS_FireResult.h"

class MOS_Agent_ABC;
class MOS_Object_ABC;

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
    void SetAgent ( MOS_Agent_ABC*      pAgent  );
    void SetObject( MOS_Object_ABC* pObject );
    //@}

private slots:
    //! @name slots
    //@{
    void OnAgentConflictEnded( MOS_Agent_ABC&      agent  );
    void OnObjectExplosion   ( MOS_Object_ABC& object );
    //@}

private:
    //! @name Operations
    //@{
    void OnFireResultCreated( const T_FireResults& fireResults );
    //@}

private:
    MOS_Agent_ABC*      pAgent_;
    MOS_Object_ABC* pObject_;
    QListViewItem*  pPopupItem_;
};

#include "MOS_FireResultListView.inl"

#endif // __MOS_FireResultListView_h_
