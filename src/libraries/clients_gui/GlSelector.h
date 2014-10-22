// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GlSelector_h_
#define __GlSelector_h_

#include "clients_kernel/OptionsObserver_ABC.h"

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class Controllers;
    class DetectionMap;
}

class QGLWidget;
class MapLayer_ABC;
class EventStrategy_ABC;
class QStackedWidget;

namespace gui
{
    class CircularEventStrategy;
    class DrawingTypes;
    class ExclusiveEventStrategy;
    class Gl3dWidget;
    class GlPlaceHolder;
    class GlProxy;
    class GlWidget;
    class IconLayout;
    class Layer;

// =============================================================================
/** @class  GlSelector
    @brief  GlSelector
*/
// Created: AGE 2007-03-09
// =============================================================================
class GlSelector : public QStackedWidget
                 , public tools::Observer_ABC
                 , public kernel::OptionsObserver_ABC
{
    Q_OBJECT

public:
    //! @name Types
    //@{
    enum E_Widget
    {
        eWidget_Empty,
        eWidget_2D,
        eWidget_3D
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             GlSelector( QWidget* parent,
                         GlProxy& proxy,
                         kernel::Controllers& controllers,
                         const tools::ExerciseConfig& config,
                         kernel::DetectionMap& map,
                         EventStrategy_ABC& strategy,
                         const DrawingTypes& drawingTypes );
    virtual ~GlSelector();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Close();

    void ChangeTo( E_Widget type );

    void AddIcon( const char** xpm, int x, int y );
    void SetFocus();
    //@}

signals:
    //! @name Signals
    //@{
    void Widget2dChanged( gui::GlWidget* );
    void Widget3dChanged( gui::Gl3dWidget* );
    void MouseMove( const geometry::Point2f& );
    void MouseMove( const geometry::Point3f& );
    void UpdateGL();
    //@}

protected:
    //! @name Operations
    //@{
    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );
    //@}

private:
    //! @name Helpers
    //@{
    void Clean();
    void InitializePasses();
    //@}

private:
    //! @name Member data
    //@{
    GlProxy& proxy_;
    kernel::Controllers& controllers_;
    const tools::ExerciseConfig& config_;
    kernel::DetectionMap& map_;
    EventStrategy_ABC& strategy_;
    std::unique_ptr< IconLayout > iconLayout_;
    const DrawingTypes& drawingTypes_;

    QTimer* displayTimer_;
    std::shared_ptr< GlWidget > widget2d_;
    std::shared_ptr< Gl3dWidget > widget3d_;
    //@}
};

}

#endif // __GlSelector_h_
