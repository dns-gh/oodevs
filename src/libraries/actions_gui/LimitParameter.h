// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __LimitParameter_h_
#define __LimitParameter_h_

#include "Param_ABC.h"
#include "clients_kernel/ContextMenuObserver_ABC.h"
#include "tools/ElementObserver_ABC.h"

namespace kernel
{
    class TacticalLine_ABC;
    class CoordinateConverter_ABC;
    class OrderParameter;
    class Controller;
}

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  LimitParameter
    @brief  LimitParameter
*/
// Created: SBO 2006-11-14
// =============================================================================
class LimitParameter : public Param_ABC
                     , public kernel::ContextMenuObserver_ABC< kernel::TacticalLine_ABC >
                     , public tools::ElementObserver_ABC< kernel::TacticalLine_ABC >

{

public:
    //! @name Constructors/Destructor
    //@{
             LimitParameter( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~LimitParameter();
    //@}

    //! @name Operations
    //@{
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Slots
    //@{
    virtual void OnMenuClick();
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
    void Display( const QString& what );
    virtual void NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::TacticalLine_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::CoordinateConverter_ABC& converter_;
    QLabel* entityLabel_;
    const kernel::TacticalLine_ABC* potential_;
    const kernel::TacticalLine_ABC* selected_;
    //@}
};

}
}

#endif // __LimitParameter_h_
