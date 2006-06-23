// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __MiniViews_h_
#define __MiniViews_h_

#include "ContextMenuObserver_ABC.h"
#include "SafePointer.h"

class Agent;
class Controllers;
class SmartGridWidget;
class GlWidget;
class MiniView;

// =============================================================================
/** @class  MiniViews
    @brief  MiniViews
*/
// Created: AGE 2006-06-23
// =============================================================================
class MiniViews : public QDockWindow
                , public Observer_ABC
                , public ContextMenuObserver_ABC< Agent >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             MiniViews( QMainWindow* parent, Controllers& controllers, GlWidget* const& widget );
    virtual ~MiniViews();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnMiniView();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MiniViews( const MiniViews& );            //!< Copy constructor
    MiniViews& operator=( const MiniViews& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const Agent&, QPopupMenu& );
    //@}

    //! @name Types
    //@{
    typedef std::map< const Agent*, MiniView* > T_MiniViews;
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    GlWidget* const & widget_;
    SmartGridWidget* grid_;
    SafePointer< Agent > selected_;
    T_MiniViews miniViews_;
    //@}
};

#endif // __MiniViews_h_
