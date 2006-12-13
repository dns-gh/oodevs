// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Spy_h_
#define __Spy_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ElementObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class Agent_ABC;
    class Population_ABC;
    class Entity_ABC;
}

class QToolBox;
class Report;

// =============================================================================
/** @class  Spy
    @brief  Spy
*/
// Created: SBO 2006-06-21
// =============================================================================
class Spy : public QVBox
          , public kernel::Observer_ABC
          , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
          , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
          , public kernel::ElementObserver_ABC< kernel::Agent_ABC >
          , public kernel::ElementObserver_ABC< kernel::Population_ABC >
          , public kernel::ElementObserver_ABC< Report >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             Spy( QWidget* parent, kernel::Controllers& controllers );
    virtual ~Spy();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Agent_ABC&, kernel::ContextMenu& );
    virtual void NotifyContextMenu( const kernel::Population_ABC&, kernel::ContextMenu& );
    virtual void NotifyDeleted( const kernel::Agent_ABC& agent );
    virtual void NotifyDeleted( const kernel::Population_ABC& agent );
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
    Spy( const Spy& );            //!< Copy constructor
    Spy& operator=( const Spy& ); //!< Assignement operator
    //@}

    //! @name Types
    //@{
    typedef std::map< const kernel::Entity_ABC*, int >   T_Agents;
    typedef T_Agents::const_iterator                   CIT_Agents;
    //@}

    //! @name Helpers
    //@{
    QWidget* CreateView( const kernel::Entity_ABC& agent );
    virtual void NotifyCreated( const Report& report );
    void NotifyContextMenu( const kernel::Entity_ABC&, kernel::ContextMenu& menu );
    void RemoveAgent( const kernel::Entity_ABC& agent );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    T_Agents spied_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    QToolBox* toolBox_;
    //@}
};

#endif // __Spy_h_
