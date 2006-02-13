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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamPoint.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 8 $
// $Workfile: ParamPoint.h $
//
// *****************************************************************************

#ifndef __ParamPoint_h_
#define __ParamPoint_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"


// =============================================================================
/** @class  ParamPoint
    @brief  ParamPoint
    @par    Using example
    @code
    ParamPoint;
    @endcode
*/
// Created: APE 2004-03-18
// =============================================================================
class ParamPoint : public QHBox, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamPoint );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamPoint( ASN1T_Point& asnPoint, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
    ~ParamPoint();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
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

#   include "ParamPoint.inl"

#endif // __ParamPoint_h_
