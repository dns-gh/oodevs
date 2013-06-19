// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamTime_h_
#define __ParamTime_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

class QTimeEdit;

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamTime
    @brief  ParamTime
*/
// Created: MMC 2012-05-15
// =============================================================================
class ParamTime : public Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamTime( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamTime();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual bool InternalCheckValidity() const;
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::Quantity& param );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChanged( const QTime& );
    //@}

private:
    //! @name Member data
    //@{
    QTimeEdit* timeEdit_;
    QTime time_;
    int days_;
    //@}
};

    }
}

#endif // __ParamTime_h_
