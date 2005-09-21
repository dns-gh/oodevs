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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ReportListView.h $
// $Author: Ape $
// $Modtime: 4/10/04 15:42 $
// $Revision: 2 $
// $Workfile: MOS_ReportListView.h $
//
// *****************************************************************************

#ifndef __MOS_ReportListView_h_
#define __MOS_ReportListView_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_ValuedRichListViewItem.h"

class MOS_Agent;
class MOS_Report_ABC;
class MOS_RC;
class MOS_Trace;
class MOS_ActionContext;
class MOS_SelectedElement;
class MOS_ReportFilterOptions;

// =============================================================================
/** @class  MOS_ReportListView
    @brief  Displays the unit reports and debug traces in a sorted listview.
*/
// Created: APE 2004-03-10
// =============================================================================
class MOS_ReportListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ReportListView );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ReportListView( QWidget* pParent, const MOS_ReportFilterOptions& filter );
    ~MOS_ReportListView();
    //@}

    //! @name Operations
    //@{
    void SetAgent( MOS_Agent* pAgent );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnReportCreated( MOS_Agent& agent, MOS_Report_ABC& report );

    void OnClick( QListViewItem*, const QPoint&, int );
    void OnRequestCenter();
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos, int nCol );
    void OnClearAll();
    void OnClearTrace();
    void OnClearUpTo();

    void NotifyReadingReports();
    void OnOptionsChanged();
    //@}

private:
    //! @name Helpers
    //@{
    void hideEvent( QHideEvent* pEvent );
    void showEvent( QShowEvent* pEvent );

    MOS_Report_ABC& GetItemValue( QListViewItem& item );
    bool InterpretLink( const QString& strLink, const QString& strKeyword, int& nResultId );
    //@}

signals:
    //! @name Signals
    //@{
    void ElementSelected( MOS_SelectedElement& selectedElement );
    void CenterOnPoint( const MT_Vector2D& vPoint );
    void NewPopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    void ReadingReports( MOS_Agent& agent );
    //@}

private:
    //! @name Types
    //@{
    enum { eRichItem = 1000, eItem = 1001 };
    typedef MT_ValuedListViewItem< MOS_Report_ABC*, eItem > T_ReportItem;
    typedef MT_ValuedRichListViewItem< MOS_Report_ABC*, eRichItem > T_RichReportItem;
    //@}
    
    //! @name Member data
    //@{
    const MOS_ReportFilterOptions& filter_;
    MOS_Agent* pAgent_;
    QPopupMenu* pPopupMenu_;
    QListViewItem* pPopupItem_;
    //@}
};

//$$$$$ Remarque générale: Attacher les popupmenus à leur listitem pour éviter les
// pb en cas d'effacement de l'item, ou gérer ca autrement.

#   include "MOS_ReportListView.inl"

#endif // __MOS_ReportListView_h_
