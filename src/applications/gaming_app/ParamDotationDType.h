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

#include "game_asn/Asn.h"
#include "ParamComboBox.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class DotationType;
    class OrderParameter;
}

// =============================================================================
/** @class  ParamDotationDType
    @brief  ParamDotationDType
    // $$$$ SBO 2007-03-21: ParamComboBox< const DotationType* > ?
*/
// Created: SBO 2006-08-09
// =============================================================================
class ParamDotationDType : public ParamComboBox< ASN1T_TypeDotationTrancheD >
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
    virtual void CommitTo( ASN1T_MissionParameter& asn ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    ParamDotationDType( const ParamDotationDType& );
    ParamDotationDType& operator=( const ParamDotationDType& );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Resolver_ABC< kernel::DotationType >& resolver_;
    //@}
};

#endif // __ParamDotationDType_h_
