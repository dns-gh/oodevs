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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamNumericField.h $
// $Author: Ape $
// $Modtime: 14/03/05 14:56 $
// $Revision: 1 $
// $Workfile: MOS_ParamNumericField.h $
//
// *****************************************************************************

#ifndef __MOS_ParamNumericField_h_
#define __MOS_ParamNumericField_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"


// =============================================================================
/** @class  MOS_ParamNumericField
    @brief  MOS_ParamNumericField
*/
// Created: APE 2005-03-14
// =============================================================================
class MOS_ParamNumericField 
: public QHBox
, public MOS_Param_ABC
{
    MT_COPYNOTALLOWED( MOS_ParamNumericField );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamNumericField( ASN1INT& asnInt,     int nMin,   int nMax,   int nDefault, const std::string& strLabel, QWidget* pParent );
    MOS_ParamNumericField( ASN1REAL& asnReal, float rMin, float rMax, float rDefault, const std::string& strLabel, QWidget* pParent );
    ~MOS_ParamNumericField();
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

#endif // __MOS_ParamNumericField_h_
