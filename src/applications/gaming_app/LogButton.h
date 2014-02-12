// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2014 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LogButton_h_
#define __LogButton_h_

#include <tools/SelectionObserver_ABC.h>
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

// =============================================================================
/** @class  LogButton
    @brief  Log button
*/
// Created: LGY 2014-02-10
// =============================================================================
class LogButton : public QPushButton
                , public tools::Observer_ABC
                , public tools::SelectionObserver< kernel::Entity_ABC >

{
    Q_OBJECT
public:
    //! @name Constructors/Destructor
    //@{
             LogButton( QWidget* parent, kernel::Controllers& controllers,
                        const QPixmap& pixmap );
    virtual ~LogButton();
    //@}

public slots:
    //! @name Slots
    //@{
    void ChangePixmap( bool disable );
    //@}

private:
    //! @name Operations
    //@{
    virtual void NotifySelected( const kernel::Entity_ABC* element );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const QPixmap enabledPixmap_;
    const QPixmap disabledPixmap_;
    //@}
};
#endif // __LogButton_h_
