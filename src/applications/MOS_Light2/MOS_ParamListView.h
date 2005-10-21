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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamListView.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 4 $
// $Workfile: MOS_ParamListView.h $
//
// *****************************************************************************

#ifndef __MOS_ParamListView_h_
#define __MOS_ParamListView_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MOS_ParamListView
    @brief  MOS_ParamListView
    @par    Using example
    @code
    MOS_ParamListView;
    @endcode
*/
// Created: APE 2004-04-19
// =============================================================================
class MOS_ParamListView : public QListView
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamListView );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ParamListView( const std::string& strLabel, bool bPopupMenu, QWidget* pParent );
    ~MOS_ParamListView();
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

#endif // __MOS_ParamListView_h_
