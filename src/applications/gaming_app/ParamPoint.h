// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamPoint_h_
#define __ParamPoint_h_

#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"

namespace kernel
{
    class CoordinateConverter_ABC;
    class OrderParameter;
}

namespace gui
{
    class RichLabel;
}

// =============================================================================
/** @class  ParamPoint
    @brief  ParamPoint
*/
// Created: APE 2004-03-18
// =============================================================================
class ParamPoint : public QObject, public Param_ABC
                 , public kernel::ContextMenuObserver_ABC< geometry::Point2f >
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamPoint( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter );
    virtual ~ParamPoint();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& extent, const kernel::GlTools_ABC& tools ) const;
    virtual bool CheckValidity();
    virtual void CommitTo( ActionParameterContainer_ABC& action ) const;
    virtual void NotifyContextMenu( const geometry::Point2f&, kernel::ContextMenu& );
    virtual void BuildInterface( QWidget* parent );
    //@}

private slots:
    //! @name Operations
    //@{
    void AcceptPopupMenuPoint();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ParamPoint( const ParamPoint& );
    ParamPoint& operator=( const ParamPoint& );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    const kernel::CoordinateConverter_ABC& converter_;
    gui::RichLabel*   pLabel_;
    QLabel*           pPosLabel_;
    geometry::Point2f popupPoint_;
    geometry::Point2f paramPoint_;
    bool              isSet_;
    //@}
};

#endif // __ParamPoint_h_
