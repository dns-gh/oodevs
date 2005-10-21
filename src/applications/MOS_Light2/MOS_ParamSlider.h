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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamSlider.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 2 $
// $Workfile: MOS_ParamSlider.h $
//
// *****************************************************************************

#ifndef __MOS_ParamSlider_h_
#define __MOS_ParamSlider_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class QSlider;


// =============================================================================
/** @class  MOS_ParamSlider
    @brief  MOS_ParamSlider
    @par    Using example
    @code
    MOS_ParamSlider;
    @endcode
*/
// Created: APE 2004-04-21
// =============================================================================
class MOS_ParamSlider : public QHBox, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamSlider );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamSlider( ASN1INT& asnInt,     int nMin,   int nMax,   int nDefault, const std::string& strLabel, QWidget* pParent );
    MOS_ParamSlider( ASN1REAL& asnReal, float rMin, float rMax, float rDefault, const std::string& strLabel, QWidget* pParent );
    ~MOS_ParamSlider();
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

#endif // __MOS_ParamSlider_h_
