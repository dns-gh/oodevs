// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ParamDotationDType_h_
#define __ParamDotationDType_h_

#include "ParamComboBox.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class DotationType;
    class OrderParameter;
}

namespace actions
{
    namespace gui
    {

// =============================================================================
/** @class  ParamDotationDType
    @brief  ParamDotationDType
    // $$$$ SBO 2007-03-21: ParamComboBox< const DotationType* > ?
    // $$$$ AGE 2007-10-23: pas utilisé
*/
// Created: SBO 2006-08-09
// =============================================================================
class ParamDotationDType : public ParamComboBox< ASN1T_DotationType >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamDotationDType( const kernel::OrderParameter& parameter, const kernel::Resolver_ABC< kernel::DotationType >& resolver );
    virtual ~ParamDotationDType();
    //@}

    //! @name Operations
    //@{
    virtual void BuildInterface( QWidget* parent );
    virtual void CommitTo( actions::ParameterContainer_ABC& action ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamDotationDType( const ParamDotationDType& );
    ParamDotationDType& operator=( const ParamDotationDType& );
    //@}

    //! @name Helpers
    //@{
    virtual bool IsOptional() const;
    //@}

private:
    //! @name Member data
    //@{
    const kernel::OrderParameter& parameter_;
    const kernel::Resolver_ABC< kernel::DotationType >& resolver_;
    //@}
};

    }
}

#endif // __ParamDotationDType_h_
