// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamDateTime_h_
#define __ParamDateTime_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamDateTime
    @brief  ParamDateTime
*/
// Created: SBO 2007-05-14
// =============================================================================
class ParamDateTime : public Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamDateTime( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamDateTime();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual bool InternalCheckValidity() const;
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Draw( const geometry::Point2f& point, const ::gui::Viewport_ABC& viewport, ::gui::GLView_ABC& tools ) const;
    virtual void Visit( const actions::parameters::DateTime& param );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChanged( const QDateTime& );
    //@}

private:
    //! @name Member data
    //@{
    QDateTimeEdit* dateTimeEdit_;
    QDateTime date_;
    //@}
};

    }
}

#endif // __ParamDateTime_h_
