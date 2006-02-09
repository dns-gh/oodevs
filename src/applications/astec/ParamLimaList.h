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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamLimaList.h $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 4 $
// $Workfile: ParamLimaList.h $
//
// *****************************************************************************

#ifndef __ParamLimaList_h_
#define __ParamLimaList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "ParamListView.h"
#include "Param_ABC.h"

class TacticalLine_ABC;


// =============================================================================
/** @class  ParamLimaList
    @brief  ParamLimaList
    @par    Using example
    @code
    ParamLimaList;
    @endcode
*/
// Created: APE 2004-04-19
// =============================================================================
class ParamLimaList : public ParamListView, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamLimaList );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamLimaList( ASN1T_ListOID& asnListLima, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
    ~ParamLimaList();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    //@}

private slots:
    //! @name Private slots
    //@{
    void AcceptPopupMenuLima();
    void OnTacticalLineDeleted( TacticalLine_ABC& line );
    //@}

private:
    //! @name Member data
    //@{
    std::string strMenuText_;
    ASN1T_ListOID& asnListLima_;
    ASN1T_OID* pAsnOIDList_;

    TacticalLine_ABC* pPopupLima_;
    //@}
};

#   include "ParamLimaList.inl"

#endif // __ParamLimaList_h_
