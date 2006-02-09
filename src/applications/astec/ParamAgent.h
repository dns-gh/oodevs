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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamAgent.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 5 $
// $Workfile: ParamAgent.h $
//
// *****************************************************************************

#ifndef __ParamAgent_h_
#define __ParamAgent_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class Agent;


// =============================================================================
/** @class  ParamAgent
    @brief  ParamAgent
    @par    Using example
    @code
    ParamAgent;
    @endcode
*/
// Created: APE 2004-03-24
// =============================================================================
class ParamAgent : public QHBox, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamAgent );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamAgent( ASN1T_Agent& asnAgent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
    ~ParamAgent();
    //@}

    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    
	void Clear();
	Agent* GetAgent();
	//@}

private slots:
    //! @name Private slots
    //@{
    void AcceptPopupMenuAgent();
    //@}

private:
    std::string strMenuText_;
    ASN1T_Agent& asnAgent_;
    Agent* pAgent_;

    MT_ParameterLabel* pLabel_;
    QLabel* pAgentLabel_;

    Agent* pPopupAgent_;
};

#   include "ParamAgent.inl"

#endif // __ParamAgent_h_
