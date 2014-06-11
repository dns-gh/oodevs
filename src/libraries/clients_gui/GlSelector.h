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
    class Logger_ABC;
}

class MapLayer_ABC;
class EventStrategy_ABC;
class QStackedWidget;

namespace gui
{
    class GlProxy;
    class CircularEventStrategy;
    class ExclusiveEventStrategy;
    class GlWidget;
    class Gl3dWidget;
    class GlPlaceHolder;
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
    //! @name Constructors/Destructor
    //@{
             GlSelector( QWidget* parent, GlProxy& proxy, kernel::Controllers& controllers, const tools::ExerciseConfig& config, kernel::DetectionMap& map, EventStrategy_ABC& strategy, kernel::Logger_ABC& logger );
    virtual ~GlSelector();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Close();

    void AddIcon( const char** xpm, int x, int y );
    void AddLayer( Layer& layer );
    void RemoveLayer( Layer& layer );

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

private slots:
    void OnUpdateGL();

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

public:
    static const int defaultFrameRate_;

private:
    //! @name Member data
    //@{
    GlProxy& proxy_;
    kernel::Controllers& controllers_;
    const tools::ExerciseConfig& config_;
    kernel::DetectionMap& map_;
    EventStrategy_ABC& strategy_;
    kernel::Logger_ABC& logger_;

    std::unique_ptr< IconLayout > iconLayout_;
    std::unique_ptr< MapLayer_ABC > moveLayer_;

    QTimer* displayTimer_;

    GlWidget* widget2d_;
    Gl3dWidget*               widget3d_;
    GlPlaceHolder*            glPlaceHolder_;

    bool b3d_;
    bool bDragMapWithWheel_;
    int refreshRate_;
    //@}
};

}

#endif // __GlSelector_h_
