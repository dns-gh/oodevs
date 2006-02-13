// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-19 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamListView.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 4 $
// $Workfile: ParamListView.h $
//
// *****************************************************************************

#ifndef __ParamListView_h_
#define __ParamListView_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  ParamListView
    @brief  ParamListView
    @par    Using example
    @code
    ParamListView;
    @endcode
*/
// Created: APE 2004-04-19
// =============================================================================
class ParamListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamListView );

public:
    //! @name Constructors/Destructor
    //@{
     ParamListView( const std::string& strLabel, bool bPopupMenu, QWidget* pParent );
    ~ParamListView();
    //@}

    void TurnHeaderRed( int nMSec );

private slots:
    //! @name Private slots
    //@{
    void OnRequestPopup( QListViewItem* pItem, const QPoint& pos );
    void OnDeleteSelectedItem();
    void OnClearList();

    void TurnHeaderBlack();
    //@}

protected:
    QPopupMenu* pPopupMenu_;
};

#endif // __ParamListView_h_
