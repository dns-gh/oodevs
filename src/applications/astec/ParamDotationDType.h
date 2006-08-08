// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-21 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamDotationDType.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 3 $
// $Workfile: ParamDotationDType.h $
//
// *****************************************************************************

#ifndef __ParamDotationDType_h_
#define __ParamDotationDType_h_

#include "Param_ABC.h"
#include "ASN_Types.h"

// =============================================================================
// Created: APE 2004-04-21
// =============================================================================
class ParamDotationDType : public QHBox, public Param_ABC
{
    MT_COPYNOTALLOWED( ParamDotationDType );

public:
    //! @name Constructors/Destructor
    //@{
     ParamDotationDType( ASN1T_OID& asn, const std::string strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamDotationDType();
    //@}

    //! @name Operations
    //@{
    void WriteMsg( std::stringstream& strMsg );
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_OID& asn_;
    QLabel* pLabel_;
    MT_ValuedComboBox<ASN1T_OID>* pComboBox_;
    //@}
};

#endif // __ParamDotationDType_h_