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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgentType.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 2 $
// $Workfile: ParamAgentType.h $
//
// *****************************************************************************

#ifndef __ParamAgentType_h_
#define __ParamAgentType_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"


// =============================================================================
// Created: APE 2004-04-21
// =============================================================================
class ParamAgentType : public QButtonGroup, public Param_ABC
{
    MT_COPYNOTALLOWED( ParamAgentType );

public:
    //! @name Constructors/Destructor
    //@{
    ParamAgentType( ASN1T_NatureAtlas& asnNatureAtlas, const std::string& strLabel, QWidget* pParent, bool bOptional );
    ~ParamAgentType();
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

#endif // __ParamAgentType_h_
