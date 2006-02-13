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

#ifndef __FireResultListView_h_
#define __FireResultListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_ValuedRichListViewItem.h"
#include "FireResult.h"

class Agent_ABC;
class Object_ABC;

// =============================================================================
// Created: SBO 2005-08-30
// =============================================================================
class FireResultListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( FireResultListView );

public:
    //! @name Constructors/Destructor
    //@{
             FireResultListView( QWidget* pParent );
    virtual ~FireResultListView();
    //@}

    //! @name Operations
    //@{
    void SetOrigin( Agent_ABC*  pOrigin );
    void SetObject( Object_ABC* pObject );
    //@}

private slots:
    //! @name slots
    //@{
    void OnConflictEnded  ( Agent_ABC&  origin );
    void OnObjectExplosion( Object_ABC& object );
    //@}

private:
    //! @name Helpers
    //@{
    void OnFireResultCreated ( const T_FireResults& fireResults );
    void BuildEquipmentResult( const FireResult& result, QListViewItem& parentItem );
    void BuildHumanResult    ( const FireResult& result, QListViewItem& parentItem );
    //@}

private:
    Agent_ABC*  pOrigin_;
    Object_ABC* pObject_;
    QListViewItem*  pPopupItem_;
};

#endif // __FireResultListView_h_
