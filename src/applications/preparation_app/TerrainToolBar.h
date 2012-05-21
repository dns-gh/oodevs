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

// =============================================================================
/** @class  TerrainToolBar
    @brief  TerrainToolBar
*/
// Created: ABR 2012-05-15
// =============================================================================
class TerrainToolBar : public gui::RichToolBar
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             TerrainToolBar( QWidget* parent, kernel::Controllers& controllers );
    virtual ~TerrainToolBar();
    //@}

    //! @name Operations
    //@{
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
    void OnBlockCreationMode();
    void OnBlockCreationAutoMode();
    void OnRemoveBlocks();
    //@}

private:
    //! @name Member data
    //@{
    QToolButton* switchModeButton_;
    QToolButton* blockCreationButton_;
    QToolButton* blockCreationAutoButton_;
    QToolButton* blockRemoveButton_;
    //@}
};

#endif // __TerrainToolBar_h_
