// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-03-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamPoint.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 8 $
// $Workfile: MOS_ParamPoint.h $
//
// *****************************************************************************

#ifndef __MOS_ParamPoint_h_
#define __MOS_ParamPoint_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"


// =============================================================================
/** @class  MOS_ParamPoint
    @brief  MOS_ParamPoint
    @par    Using example
    @code
    MOS_ParamPoint;
    @endcode
*/
// Created: APE 2004-03-18
// =============================================================================
class MOS_ParamPoint : public QHBox, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamPoint );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamPoint( ASN1T_Point& asnPoint, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
    ~MOS_ParamPoint();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );

    const MT_Vector2D& GetPoint();
    void Clear();
    //@}


private slots:
    //! @name Operations
    //@{
    void AcceptPopupMenuPoint();
    //@}

private:
    //! @name Helpers
    //@{
    //@}

private:
    std::string     strMenuText_;
    ASN1T_Point&    asnPoint_;
    ASN1T_CoordUTM  asnPos_;
    MT_Vector2D     point_;

    MT_ParameterLabel* pLabel_;
    QLabel* pPosLabel_;

    MT_Vector2D popupMenuPoint_;
};

#   include "MOS_ParamPoint.inl"

#endif // __MOS_ParamPoint_h_
