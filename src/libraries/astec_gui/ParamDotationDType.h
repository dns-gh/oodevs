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

#include "astec_gaming/ASN_Types.h"
#include "ParamComboBox.h"
#include "astec_kernel/Resolver.h"

class DotationType;

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
             ParamDotationDType( QWidget* pParent, ASN1T_TypeDotationTrancheD& asnListEquipment, const std::string& label, const Resolver< DotationType >& resolver );
    virtual ~ParamDotationDType();
    //@}
};

#endif // __ParamDotationDType_h_
