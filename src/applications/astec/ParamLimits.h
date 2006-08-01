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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamLimits.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 6 $
// $Workfile: ParamLimits.h $
//
// *****************************************************************************

#ifndef __ParamLimits_h_
#define __ParamLimits_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class Limit;
class TacticalLine_ABC;


// =============================================================================
/** @class  ParamLimits
    @brief  ParamLimits
    @par    Using example
    @code
    ParamLimits;
    @endcode
*/
// Created: APE 2004-04-16
// =============================================================================
class ParamLimits : public QHBox, public Param_ABC 
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamLimits );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamLimits( ASN1T_OID& asnLimit1ID, ASN1T_OID& asnLimit2ID, const std::string strLabel, const std::string strMenuText1, const std::string strMenuText2, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamLimits();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    bool CheckValidity();
    bool CheckValidityWhenRequired();
    void WriteMsg( std::stringstream& strMsg );
    
    void UpdateLabels();
    //@}

private slots:
    //! @name Operations
    //@{
    void AcceptPopupMenuLimit( int n );
    void OnTacticalLineDeleted( TacticalLine_ABC& line );
    void ClearSelectedLimits();
    //@}

private:
    //! @name Member data
    //@{
    std::string strMenuText1_;
    std::string strMenuText2_;
    ASN1T_OID&  asnLimit1ID_;
    ASN1T_OID&  asnLimit2ID_;

    TacticalLine_ABC* pLimit1_;
    TacticalLine_ABC* pLimit2_;

    MT_ParameterLabel* pLabel_;
    QLabel* pLimit1Label_;
    QLabel* pLimit2Label_;

    TacticalLine_ABC* pPopupMenuLimit_;
    //@}
};

#   include "ParamLimits.inl"

#endif // __ParamLimits_h_
