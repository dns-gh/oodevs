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

namespace gui
{
    class RichLabel;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamNumericField
    @brief  ParamNumericField
*/
// Created: AGE 2006-03-15
// =============================================================================
class ParamNumericField : public Param_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ParamNumericField( const kernel::OrderParameter& parameter, bool isReal );
    virtual ~ParamNumericField();
    //@}

    //! @name Operations
    //@{
    virtual void Show();
    virtual void Hide();
    virtual QWidget* BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    void SetLimits( float min, float max );
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool InternalCheckValidity() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    bool isReal_;
    QLineEdit* pEdit_;
    //@}
};

    }
}

#endif // __ParamNumericField_h_
