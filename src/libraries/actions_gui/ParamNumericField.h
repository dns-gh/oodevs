// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamNumericField_h_
#define __ParamNumericField_h_

#include "Param_ABC.h"
#include "clients_kernel/OrderParameter.h"

namespace actions
{
    namespace gui
    {
        class InterfaceBuilder_ABC;

// =============================================================================
/** @class  ParamNumericField
    @brief  ParamNumericField
*/
// Created: AGE 2006-03-15
// =============================================================================
template< typename NumericType = float >
class ParamNumericField : public Param_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamNumericField( const InterfaceBuilder_ABC& builder, const kernel::OrderParameter& parameter );
    virtual ~ParamNumericField();
    //@}

    //! @name Operations
    //@{
    virtual void Show();
    virtual void Hide();
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    void SetLimits( NumericType min, NumericType max );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    QLineEdit* pEdit_;
    //@}
};

typedef ParamNumericField< int >   ParamInt;
typedef ParamNumericField< float > ParamFloat;

    }
}

#include "ParamNumericField.inl"

#endif // __ParamNumericField_h_
