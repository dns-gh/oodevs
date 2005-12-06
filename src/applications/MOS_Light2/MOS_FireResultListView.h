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

class MOS_Agent;
class MOS_Object_ABC;

// =============================================================================
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
    void SetAgent ( MOS_Agent*      pAgent  );
    void SetObject( MOS_Object_ABC* pObject );
    //@}

private slots:
    //! @name slots
    //@{
    void OnAgentConflictEnded( MOS_Agent&      agent  );
    void OnObjectExplosion   ( MOS_Object_ABC& object );
    //@}

private:
    //! @name Helpers
    //@{
    void OnFireResultCreated ( const T_FireResults& fireResults );
    void BuildEquipmentResult( const MOS_FireResult& result, QListViewItem& parentItem );
    void BuildHumanResult    ( const MOS_FireResult& result, QListViewItem& parentItem );
    //@}

private:
    MOS_Agent*      pAgent_;
    MOS_Object_ABC* pObject_;
    QListViewItem*  pPopupItem_;
};

#endif // __MOS_FireResultListView_h_
