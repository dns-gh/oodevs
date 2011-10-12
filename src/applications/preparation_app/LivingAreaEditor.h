// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LivingAreaEditor_h_
#define __LivingAreaEditor_h_

#include <boost/noncopyable.hpp>
#include "clients_kernel/Inhabitant_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "tools/Observer_ABC.h"

namespace kernel
{
    class Controllers;
    class ContextMenu;
}

namespace gui
{
    class RichLabel;
    class LocationCreator;
    class ParametersLayer;
}

// =============================================================================
/** @class  LivingAreaEditor
    @brief  Living area editor
*/
// Created: LGY 2011-09-19
// =============================================================================
class LivingAreaEditor : public QDialog
                       , private boost::noncopyable
                       , public tools::Observer_ABC
                       , public kernel::ContextMenuObserver_ABC< kernel::Inhabitant_ABC >
                       , public gui::ShapeHandler_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LivingAreaEditor( QWidget* parent, kernel::Controllers& controllers,
                               gui::ParametersLayer& layer, const kernel::GlTools_ABC& tools );
    virtual ~LivingAreaEditor();
    //@}

    //! @name Operations
    //@{
    virtual void NotifyContextMenu( const kernel::Inhabitant_ABC& entity, kernel::ContextMenu& menu );
    virtual void Handle( kernel::Location_ABC& location );
    virtual void Draw( kernel::Viewport_ABC& viewport );
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

private slots:
    //! @name Slots
    //@{
    void Add();
    void Remove();
    void closeEvent( QCloseEvent* pEvent );
    void Accept();
    void Reject();
    //@}

private:
    //! @name Helpers
    //@{
    void ResetLocation();
    bool CheckValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    kernel::SafePointer< kernel::Inhabitant_ABC > selected_;
    const kernel::GlTools_ABC& tools_;
    Mode mode_;
    gui::RichLabel* position_;
    QLabel* locationLabel_;
    gui::LocationCreator* locationCreator_;
    kernel::Location_ABC* location_;
    //@}
};

#endif // __LivingAreaEditor_h_
