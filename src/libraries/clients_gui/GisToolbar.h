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
#include "RichCheckBox.h"
#include "RichWidget.h"
#include "RichSpinBox.h"

#include "clients_kernel/OptionsObserver_ABC.h"
#include <tools/ElementObserver_ABC.h>

#pragma warning( push, 0 )
#include <QtGui/qtoolbar.h>
#pragma warning( pop )

class QColor;

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class ModelLoaded;
}

namespace gui
{
    class ColorButton;
    class TerrainProfiler;
    class TerrainProfilerLayer;
    class ContourLinesObserver;
    class RichDockWidget;
    template< typename T > class RichWidget;

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
             GisToolbar( QMainWindow* parent, kernel::Controllers& controllers, const kernel::DetectionMap& detection, gui::TerrainProfiler& terrainProfiler );
    virtual ~GisToolbar();
    //@}

    //! @name Accessors
    //@{
    RichWidget< QToolButton >* GetTerrainProfilerButton() const;
    //@}

private slots:
    //! @name Operations
    //@{
    void OnToggleWatershedEnabled( bool toggled );
    void OnModeChanged( int mode );
    void OnHeightChanged( int height );
    void OnColorChanged( const QColor& color );
    void OnToggleContourLinesEnabled( bool toggled );
    void OnLinesHeightChanged();
    void OnColorContourChanged( const QColor& color );
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
    RichCheckBox* watershedEnabled_;
    RichWidget< QComboBox >* mode_;
    RichSpinBox* height_;
    RichSpinBox* linesHeight_;
    ColorButton* color_;
    RichCheckBox* contourBoxEnabled_;
    ColorButton* colorContourLines_;
    QLabel* progress_;
    RichWidget< QToolButton >* terrainProfilerButton_;
    //@}
};

} //! namespace gui

#endif // __GisToolbar_h_
