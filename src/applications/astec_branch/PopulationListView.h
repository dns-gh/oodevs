// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationListView_h_
#define __PopulationListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

class Team;
class Population;
class SelectedElement;
class ActionContext;
class AgentManager;


// =============================================================================
/** @class  PopulationListView
    @brief  PopulationListView
    @par    Using example
    @code
    PopulationListView;
    @endcode
*/
// Created: HME 2005-10-03
// =============================================================================
class PopulationListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( PopulationListView );

private:
    enum
    {
        eTeam  = 1000,
        eGtia  = 1001,
        eAgent = 1002
    };
public:
    //! @name Constructors/Destructor
    //@{
             PopulationListView( QWidget* pParent );
    virtual ~PopulationListView();
    //@}

signals:
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void ElementSelected( SelectedElement& selectedElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const ActionContext& context );

public slots:
	void AddPopulation( Population& population );
    void SetSelectedElement( SelectedElement& selectedElement );

private:
    void keyPressEvent( QKeyEvent* pEvent );

private slots:
	void OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int nCol );
    void OnRequestCenter();

    void OnSelectionChange( QListViewItem* pItem );
    void OnTeamChanged();

private:
    Team*       ToTeam ( QListViewItem* pItem );
	Population* ToPopulation( QListViewItem* pItem );
    void            RemovePopulation( Population& population );

private:
    QPopupMenu* pPopupMenu_;
};



#include "PopulationListView.inl"

#endif // __PopulationListView_h_
