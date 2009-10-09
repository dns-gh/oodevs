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
    class Parameter_ABC;
}

namespace gui
{
    class RichLabel;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  LimitParameter
    @brief  LimitParameter
*/
// Created: SBO 2006-11-14
// =============================================================================
class LimitParameter : public QObject
                     , public Param_ABC
                     , public kernel::ContextMenuObserver_ABC< kernel::TacticalLine_ABC >
                     , public tools::ElementObserver_ABC< kernel::TacticalLine_ABC >

{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             LimitParameter( QObject* parent, const kernel::OrderParameter& parameter, const kernel::CoordinateConverter_ABC& converter, kernel::Controller& controller );
    virtual ~LimitParameter();
    //@}

    //! @name Operations
    //@{
    virtual bool CheckValidity();
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    virtual void MenuItemValidated();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    LimitParameter( const LimitParameter& );            //!< Copy constructor
    LimitParameter& operator=( const LimitParameter& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    void Display( const QString& what );
    virtual void NotifyContextMenu( const kernel::TacticalLine_ABC& entity, kernel::ContextMenu& menu );
    virtual void NotifyDeleted( const kernel::TacticalLine_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::OrderParameter& parameter_;
    const kernel::CoordinateConverter_ABC& converter_;
    ::gui::RichLabel* pLabel_;
    QLabel*         entityLabel_; // $$$$ AGE 2006-03-14: LabelDisplayer ?
    const kernel::TacticalLine_ABC* potential_;
    const kernel::TacticalLine_ABC* selected_;
    //@}
};

    }
}

#endif // __LimitParameter_h_
