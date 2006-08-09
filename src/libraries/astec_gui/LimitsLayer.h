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

#include "Layer_ABC.h"
#include "astec_kernel/Observer_ABC.h"
#include "astec_kernel/ElementObserver_ABC.h"
#include "astec_kernel/ContextMenuObserver_ABC.h"
#include "ShapeHandler_ABC.h"
#include "astec_kernel/OptionsObserver_ABC.h"
#include "astec_kernel/TristateOption.h"
#include "astec_kernel/LocationVisitor_ABC.h"

class Controllers;
class Lima;
class Limit;
class TacticalLine_ABC;
class GlTools_ABC;
class ColorStrategy_ABC;
class ParametersLayer;
class LimitsModel;

// =============================================================================
/** @class  LimitsLayer
    @brief  LimitsLayer
*/
// Created: AGE 2006-03-24
// =============================================================================
class LimitsLayer : public QObject
                  , public Layer_ABC
                  , public Observer_ABC
                  , public ElementObserver_ABC< Lima >
                  , public ElementObserver_ABC< Limit >
                  , public ContextMenuObserver_ABC< geometry::Point2f >
                  , public OptionsObserver_ABC
                  , private ShapeHandler_ABC
                  , private LocationVisitor_ABC
                  
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LimitsLayer( Controllers& controllers, const GlTools_ABC& tools, ColorStrategy_ABC& strategy, ParametersLayer& parameters, LimitsModel& model );
    virtual ~LimitsLayer();
    //@}

    //! @name Operations
    //@{
    virtual void Paint( const geometry::Rectangle2f& viewport );
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

    virtual bool HandleKeyPress        ( QKeyEvent* key );
    virtual bool HandleMousePress      ( QMouseEvent* mouse, const geometry::Point2f& point );

    virtual void NotifyContextMenu( const geometry::Point2f&, ContextMenu& menu );

    virtual bool IsInSelection( const TacticalLine_ABC& line, const geometry::Point2f& point ) const;
    void Select( const TacticalLine_ABC& line );
    void ContextMenu( const TacticalLine_ABC& line, const QPoint& point );

    virtual void Handle( Location_ABC& location );

    virtual void OptionChanged( const std::string& name, const OptionVariant& value );

    virtual void VisitLines  ( const T_PointVector& points );
    virtual void VisitPolygon( const T_PointVector& ) {};
    virtual void VisitCircle ( const geometry::Point2f& , float ) {};
    virtual void VisitPoint  ( const geometry::Point2f& ) {};
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
    Controllers& controllers_;
    const GlTools_ABC& tools_;
    ColorStrategy_ABC& strategy_;
    ParametersLayer& parameters_;
    LimitsModel& model_;

    TristateOption drawLines_;
    T_Lines lines_;
    unsigned selected_;
    int type_;
    //@}
};

#endif // __LimitsLayer_h_
