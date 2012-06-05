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
#include "clients_kernel/SafePointer.h"
#include "tools/SelectionObserver_ABC.h"

namespace gui
{
    class ExclusiveEventStrategy;
    class ParametersLayer;
}

namespace kernel
{
    class Controllers;
    class Location_ABC;
    class UrbanObject_ABC;
}

class UrbanModel;

// =============================================================================
/** @class  TerrainToolBar
    @brief  TerrainToolBar
*/
// Created: ABR 2012-05-15
// =============================================================================
class TerrainToolBar : public gui::RichToolBar
                     , public tools::SelectionObserver< kernel::UrbanObject_ABC >
                     , public gui::ShapeHandler_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TerrainToolBar( QWidget* parent, kernel::Controllers& controllers, gui::ExclusiveEventStrategy& eventStrategy, gui::ParametersLayer& paramLayer, UrbanModel& urbanModel );
    virtual ~TerrainToolBar();
    //@}

    //! @name SelectionObserver
    //@{
    virtual void NotifySelected( const kernel::UrbanObject_ABC* urbanObject );
    //@}

    //! @name ShapeHandler_ABC
    //@{
    virtual void Handle( kernel::Location_ABC& location );
    //@}

    //! @name DisplayableModeObserver_ABC
    //@{
    virtual void NotifyModeChanged( int newMode, bool useDefault, bool firstChangeToSavedMode );
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
    void OnRemoveBlocks();
    //@}

private:
    //! @name Member data
    //@{
    gui::ExclusiveEventStrategy&                   eventStrategy_;
    gui::ParametersLayer&                          paramLayer_;
    UrbanModel&                                    urbanModel_;
    kernel::SafePointer< kernel::UrbanObject_ABC > selected_;
    QToolButton*                                   switchModeButton_;
    QToolButton*                                   blockCreationButton_;
    QToolButton*                                   blockCreationAutoButton_;
    QToolButton*                                   blockRemoveButton_;
    bool                                           isAuto_;
    //@}
};

#endif // __TerrainToolBar_h_
