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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamSlider.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 2 $
// $Workfile: ParamSlider.h $
//
// *****************************************************************************

#ifndef __ParamSlider_h_
#define __ParamSlider_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class QSlider;


// =============================================================================
/** @class  ParamSlider
    @brief  ParamSlider
    @par    Using example
    @code
    ParamSlider;
    @endcode
*/
// Created: APE 2004-04-21
// =============================================================================
class ParamSlider : public QHBox, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamSlider );

public:
    //! @name Constructors/Destructor
    //@{
    ParamSlider( ASN1INT& asnInt,     int nMin,   int nMax,   int nDefault, const std::string& strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ParamSlider( ASN1REAL& asnReal, float rMin, float rMax, float rDefault, const std::string& strLabel, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamSlider();
    //@}

    //! @name Operations
    //@{
    void WriteMsg( std::stringstream& strMsg );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnValueChanged( int nValue );
    //@}

private:
    //! @name Member data
    //@{
    ASN1INT*  pAsnInt_;
    ASN1REAL* pAsnReal_;
    
    QLabel* pLabel_;
    QLabel* pValueLabel_;
    QSlider* pSlider_;
    //@}
};

#endif // __ParamSlider_h_
