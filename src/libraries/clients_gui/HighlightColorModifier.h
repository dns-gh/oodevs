// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __HighlightColorModifier_h_
#define __HighlightColorModifier_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "ColorModifier_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Agent_ABC;
    class AgentKnowledge_ABC;
    class Automat_ABC;
    class Controllers;
    class Entity_ABC;
    class Formation_ABC;
    class Ghost_ABC;
    class Population_ABC;
    class Object_ABC;
    class ObjectKnowledge_ABC;
    class Profile_ABC;
}

namespace gui
{

// =============================================================================
/** @class  HighlightColorModifier
    @brief  Highlight color modifier
*/
// Created: AGE 2008-05-15
// =============================================================================
class HighlightColorModifier : public QObject
                             , public ColorModifier_ABC
                             , public tools::Observer_ABC
                             , public kernel::ContextMenuObserver_ABC< kernel::Agent_ABC >
                             , public kernel::ContextMenuObserver_ABC< kernel::Automat_ABC >
                             , public kernel::ContextMenuObserver_ABC< kernel::Formation_ABC >
                             , public kernel::ContextMenuObserver_ABC< kernel::Ghost_ABC >
                             , public kernel::ContextMenuObserver_ABC< kernel::Population_ABC >
                             , public kernel::ContextMenuObserver_ABC< kernel::Object_ABC >
                             , public kernel::ContextMenuObserver_ABC< kernel::AgentKnowledge_ABC >
                             , public kernel::ContextMenuObserver_ABC< kernel::ObjectKnowledge_ABC >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             HighlightColorModifier( kernel::Controllers& controllers, const kernel::Profile_ABC& profile );
    virtual ~HighlightColorModifier();
    //@}

    //! @name Operations
    //@{
    virtual QColor Apply( const kernel::Entity_ABC& entity, const QColor& base ) const;
    virtual float Apply( const kernel::Entity_ABC& entity, float alpha ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void ToggleHighlight();
    void OnBlink();
    //@}

    //! @name Helpers
    //@{
    virtual void UpdateContextMenu( const kernel::Entity_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Agent_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Automat_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Formation_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Ghost_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Population_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::Object_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::AgentKnowledge_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const kernel::ObjectKnowledge_ABC& entity, kernel::ContextMenu& menu );
    QColor Highlight( const QColor& base ) const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::Profile_ABC& profile_;
    kernel::SafePointer< kernel::Entity_ABC > selected_;
    bool blink_;
    //@}
};

}

#endif // __HighlightColorModifier_h_
