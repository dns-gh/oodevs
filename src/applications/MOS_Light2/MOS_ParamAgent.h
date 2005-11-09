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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamAgent.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:43 $
// $Revision: 5 $
// $Workfile: MOS_ParamAgent.h $
//
// *****************************************************************************

#ifndef __MOS_ParamAgent_h_
#define __MOS_ParamAgent_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class MOS_Agent;


// =============================================================================
/** @class  MOS_ParamAgent
    @brief  MOS_ParamAgent
    @par    Using example
    @code
    MOS_ParamAgent;
    @endcode
*/
// Created: APE 2004-03-24
// =============================================================================
class MOS_ParamAgent : public QHBox, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamAgent );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamAgent( ASN1T_Agent& asnAgent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
    ~MOS_ParamAgent();
    //@}

    //! @name Operations
    //@{
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    
	void Clear();
	MOS_Agent* GetAgent();
	//@}

private slots:
    //! @name Private slots
    //@{
    void AcceptPopupMenuAgent();
    //@}

private:
    std::string strMenuText_;
    ASN1T_Agent& asnAgent_;
    MOS_Agent* pAgent_;

    MT_ParameterLabel* pLabel_;
    QLabel* pAgentLabel_;

    MOS_Agent* pPopupAgent_;
};

#   include "MOS_ParamAgent.inl"

#endif // __MOS_ParamAgent_h_
