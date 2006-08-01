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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgentList.h $
// $Author: Ape $
// $Modtime: 21/07/04 11:34 $
// $Revision: 4 $
// $Workfile: ParamAgentList.h $
//
// *****************************************************************************

#ifndef __ParamAgentList_h_
#define __ParamAgentList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"
#include "ParamListView.h"

class Agent;
class QPopupMenu;

// =============================================================================
/** @class  ParamAgentList
    @brief  ParamAgentList
    @par    Using example
    @code
    ParamAgentList;
    @endcode
*/
// Created: APE 2004-03-24
// =============================================================================
class ParamAgentList : public ParamListView, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamAgentList );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamAgentList( ASN1T_ListAgent& asnListAgent, const std::string strLabel, const std::string strMenuText, int nMinAgents, int nMaxAgents, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamAgentList();
    //@}

    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
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

    Agent* pPopupAgent_;
};

#endif // __ParamAgentList_h_
