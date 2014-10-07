// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __TerrainToolBar_h_
#define __TerrainToolBar_h_

#include "clients_gui/RichToolBar.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/SafePointer.h"
#include "clients_kernel/UrbanColor_ABC.h"
#include <tools/SelectionObserver_ABC.h>

namespace gui
{
    class ExclusiveEventStrategy;
    class ParametersLayer;
    template< typename T > class RichWidget;
}

namespace kernel
{
    class Controllers;
    class Location_ABC;
    class UrbanObject_ABC;
}

class RemoveBlocksDialog;
class UrbanModel;

// =============================================================================
/** @class  TerrainToolBar
    @brief  TerrainToolBar
*/
// Created: ABR 2012-05-15
// =============================================================================
class TerrainToolBar : public gui::RichToolBar
                     , public tools::SelectionObserver< kernel::UrbanObject_ABC >
                     , public kernel::ContextMenuObserver_ABC< kernel::UrbanObject_ABC >
                     , public gui::ShapeHandler_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TerrainToolBar( QWidget* parent,
                             kernel::Controllers& controllers,
                             gui::ExclusiveEventStrategy& eventStrategy,
                             const std::shared_ptr< gui::ParametersLayer >& paramLayer,
                             UrbanModel& urbanModel,
                             RemoveBlocksDialog& removeBlocksDialog );
    virtual ~TerrainToolBar();
    //@}

    //! @name SelectionObserver
    //@{
    virtual void NotifySelected( const kernel::UrbanObject_ABC* urbanObject );
    //@}

    //! @name ContextMenuObserver_ABC
    //@{
    virtual void NotifyContextMenu( const kernel::UrbanObject_ABC& object, kernel::ContextMenu& menu );
    //@}

    //! @name ShapeHandler_ABC
    //@{
    virtual void Handle( kernel::Location_ABC& location );
    //@}

    //! @name DisplayableModeObserver_ABC
    //@{
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode );
    //@}

private:
    //! @name Helpers
    //@{
    void UncheckBlockCreationButtons();
    void EnableBlockCreationButtons( bool enabled );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnSwitchMode();
    void OnBlockCreation();
    void OnBlockCreationAuto();
    void OnChangeShape();
    void OnBlink();
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< kernel::SafePointer< kernel::UrbanObject_ABC >*, kernel::UrbanBlockColor > T_UrbanColors;
    //@}

    //! @name Member data
    //@{
    gui::ExclusiveEventStrategy& eventStrategy_;
    std::shared_ptr< gui::ParametersLayer > paramLayer_;
    UrbanModel& urbanModel_;
    kernel::SafePointer< kernel::UrbanObject_ABC > selected_;
    gui::RichWidget< QToolButton >* switchModeButton_;
    gui::RichWidget< QToolButton >* blockCreationButton_;
    gui::RichWidget< QToolButton >* blockCreationAutoButton_;
    gui::RichWidget< QToolButton >* blockRemoveButton_;
    QDoubleSpinBox* roadWidthSpinBox_;
    T_UrbanColors urbanColors_;
    unsigned char blinks_;
    bool isAuto_;
    bool changingGeom_;
    //@}
};

#endif // __TerrainToolBar_h_
