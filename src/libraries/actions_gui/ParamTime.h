// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ParamTime_h_
#define __ParamTime_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

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
    virtual void Show() { if( group_ ) group_->show(); }
    virtual void Hide() { if( group_ ) group_->hide(); }
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual bool InternalCheckValidity() const;
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChanged( const QTime& );
    //@}

private:
    //! @name Member data
    //@{
    QTime time_;
    int days_;
    //@}
};

    }
}

#endif // __ParamTime_h_
