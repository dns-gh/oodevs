// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __BigBrother_h_
#define __BigBrother_h_

#include "ContextMenuObserver_ABC.h"
#include "Observer_ABC.h"
#include "SafePointer.h"
#include "ElementObserver_ABC.h"

class Controllers;
class Agent;
class QToolBox;
class Report_ABC;

// =============================================================================
/** @class  BigBrother
    @brief  BigBrother
*/
// Created: SBO 2006-06-21
// =============================================================================
class BigBrother : public QVBox
                 , public Observer_ABC
                 , public ContextMenuObserver_ABC< Agent >
                 , public ElementObserver_ABC< Report_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             BigBrother( QWidget* parent, Controllers& controllers );
    virtual ~BigBrother();
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
    void OnSelect( int index );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    BigBrother( const BigBrother& );            //!< Copy constructor
    BigBrother& operator=( const BigBrother& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const Agent*, int >   T_Agents;
    typedef T_Agents::const_iterator      CIT_Agents;
    //@}

    //! @name Helpers
    //@{
    QWidget* CreateView( const Agent& agent );
    virtual void NotifyCreated( const Report_ABC& report );
    //@}

private:
    //! @name Member data
    //@{
    Controllers& controllers_;
    T_Agents spied_;
    SafePointer< Agent > selected_;
    QToolBox* toolBox_;
    //@}
};

#endif // __BigBrother_h_
