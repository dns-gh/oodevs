// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __GLStackedWidget_h_
#define __GLStackedWidget_h_

namespace kernel
{
    class DetectionMap;
}

namespace tools
{
    class ExerciseConfig;
}

class EventStrategy_ABC;
class QStackedWidget;

namespace gui
{
    class DrawingTypes;
    class GL2DWidget;
    class GL3DWidget;
    class GLView_ABC;
    class IconLayout;
    class Layer;

// =============================================================================
/** @class  GLStackedWidget
    @brief  GLStackedWidget
*/
// Created: AGE 2007-03-09
// =============================================================================
class GLStackedWidget : public QStackedWidget
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
             GLStackedWidget( QWidget* parent,
                              const std::shared_ptr< GLView_ABC >& proxy,
                              const tools::ExerciseConfig& config,
                              kernel::DetectionMap& map,
                              EventStrategy_ABC& strategy,
                              const std::shared_ptr< IconLayout >& iconLayout,
                              QGLWidget* shareWidget = 0 );
    virtual ~GLStackedWidget();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Close();
    void ChangeTo( E_Widget type );
    void SetFocus();
    std::shared_ptr< GLView_ABC > GetProxy() const;
    std::shared_ptr< GL2DWidget > GetWidget2d() const;
    std::shared_ptr< QGLWidget > GetCurrentWidget() const;
    //@}

signals:
    //! @name Signals
    //@{
    void MouseMove( const geometry::Point2f& );
    void MouseMove( const geometry::Point3f& );
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
    const tools::ExerciseConfig& config_;
    kernel::DetectionMap& map_;
    EventStrategy_ABC& strategy_;
    std::shared_ptr< IconLayout > iconLayout_;
    QGLWidget* shareWidget_;

    std::shared_ptr< GLView_ABC > proxy_;
    std::shared_ptr< GL2DWidget > widget2d_;
    std::shared_ptr< GL3DWidget > widget3d_;
    //@}
};

} //! namespace gui

#endif // __GLStackedWidget_h_
