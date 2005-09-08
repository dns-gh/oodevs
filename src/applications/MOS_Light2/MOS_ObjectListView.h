// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-08-05 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectListView.h $
// $Author: Ape $
// $Modtime: 26/08/04 11:03 $
// $Revision: 3 $
// $Workfile: MOS_ObjectListView.h $
//
// *****************************************************************************

#ifndef __MOS_ObjectListView_h_
#define __MOS_ObjectListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

class MOS_SelectedElement;
class MOS_ActionContext;
class MOS_Object_ABC;


// =============================================================================
/** @class  MOS_ObjectListView
    @brief  MOS_ObjectListView
    @par    Using example
    @code
    MOS_ObjectListView;
    @endcode
*/
// Created: APE 2004-08-05
// =============================================================================
class MOS_ObjectListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ObjectListView );

private:
    enum 
    {
        eTeam,
        eObjectType,
        eObject
    };

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ObjectListView( QWidget* pParent );
    ~MOS_ObjectListView();
    //@}

    //! @name Operations
    //@{
     QSize sizeHint() const;
    //@}

public slots:
    void AddObject( MOS_Object_ABC& object );
    void RemoveObject( MOS_Object_ABC& object );
    void SetSelectedElement( MOS_SelectedElement& selectedElement );

private slots:
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int nCol );
    void OnRequestCenter();

    void OnSelectionChange( QListViewItem* pItem );
    void OnTeamChanged();

signals:
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void ElementSelected( MOS_SelectedElement& selectedElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );

private:
    //! @name Helpers
    //@{
    MOS_Object_ABC* ToObject( QListViewItem* pItem );
    //@}

    void keyPressEvent( QKeyEvent* pEvent );

private:
    //! @name Member data
    //@{
    QPopupMenu* pPopupMenu_;
    //@}
};


#ifdef MOS_USE_INLINE
#   include "MOS_ObjectListView.inl"
#endif

#endif // __MOS_ObjectListView_h_
