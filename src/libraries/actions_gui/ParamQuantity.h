// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamQuantity_h_
#define __ParamQuantity_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

class QSpinBox;

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamQuantity
    @brief  ParamQuantity
*/
// Created: MMC 2012-05-15
// =============================================================================
class ParamQuantity : public Param_ABC
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             ParamQuantity( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamQuantity();
    //@}

    //! @name Operations
    //@{
    virtual void Show() { if( group_ ) group_->show(); }
    virtual void Hide() { if( group_ ) group_->hide(); }
    virtual QWidget* BuildInterface( const QString& objectName, QWidget* parent );
    virtual bool InternalCheckValidity() const;
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    virtual void Visit( const actions::parameters::Quantity& param );
    void SetLimit( int min, int max );
    void SetSuffix( const QString& suffix );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnChanged( int );
    //@}

private:
    //! @name Member data
    //@{
    int modifier_;
    int min_;
    int max_;
    QString suffix_;
    QSpinBox* spinBox_;
    //@}
};

    }
}

#endif // __ParamQuantity_h_
