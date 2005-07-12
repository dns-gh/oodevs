// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-04-19 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamLimaList.h $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 4 $
// $Workfile: MOS_ParamLimaList.h $
//
// *****************************************************************************

#ifndef __MOS_ParamLimaList_h_
#define __MOS_ParamLimaList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_ParamListView.h"
#include "MOS_Param_ABC.h"

class MOS_TacticalLine_ABC;


// =============================================================================
/** @class  MOS_ParamLimaList
    @brief  MOS_ParamLimaList
    @par    Using example
    @code
    MOS_ParamLimaList;
    @endcode
*/
// Created: APE 2004-04-19
// =============================================================================
class MOS_ParamLimaList : public MOS_ParamListView, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamLimaList );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamLimaList( ASN1T_ListOID& asnListLima, const std::string strLabel, const std::string strMenuText, QWidget* pParent );
    ~MOS_ParamLimaList();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    //@}

private slots:
    //! @name Private slots
    //@{
    void AcceptPopupMenuLima();
    void OnTacticalLineDeleted( MOS_TacticalLine_ABC& line );
    //@}

private:
    //! @name Member data
    //@{
    std::string strMenuText_;
    ASN1T_ListOID& asnListLima_;
    ASN1T_OID* pAsnOIDList_;

    MOS_TacticalLine_ABC* pPopupLima_;
    //@}
};


#ifdef MOS_USE_INLINE
#   include "MOS_ParamLimaList.inl"
#endif

#endif // __MOS_ParamLimaList_h_
