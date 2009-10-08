// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __UnitPreviewIcon_h_
#define __UnitPreviewIcon_h_

#include "clients_kernel/SelectionObserver_ABC.h"
#include "clients_kernel/SafePointer.h"

namespace kernel
{
    class AgentType;
    class AutomatType;
    class Formation_ABC;
    class Automat_ABC;
    class Entity_ABC;
}

namespace gui
{
    class SymbolIcons;
    class ColorStrategy_ABC;

// =============================================================================
/** @class  UnitPreviewIcon
    @brief  UnitPreviewIcon
*/
// Created: SBO 2007-10-16
// =============================================================================
class UnitPreviewIcon : public QHBox
                      , public tools::Observer_ABC
                      , public kernel::SelectionObserver_ABC
                      , public kernel::SelectionObserver_Base< kernel::Formation_ABC >
                      , public kernel::SelectionObserver_Base< kernel::Automat_ABC >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             UnitPreviewIcon( QWidget* parent, kernel::Controllers& controllers, SymbolIcons& icons, ColorStrategy_ABC& colorStrategy );
    virtual ~UnitPreviewIcon();
    //@}

    //! @name Operations
    //@{
    void NotifySelected( const kernel::AgentType& type );
    void NotifySelected( const kernel::AutomatType& type );
    //@}

signals:
    //! @name Signals
    //@{
    void StartDrag();
    //@}

private slots:
    //! @name Slots
    //@{
    void UpdateSymbol();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    UnitPreviewIcon( const UnitPreviewIcon& );            //!< Copy constructor
    UnitPreviewIcon& operator=( const UnitPreviewIcon& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    virtual void BeforeSelection();
    virtual void AfterSelection();
    virtual void Select( const kernel::Formation_ABC& element );
    virtual void Select( const kernel::Automat_ABC& element );
    virtual void mouseMoveEvent( QMouseEvent* event );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    SymbolIcons& icons_;
    ColorStrategy_ABC& colorStrategy_;
    kernel::SafePointer< kernel::Entity_ABC > selectedParent_;
    QLabel* icon_;
    std::string symbol_, level_;
    //@}
};

}

#endif // __UnitPreviewIcon_h_
