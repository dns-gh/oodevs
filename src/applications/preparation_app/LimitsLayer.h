// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimitsLayer_h_
#define __LimitsLayer_h_

#include "clients_gui/Layer_ABC.h"
#include "clients_gui/ShapeHandler_ABC.h"
#include "clients_kernel/Observer_ABC.h"
#include "clients_kernel/ElementObserver_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "clients_kernel/OptionsObserver_ABC.h"
#include "clients_kernel/TristateOption.h"
#include "clients_kernel/LocationVisitor_ABC.h"
#include "clients_kernel/TacticalLine_ABC.h"
#include "clients_gui/EntityLayer.h"

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Location_ABC;
}

namespace gui
{
    class ColorStrategy_ABC;
    class ParametersLayer;
    class View_ABC;
}

class Lima;
class Limit;
class ModelBuilder;
class TacticalLine_ABC;

// =============================================================================
/** @class  LimitsLayer
    @brief  LimitsLayer
*/
// Created: AGE 2006-03-24
// =============================================================================
class LimitsLayer : public QObject
                  , public gui::EntityLayer< kernel::TacticalLine_ABC >
                  , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                  , public kernel::OptionsObserver_ABC
                  , private gui::ShapeHandler_ABC
                  , private kernel::LocationVisitor_ABC
                  
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LimitsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::ParametersLayer& parameters, ModelBuilder& modelBuilder, gui::View_ABC& view, const kernel::Profile_ABC& profile );
    virtual ~LimitsLayer();
    //@}

private slots:
    //! @name Slots
    //@{
    void OnCreateLimit();
    void OnCreateLima( int );
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LimitsLayer( const LimitsLayer& );            //!< Copy constructor
    LimitsLayer& operator=( const LimitsLayer& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool HandleKeyPress  ( QKeyEvent* key );
    virtual void NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& menu );
    virtual void NotifySelected( const kernel::TacticalLine_ABC* element );

    virtual void Handle( kernel::Location_ABC& location );
    virtual bool ShouldDisplay( const kernel::Entity_ABC& );

    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );

    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& ) {};
    virtual void VisitCircle ( const geometry::Point2f& , float ) {};
    virtual void VisitPoint  ( const geometry::Point2f& ) {};
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    gui::ColorStrategy_ABC& strategy_;
    gui::ParametersLayer& parameters_;
    ModelBuilder& modelBuilder_;

    kernel::TristateOption drawLines_;
    int type_;
    const kernel::TacticalLine_ABC* selected_;
    //@}
};

#endif // __LimitsLayer_h_
