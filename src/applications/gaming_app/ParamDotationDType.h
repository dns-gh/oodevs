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

#include "gaming/ASN_Types.h"
#include "ParamComboBox.h"
#include "clients_kernel/Resolver.h"

namespace kernel
{
    class DotationType;
}

// =============================================================================
/** @class  ParamDotationDType
    @brief  ParamDotationDType
*/
// Created: SBO 2006-08-09
// =============================================================================
class ParamDotationDType : public ParamComboBox< ASN1T_TypeDotationTrancheD >
{

public:
    //! @name Constructors/Destructor
    //@{
             ParamDotationDType( QWidget* pParent, ASN1T_TypeDotationTrancheD& asnListEquipment, const QString& label, const kernel::Resolver_ABC< kernel::DotationType >& resolver );
    virtual ~ParamDotationDType();
    //@}
};

#endif // __ParamDotationDType_h_
