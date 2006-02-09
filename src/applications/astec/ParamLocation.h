// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-06 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamLocation.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 8 $
// $Workfile: ParamLocation.h $
//
// *****************************************************************************

#ifndef __ParamLocation_h_
#define __ParamLocation_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"

class ShapeEditorMapEventFilter;


// =============================================================================
/** @class  ParamLocation
    @brief  ParamLocation
    @par    Using example
    @code
    ParamLocation;
    @endcode
*/
// Created: APE 2004-05-06
// =============================================================================
class ParamLocation : public QHBox, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamLocation );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     ParamLocation( ASN1T_Localisation& asnListPoint, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional, bool bOutsideData = false );
    virtual ~ParamLocation();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );

    void Clear();
    //@}

    const T_PointVector& GetPointList();
    ASN1T_EnumTypeLocalisation GetType();

private slots:
    void StartTracing( int nLocationType );
    void TracingDone();

private:
    //! @name Member data
    //@{
    std::string strMenuText_;

    MT_ParameterLabel* pLabel_;
    QLabel* pShapeLabel_;
    QPopupMenu* pPopupMenu_;
    MT_Vector2D popupPoint_;

    ASN1T_Localisation& asnListPoint_;
    ASN1T_CoordUTM* pUMTCoords_;
    bool bOutsideData_;

    T_PointVector pointList_;
    ASN1T_EnumTypeLocalisation nType_;

    ShapeEditorMapEventFilter* pLineEditor_;
    //@}
};

#   include "ParamLocation.inl"

#endif // __ParamLocation_h_
