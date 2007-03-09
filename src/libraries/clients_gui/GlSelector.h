// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __GlSelector_h_
#define __GlSelector_h_

#include "clients_kernel/OptionsObserver_ABC.h"

namespace kernel
{
    class Controllers;
    class ExerciseConfig;
    class DetectionMap;
}

class MapLayer_ABC;
class EventStrategy_ABC;

namespace gui
{
    class GlProxy;
    class CircularEventStrategy;
    class ExclusiveEventStrategy;
    class GlWidget;
    class Gl3dWidget;
    class GlPlaceHolder;
    class IconLayout;

// =============================================================================
/** @class  GlSelector
    @brief  GlSelector
*/
// Created: AGE 2007-03-09
// =============================================================================
class GlSelector : public QObject
                 , public kernel::Observer_ABC
                 , public kernel::OptionsObserver_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             GlSelector( QMainWindow* parent, GlProxy& proxy, kernel::Controllers& controllers, const kernel::ExerciseConfig& config, kernel::DetectionMap& map, EventStrategy_ABC& strategy );
    virtual ~GlSelector();
    //@}

    //! @name Operations
    //@{
    void Load();
    void Close();

    void AddIcon( const char** xpm, int x, int y );
    //@}

signals:
    //! @name Signals
    //@{
    void Widget2dChanged( gui::GlWidget* );
    void MouseMove( const geometry::Point2f& );
    void MouseMove( const geometry::Point3f& );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GlSelector( const GlSelector& );            //!< Copy constructor
    GlSelector& operator=( const GlSelector& ); //!< Assignement operator
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
    //@}

private:
    //! @name Member data
    //@{
    QMainWindow* parent_;
    GlProxy& proxy_;
    kernel::Controllers& controllers_;
    const kernel::ExerciseConfig& config_;
    kernel::DetectionMap& map_;
    EventStrategy_ABC& strategy_;

    std::auto_ptr< IconLayout > iconLayout_;
    std::auto_ptr< MapLayer_ABC >    moveLayer_;

    QTimer* displayTimer_;

    GlWidget*                 widget2d_;
    Gl3dWidget*               widget3d_;
    GlPlaceHolder*            glPlaceHolder_;

    bool b3d_;
    //@}
};

}

#endif // __GlSelector_h_
