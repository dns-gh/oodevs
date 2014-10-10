// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __GisToolbar_h_
#define __GisToolbar_h_

#include "RichToolBar.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class ModelLoaded;
}

namespace gui
{
    class TerrainProfiler;
    class ContourLinesObserver;

// =============================================================================
/** @class  GisToolbar
    @brief  GisToolbar
*/
// Created: SBO 2010-03-23
// =============================================================================
class GisToolbar : public RichToolBar
                 , public kernel::OptionsObserver_ABC
                 , public tools::ElementObserver_ABC< kernel::ModelLoaded >
                 , public tools::ElementObserver_ABC< ContourLinesObserver >
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             GisToolbar( QMainWindow* parent,
                         kernel::Controllers& controllers,
                         const kernel::DetectionMap& detection,
                         gui::TerrainProfiler& terrainProfiler );
    virtual ~GisToolbar();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnToggleWatershedEnabled( bool toggled );
    void OnModeChanged( int mode );
    void OnToggleContourLinesEnabled( bool toggled );
    void SetTerrainProfilerChecked( bool visible );
    //@}

private:
    //! @name Helpers
    //@{
    virtual void NotifyUpdated( const kernel::ModelLoaded& model );
    virtual void NotifyUpdated( const ContourLinesObserver& observer );
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::DetectionMap& detection_;
    QComboBox* mode_;
    QLabel* progress_;
    QToolButton* terrainProfilerButton_;

    QSpinBox* watershedHeight_;
    QToolButton* watershedColor_;

    QSpinBox* contourHeight_;
    QToolButton* contourColor_;
    //@}
};

} //! namespace gui

#endif // __GisToolbar_h_
