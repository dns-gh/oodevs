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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamPath.h $
// $Author: Ape $
// $Modtime: 1/09/04 15:36 $
// $Revision: 7 $
// $Workfile: MOS_ParamPath.h $
//
// *****************************************************************************

#ifndef __MOS_ParamPath_h_
#define __MOS_ParamPath_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"
#include "MOS_ParamListView.h"

class MOS_Agent;
class MOS_ShapeEditorMapEventFilter;
class QPopupMenu;


// =============================================================================
/** @class  MOS_ParamPath
    @brief  MOS_ParamPath
    @par    Using example
    @code
    MOS_ParamPath;
    @endcode
*/
// Created: APE 2004-03-25
// =============================================================================
class MOS_ParamPath : public MOS_ParamListView, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamPath );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamPath( ASN1T_Itineraire& asnListPoint, MOS_Agent& agent, const std::string strLabel, const std::string strMenuText, QWidget* pParent);
    ~MOS_ParamPath();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
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
    MOS_Agent& agent_;

    T_PointVector pointList_;

    MOS_ShapeEditorMapEventFilter* pLineEditor_;
};

#   include "MOS_ParamPath.inl"

#endif // __MOS_ParamPath_h_
