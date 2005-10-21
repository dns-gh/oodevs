// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamAgentList.h $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 4 $
// $Workfile: MOS_ParamAgentList.h $
//
// *****************************************************************************

#ifndef __MOS_ParamAgentList_h_
#define __MOS_ParamAgentList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"
#include "MOS_ParamListView.h"

class MOS_Agent;
class QPopupMenu;

// =============================================================================
/** @class  MOS_ParamAgentList
    @brief  MOS_ParamAgentList
    @par    Using example
    @code
    MOS_ParamAgentList;
    @endcode
*/
// Created: APE 2004-03-24
// =============================================================================
class MOS_ParamAgentList : public MOS_ParamListView, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamAgentList );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamAgentList( ASN1T_ListAgent& asnListAgent, const std::string strLabel, const std::string strMenuText, int nMinAgents, int nMaxAgents, QWidget* pParent );
    ~MOS_ParamAgentList();
    //@}

    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    //@}

private slots:
    //! @name Private slots
    //@{
    void AcceptPopupMenuAgent();
    //@}

private:
    std::string strMenuText_;
    ASN1T_ListAgent& asnListAgent_;
    ASN1T_OID* pAsnOIDList_;
    int nMinAgents_;
    int nMaxAgents_;

    MOS_Agent* pPopupAgent_;
};

#endif // __MOS_ParamAgentList_h_
