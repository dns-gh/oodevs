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
#include "tools/ElementObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"

#pragma warning( push, 0 )
#include <QtGui/qtoolbar.h>
#pragma warning( pop )

class QColor;
class QCheckBox;
class QComboBox;
class QSpinBox;

namespace kernel
{
    class Controllers;
    class DetectionMap;
    class ModelLoaded;
}

namespace gui
{
    class ColorButton;
    class TerrainProfilerLayer;
    class ContourLinesObserver;
    class RichDockWidget;

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
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             GisToolbar( QMainWindow* parent, kernel::Controllers& controllers
                       , const kernel::DetectionMap& detection, TerrainProfilerLayer& layer );
    virtual ~GisToolbar();
    //@}

private slots:
    //! @name Operations
    //@{
    void OnToggleWatershedEnabled( bool toggled );
    void OnModeChanged( int mode );
    void OnHeightChanged( int height );
    void OnColorChanged( const QColor& color );
    void OnToggleCut( bool toggled );
    void OnToggleContourLinesEnabled( bool toggled );
    void OnLinesHeightChanged();
    void OnColorContourChanged( const QColor& color );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GisToolbar( const GisToolbar& );            //!< Copy constructor
    GisToolbar& operator=( const GisToolbar& ); //!< Assignment operator
    //@}

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
    RichDockWidget* terrainProfiler_;
    QCheckBox* watershedEnabled_;
    QComboBox* mode_;
    QSpinBox* height_;
    QSpinBox* linesHeight_;
    ColorButton* color_;
    QCheckBox* contourBoxEnabled_;
    ColorButton* colorContourLines_;
    QLabel* progress_;
    //@}
};

} //! namespace gui

#endif // __GisToolbar_h_
