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

#include "Param_ABC.h"

// =============================================================================
/** @class  ParamListView
    @brief  ParamListView
*/
// Created: APE 2004-04-19
// =============================================================================
class ParamListView : public QObject
                    , public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamListView( QObject* parent, const QString& name );
    virtual ~ParamListView();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void Commit();
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
