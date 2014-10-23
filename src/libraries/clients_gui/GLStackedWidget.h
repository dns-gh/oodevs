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

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class DetectionMap;
}

class EventStrategy_ABC;
class QStackedWidget;

namespace gui
{
    class DrawingTypes;
    class Gl3dWidget;
    class GlProxy;
    class GlWidget;
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
                              GlProxy& proxy,
                              const tools::ExerciseConfig& config,
                              kernel::DetectionMap& map,
                              EventStrategy_ABC& strategy,
                              const DrawingTypes& drawingTypes,
                              const IconLayout& iconLayout );
    virtual ~GLStackedWidget();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Close();
    void ChangeTo( E_Widget type );
    void SetFocus();
    std::shared_ptr< GlWidget > GetWidget2d();
    //@}

    GlWidget* GetWidget2d() const;

signals:
    //! @name Signals
    //@{
    void MouseMove( const geometry::Point2f& );
    void MouseMove( const geometry::Point3f& );
    void UpdateGL();
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
    const tools::ExerciseConfig& config_;
    kernel::DetectionMap& map_;
    EventStrategy_ABC& strategy_;
    const DrawingTypes& drawingTypes_;
    const IconLayout& iconLayout_;

    std::shared_ptr< GlWidget > widget2d_;
    std::shared_ptr< Gl3dWidget > widget3d_;
    //@}
};

} //! namespace gui

#endif // __GLStackedWidget_h_
