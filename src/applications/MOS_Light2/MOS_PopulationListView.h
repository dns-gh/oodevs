// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MOS_PopulationListView_h_
#define __MOS_PopulationListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

class MOS_Team;
class MOS_Population;
class MOS_SelectedElement;
class MOS_ActionContext;
class MOS_AgentManager;


// =============================================================================
/** @class  MOS_PopulationListView
    @brief  MOS_PopulationListView
    @par    Using example
    @code
    MOS_PopulationListView;
    @endcode
*/
// Created: HME 2005-10-03
// =============================================================================
class MOS_PopulationListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_PopulationListView );

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
             MOS_PopulationListView( QWidget* pParent );
    virtual ~MOS_PopulationListView();
    //@}

signals:
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void ElementSelected( MOS_SelectedElement& selectedElement );
    void NewPopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );

public slots:
	void AddPopulation( MOS_Population& population );
    void SetSelectedElement( MOS_SelectedElement& selectedElement );

private:
    void keyPressEvent( QKeyEvent* pEvent );

private slots:
	void OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int nCol );
    void OnRequestCenter();

    void OnSelectionChange( QListViewItem* pItem );
    void OnTeamChanged();

private:
    MOS_Team*       ToTeam ( QListViewItem* pItem );
	MOS_Population* ToPopulation( QListViewItem* pItem );
    void            RemovePopulation( MOS_Population& population );

private:
    QPopupMenu* pPopupMenu_;
};



#include "MOS_PopulationListView.inl"

#endif // __MOS_PopulationListView_h_
