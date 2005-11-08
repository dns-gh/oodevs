// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-16 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamLimits.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 6 $
// $Workfile: MOS_ParamLimits.h $
//
// *****************************************************************************

#ifndef __MOS_ParamLimits_h_
#define __MOS_ParamLimits_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class MOS_Limit;
class MOS_TacticalLine_ABC;


// =============================================================================
/** @class  MOS_ParamLimits
    @brief  MOS_ParamLimits
    @par    Using example
    @code
    MOS_ParamLimits;
    @endcode
*/
// Created: APE 2004-04-16
// =============================================================================
class MOS_ParamLimits : public QHBox, public MOS_Param_ABC 
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamLimits );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamLimits( ASN1T_OID& asnLimit1ID, ASN1T_OID& asnLimit2ID, const std::string strLabel, const std::string strMenuText1, const std::string strMenuText2, QWidget* pParent );
    ~MOS_ParamLimits();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity();
    bool CheckValidityWhenRequired();
    void WriteMsg( std::stringstream& strMsg );
    
    void UpdateLabels();
    //@}

private slots:
    //! @name Operations
    //@{
    void AcceptPopupMenuLimit( int n );
    void OnTacticalLineDeleted( MOS_TacticalLine_ABC& line );
    void ClearSelectedLimits();
    //@}

private:
    //! @name Member data
    //@{
    std::string strMenuText1_;
    std::string strMenuText2_;
    ASN1T_OID&  asnLimit1ID_;
    ASN1T_OID&  asnLimit2ID_;

    MOS_TacticalLine_ABC* pLimit1_;
    MOS_TacticalLine_ABC* pLimit2_;

    MT_ParameterLabel* pLabel_;
    QLabel* pLimit1Label_;
    QLabel* pLimit2Label_;

    MOS_TacticalLine_ABC* pPopupMenuLimit_;
    //@}
};

#   include "MOS_ParamLimits.inl"

#endif // __MOS_ParamLimits_h_
