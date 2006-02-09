// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-10 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ReportListView.h $
// $Author: Ape $
// $Modtime: 4/10/04 15:42 $
// $Revision: 2 $
// $Workfile: ReportListView.h $
//
// *****************************************************************************

#ifndef __ReportListView_h_
#define __ReportListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_ValuedRichListViewItem.h"

class Agent_ABC;
class Report_ABC;
class ActionContext;
class SelectedElement;
class ReportFilterOptions;


// =============================================================================
/** @class  ReportListView
    @brief  Displays the unit reports and debug traces in a sorted listview.
*/
// Created: APE 2004-03-10
// =============================================================================
class ReportListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ReportListView );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     ReportListView( QWidget* pParent, const ReportFilterOptions& filter );
    ~ReportListView();
    //@}

    //! @name Operations
    //@{
    void SetAgent( Agent_ABC* pAgent );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnReportCreated( Agent_ABC& agent, Report_ABC& report );

    void OnClick        ( QListViewItem*, const QPoint&, int );
    void OnRequestCenter();
    void OnRequestPopup ( QListViewItem* pItem, const QPoint& pos, int nCol );
    void OnClearAll     ();
    void OnClearTrace   ();
    void OnClearUpTo    ();

    void NotifyReadingReports();
    void OnOptionsChanged    ();
    //@}

private:
    //! @name Helpers
    //@{
    void hideEvent( QHideEvent* pEvent );
    void showEvent( QShowEvent* pEvent );

    Report_ABC& GetItemValue ( QListViewItem& item );
    bool            InterpretLink( const QString& strLink, const QString& strKeyword, int& nResultId );
    //@}

signals:
    //! @name Signals
    //@{
    void ElementSelected( SelectedElement& selectedElement );
    void CenterOnPoint  ( const MT_Vector2D& vPoint );
    void NewPopupMenu   ( QPopupMenu& popupMenu, const ActionContext& context );
    void ReadingReports ( Agent_ABC& agent );
    //@}

private:
    //! @name Types
    //@{
    enum
    { 
        eRichItem = 1000,
        eItem = 100
    };
    typedef MT_ValuedListViewItem    < Report_ABC*, eItem >     T_ReportItem;
    typedef MT_ValuedRichListViewItem< Report_ABC*, eRichItem > T_RichReportItem;
    //@}
    
    //! @name Member data
    //@{
    const ReportFilterOptions& filter_;
    Agent_ABC*                 pAgent_;
    QPopupMenu*                    pPopupMenu_;
    QListViewItem*                 pPopupItem_;
    //@}
};

//$$$$$ Remarque générale: Attacher les popupmenus à leur listitem pour éviter les
// pb en cas d'effacement de l'item, ou gérer ca autrement.

#   include "ReportListView.inl"

#endif // __ReportListView_h_
