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

namespace kernel
{
    class Controllers;
    class GlTools_ABC;
    class Location_ABC;
}

namespace xml
{
    class xistream;
}

namespace gui
{
    class ColorStrategy_ABC;
    class ParametersLayer;
}

class Lima;
class Limit;
class LimitsModel;
class TacticalLine_ABC;

// =============================================================================
/** @class  LimitsLayer
    @brief  LimitsLayer
    // $$$$ AGE 2006-08-22: utiliser EntityLayer
    // $$$$ AGE 2006-09-06: Bouger dans gaming_app
*/
// Created: AGE 2006-03-24
// =============================================================================
class LimitsLayer : public QObject
                  , public gui::Layer_ABC
                  , public kernel::Observer_ABC
                  , public kernel::ElementObserver_ABC< Lima >
                  , public kernel::ElementObserver_ABC< Limit >
                  , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
                  , public kernel::OptionsObserver_ABC
                  , private gui::ShapeHandler_ABC
                  , private kernel::LocationVisitor_ABC
                  
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LimitsLayer( kernel::Controllers& controllers, const kernel::GlTools_ABC& tools, gui::ColorStrategy_ABC& strategy, gui::ParametersLayer& parameters, LimitsModel& model );
    virtual ~LimitsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
    //@}

public slots:
    //! @name 
    //@{
    void Load( const std::string& filename );
    void Save( const std::string& filename ) const;
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
    virtual void NotifyCreated( const Lima& );
    virtual void NotifyDeleted( const Lima& );

    virtual void NotifyCreated( const Limit& );
    virtual void NotifyDeleted( const Limit& );

    void Add( const TacticalLine_ABC& line );
    void Remove( const TacticalLine_ABC& line );

    virtual bool HandleKeyPress  ( QKeyEvent* key );
    virtual bool HandleMousePress( QMouseEvent* mouse, const geometry::Point2f& point );

    virtual void NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& menu );

    virtual bool IsInSelection( const TacticalLine_ABC& line, const geometry::Point2f& point ) const;
    void Select( const TacticalLine_ABC& line );
    void ContextMenu( const TacticalLine_ABC& line, const QPoint& point );

    virtual void Handle( kernel::Location_ABC& location );

    virtual void OptionChanged( const std::string& name, const kernel::OptionVariant& value );

    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& ) {};
    virtual void VisitCircle ( const geometry::Point2f& , float ) {};
    virtual void VisitPoint  ( const geometry::Point2f& ) {};

    void ReadLine( const std::string& name, xml::xistream& xis );
    void ReadPoint( xml::xistream& xis, T_PointVector& points );
    //@}

    //! @name Types
    //@{
    typedef std::vector< const TacticalLine_ABC* > T_Lines;
    typedef T_Lines::iterator                     IT_Lines;
    typedef T_Lines::const_iterator              CIT_Lines;
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    const kernel::GlTools_ABC& tools_;
    gui::ColorStrategy_ABC& strategy_;
    gui::ParametersLayer& parameters_;
    LimitsModel& model_;

    kernel::TristateOption drawLines_;
    T_Lines lines_;
    unsigned selected_;
    int type_;
    //@}
};

#endif // __LimitsLayer_h_
