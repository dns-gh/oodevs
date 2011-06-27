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
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "ColorModifier_ABC.h"

namespace kernel
{
    class Entity_ABC;
    class Controllers;
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
                             , public tools::ElementObserver_ABC    < kernel::Entity_ABC >
                             , public kernel::ContextMenuObserver_ABC< kernel::Entity_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
    explicit HighlightColorModifier( kernel::Controllers& controllers );
    virtual ~HighlightColorModifier();
    //@}

    //! @name Operations
    //@{
    virtual QColor Apply( const kernel::Entity_ABC& entity, const QColor& base );
    //@}

private slots:
    //! @name Slots
    //@{
    void Highlight();
    void Unhighlight();
    void OnBlink();
    //@}

    //! @name Helpers
    //@{
    virtual void NotifyContextMenu( const kernel::Entity_ABC&, kernel::ContextMenu& );
    virtual void NotifyDeleted( const kernel::Entity_ABC& );
    QColor Highlight( const QColor& base );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    std::set< const kernel::Entity_ABC* > highlighted_;
    kernel::SafePointer< const kernel::Entity_ABC > selected_;
    bool blink_;
    //@}
};

}

#endif // __HighlightColorModifier_h_
