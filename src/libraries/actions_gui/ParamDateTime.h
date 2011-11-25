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

// =============================================================================
/** @class  ParamDateTime
    @brief  ParamDateTime
*/
// Created: SBO 2007-05-14
// =============================================================================
class ParamDateTime : public QObject
                    , public Param_ABC
{
    Q_OBJECT;

public:
    //! @name Constructors/Destructor
    //@{
             ParamDateTime( QObject* parent, const kernel::OrderParameter& parameter, const QDateTime& current );
             ParamDateTime( QObject* parent, const QString& name, const QDateTime& current, bool optional );
    virtual ~ParamDateTime();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual bool InternalCheckValidity() const;
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Draw( const geometry::Point2f& point, const kernel::Viewport_ABC& viewport, const kernel::GlTools_ABC& tools ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChanged( const QDateTime& );
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    QDateTime date_;
    //@}
};

    }
}

#endif // __ParamDateTime_h_
