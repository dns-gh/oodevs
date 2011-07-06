// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamStringField_h_
#define __ParamStringField_h_

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
/** @class  ParamStringField
    @brief  ParamStringField
*/
// Created: BCI 2011-03-14
// =============================================================================
class ParamStringField : public Param_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ParamStringField( const kernel::OrderParameter& parameter );
    virtual ~ParamStringField();
    //@}

    //! @name Operations
    //@{
    virtual QWidget* BuildInterface( QWidget* parent );

    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    //@}

private:
    //! @name Member data
    //@{
    kernel::OrderParameter parameter_;
    ::gui::RichLabel* pLabel_;
    QLineEdit* pEdit_;
    //@}
};

}
}

#endif // __ParamStringField_h_
