// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2005 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2005-03-14 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamNumericField.h $
// $Author: Ape $
// $Modtime: 14/03/05 14:56 $
// $Revision: 1 $
// $Workfile: ParamNumericField.h $
//
// *****************************************************************************

#ifndef __ParamNumericField_h_
#define __ParamNumericField_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"


// =============================================================================
/** @class  ParamNumericField
    @brief  ParamNumericField
*/
// Created: APE 2005-03-14
// =============================================================================
class ParamNumericField 
: public QHBox
, public Param_ABC
{
    MT_COPYNOTALLOWED( ParamNumericField );

public:
    //! @name Constructors/Destructor
    //@{
    ParamNumericField( ASN1INT& asnInt,     int nMin,   int nMax,   int nDefault, const std::string& strLabel, QWidget* pParent, bool bOptional );
    ParamNumericField( ASN1REAL& asnReal, float rMin, float rMax, float rDefault, const std::string& strLabel, QWidget* pParent, bool bOptional );
    ~ParamNumericField();
    //@}

    //! @name Operations
    //@{
    void WriteMsg( std::stringstream& strMsg );
    //@}

private:
    //! @name Member data
    //@{
    ASN1INT*  pAsnInt_;
    ASN1REAL* pAsnReal_;

    QLabel* pLabel_;
    QLineEdit* pEdit_;
    //@}
};

#endif // __ParamNumericField_h_
