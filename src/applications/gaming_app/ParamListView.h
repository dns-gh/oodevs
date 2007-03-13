// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamListView_h_
#define __ParamListView_h_

// =============================================================================
/** @class  ParamListView
    @brief  ParamListView
*/
// Created: APE 2004-04-19
// =============================================================================
class ParamListView : public QObject
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamListView( QWidget* pParent, const QString& label );
    virtual ~ParamListView();
    //@}

    //! @name Operations
    //@{
    bool Invalid();
    QListView* ListView(); // $$$$ SBO 2007-03-13: bof bof
    //@}

public slots:
    //! @name slots
    //@{
    virtual void OnRequestPopup( QListViewItem* pItem, const QPoint& pos );
    virtual void OnDeleteSelectedItem();
    virtual void OnClearList();
    virtual void TurnHeaderBlack();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamListView( const ParamListView& );
    ParamListView& operator=( const ParamListView& );
    //@}

private:
    //! @name Member data
    //@{
    QListView* list_;
    QPopupMenu* pPopupMenu_;
    //@}
};

#endif // __ParamListView_h_
