// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamAgentType.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 2 $
// $Workfile: MOS_ParamAgentType.h $
//
// *****************************************************************************

#ifndef __MOS_ParamAgentType_h_
#define __MOS_ParamAgentType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"


// =============================================================================
/** @class  MOS_ParamAgentType
    @brief  MOS_ParamAgentType
    @par    Using example
    @code
    MOS_ParamBool;
    @endcode
*/
// Created: APE 2004-04-21
// =============================================================================
class MOS_ParamAgentType : public QButtonGroup, public MOS_Param_ABC
{
    MT_COPYNOTALLOWED( MOS_ParamAgentType );

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamAgentType( ASN1T_NatureAtlas& asnNatureAtlas, const std::string& strLabel, QWidget* pParent );
    ~MOS_ParamAgentType();
    //@}

    //! @name Operations
    //@{
    void WriteMsg( std::stringstream& strMsg );
    //@}

private:
    //! @name Member data
    //@{
    ASN1T_NatureAtlas& asnNatureAtlas_;

    QCheckBox* pArmoredCheckBox_;
    QCheckBox* pASSCheckBox_;
    QCheckBox* pHQCheckBox_;
    QCheckBox* pLogCheckBox_;
    QCheckBox* pMortarCheckBox_;
    QCheckBox* pRocketLauncherCheckBox_;
    QCheckBox* pVehiculeCheckBox_;
    QCheckBox* pFootTroopsCheckBox_;
    QCheckBox* pHelicopterCheckBox_;
    QCheckBox* pUndefinedCheckBox_;
    QCheckBox* pNoneCheckBox_;
    //@}
};

#   include "MOS_ParamAgentType.inl"

#endif // __MOS_ParamAgentType_h_
