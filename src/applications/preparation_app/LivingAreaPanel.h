// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LivingAreaPanel_h_
#define __LivingAreaPanel_h_

#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "LivingAreaEditor_ABC.h"

namespace kernel
{
    class Controllers;
    class Inhabitant_ABC;
    class Viewport_ABC;
    class GlTools_ABC;
    class Location_ABC;
}

namespace gui
{
    class ParametersLayer;
}


// =============================================================================
/** @class  LivingAreaPanel
    @brief  Living area panel
*/
// Created: LGY 2012-01-05
// =============================================================================
class LivingAreaPanel : public QDockWidget
                      , public kernel::OptionsObserver_ABC
                      , public tools::Observer_ABC
                      , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
                      , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                      , public gui::ShapeHandler_ABC
                      , public LivingAreaEditor_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LivingAreaPanel( QMainWindow* parent, kernel::Controllers& controllers, gui::ParametersLayer& paramLayer,
                              const kernel::GlTools_ABC& tools );
    virtual ~LivingAreaPanel();
    //@}

    //! @name Operations
    //@{
    virtual void Select( const geometry::Point2f& point );
    //@}

private:
    //! @name Operations
    //@{
    virtual void closeEvent( QCloseEvent* pEvent );
    virtual void NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyContextMenu( const geometry::Point2f& point, kernel::ContextMenu& menu );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    virtual void Handle( kernel::Location_ABC& location );
    virtual void Draw( kernel::Viewport_ABC& viewport );
    //@}

private slots:
    //! @name Slots
    //@{
    void Accept();
    void Reject();
    void Add();
    void Remove();
    void Update();
    //@}

private:
    //! @name Helpers
    //@{
    void Reset();
    void Valid();
    //@}

private:
    //! @name Enums
    //@{
    enum Mode
    {
        add,
        remove
    };
    //@}

private:
    //! @name Member Data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Inhabitant_ABC > selected_;
    gui::ParametersLayer& paramLayer_;
    const kernel::GlTools_ABC& tools_;
    Mode mode_;
    kernel::Location_ABC* location_;
    bool editorEnabled_;
    QLabel* text_;
    //@}
};
#endif // __LivingAreaPanel_h_
