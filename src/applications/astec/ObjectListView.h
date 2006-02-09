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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectListView.h $
// $Author: Ape $
// $Modtime: 26/08/04 11:03 $
// $Revision: 3 $
// $Workfile: ObjectListView.h $
//
// *****************************************************************************

#ifndef __ObjectListView_h_
#define __ObjectListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

class SelectedElement;
class ActionContext;
class Object_ABC;


// =============================================================================
/** @class  ObjectListView
    @brief  ObjectListView
    @par    Using example
    @code
    ObjectListView;
    @endcode
*/
// Created: APE 2004-08-05
// =============================================================================
class ObjectListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ObjectListView );

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
     ObjectListView( QWidget* pParent );
    ~ObjectListView();
    //@}

    //! @name Operations
    //@{
     QSize sizeHint() const;
    //@}

public slots:
    void AddObject( Object_ABC& object );
    void RemoveObject( Object_ABC& object );
    void SetSelectedElement( SelectedElement& selectedElement );

private slots:
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int nCol );
    void OnRequestCenter();

    void OnSelectionChange( QListViewItem* pItem );
    void OnTeamChanged();

signals:
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void ElementSelected( SelectedElement& selectedElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const ActionContext& context );

private:
    //! @name Helpers
    //@{
    Object_ABC* ToObject( QListViewItem* pItem );
    //@}

    void keyPressEvent( QKeyEvent* pEvent );

private:
    //! @name Member data
    //@{
    QPopupMenu* pPopupMenu_;
    //@}
};

#   include "ObjectListView.inl"

#endif // __ObjectListView_h_
