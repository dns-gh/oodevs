// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef gui_AggregateToolbar_h
#define gui_AggregateToolbar_h

#include <tools/Observer_ABC.h>
#include <tools/ElementObserver_ABC.h>
#include <vector>

namespace kernel
{
    class Automat_ABC;
    class Formation_ABC;
    class Controller;
    class ContextMenu;
}

namespace gui
{
    class AutomatsLayer;
    class FormationLayer;

// =============================================================================
/** @class  AggregateToolbar
    @brief  Aggregate toolbar
*/
// Created: LGY 2011-10-14
// =============================================================================
class AggregateToolbar : public QHBoxLayout
                       , public tools::Observer_ABC
                       , public tools::ElementObserver_ABC< kernel::Automat_ABC >
                       , public tools::ElementObserver_ABC< kernel::Formation_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             AggregateToolbar( kernel::Controller& controller,
                               const std::shared_ptr< AutomatsLayer >& automatsLayer,
                               const std::shared_ptr< FormationLayer >& formationsLayer,
                               bool showDisplayModes );
    virtual ~AggregateToolbar();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyCreated( const kernel::Automat_ABC& automat );
    virtual void NotifyDeleted( const kernel::Automat_ABC& automat );
    virtual void NotifyCreated( const kernel::Formation_ABC& formation );
    virtual void NotifyDeleted( const kernel::Formation_ABC& formation );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    AggregateToolbar( const AggregateToolbar& );            //!< Copy constructor
    AggregateToolbar& operator=( const AggregateToolbar& ); //!< Assignment operator
    //@}

signals:
    void LockDragAndDrop( bool lock );
    void ChangeDisplay( int mode );

private slots:
    //! @name Slots
    //@{
    void Aggregate();
    void DisaggregateAll();
    void Aggregate( int id );
    void OnLockDragAndDropToggled( bool toggled );
    void OnChangeDisplay( int id );
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< const kernel::Automat_ABC* > T_Automats;
    typedef T_Automats::iterator                     IT_Automats;
    typedef T_Automats::const_iterator              CIT_Automats;

    typedef std::vector< const kernel::Formation_ABC* > T_Formations;
    typedef T_Formations::iterator                     IT_Formations;
    typedef T_Formations::const_iterator              CIT_Formations;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const std::shared_ptr< AutomatsLayer >& automatsLayer_;
    const std::shared_ptr< FormationLayer >& formationsLayer_;
    T_Automats automats_;
    T_Formations formations_;
    kernel::ContextMenu* levelMenu_;
    kernel::ContextMenu* displayMenu_;
    //@}
};
}

#endif // gui_AggregateToolbar_h
