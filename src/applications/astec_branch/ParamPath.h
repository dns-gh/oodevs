// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-25 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamPath.h $
// $Author: Ape $
// $Modtime: 1/09/04 15:36 $
// $Revision: 7 $
// $Workfile: ParamPath.h $
//
// *****************************************************************************

#ifndef __ParamPath_h_
#define __ParamPath_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"
#include "ParamListView.h"

class Agent_ABC;
class ShapeEditorMapEventFilter;
class QPopupMenu;


// =============================================================================
/** @class  ParamPath
    @brief  ParamPath
    @par    Using example
    @code
    ParamPath;
    @endcode
*/
// Created: APE 2004-03-25
// =============================================================================
class ParamPath : public ParamListView, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamPath );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamPath( ASN1T_Itineraire& asnListPoint, Agent_ABC& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
    ~ParamPath();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    //@}

private slots:
    void StartPath();
    void PathDone();

private:
    std::string strMenuText_;
    ASN1T_Itineraire& asnListPoint_;
    ASN1T_CoordUTM* pUMTCoords_;
    Agent_ABC& agent_;

    T_PointVector pointList_;

    ShapeEditorMapEventFilter* pLineEditor_;
};

#   include "ParamPath.inl"

#endif // __ParamPath_h_
