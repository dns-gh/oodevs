// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Bookmarks_h_
#define __Bookmarks_h_

#include "ContextMenuObserver_ABC.h"
#include "Observer_ABC.h"

class Controllers;
class Agent;
class QToolBox;

// =============================================================================
/** @class  Bookmarks
    @brief  Bookmarks
*/
// Created: SBO 2006-06-21
// =============================================================================
class Bookmarks : public QVBox
                , public Observer_ABC
                , public ContextMenuObserver_ABC< Agent >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Bookmarks( QWidget* parent, Controllers& controllers );
    virtual ~Bookmarks();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const Agent&, QPopupMenu& );
    //@}

private slots:
    //! @name Slots
    //@{
    void Bookmark();
    void RemoveAll();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Bookmarks( const Bookmarks& );            //!< Copy constructor
    Bookmarks& operator=( const Bookmarks& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::vector< const Agent* >   T_Agents;
    typedef T_Agents::const_iterator    CIT_Agents;
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    T_Agents bookmarks_;
    const Agent* selected_;
    QToolBox* toolBox_;
    //@}
};

#endif // __Bookmarks_h_
