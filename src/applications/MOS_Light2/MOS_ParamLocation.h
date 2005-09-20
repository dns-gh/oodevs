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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamLocation.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:36 $
// $Revision: 8 $
// $Workfile: MOS_ParamLocation.h $
//
// *****************************************************************************

#ifndef __MOS_ParamLocation_h_
#define __MOS_ParamLocation_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class MOS_ShapeEditorMapEventFilter;


// =============================================================================
/** @class  MOS_ParamLocation
    @brief  MOS_ParamLocation
    @par    Using example
    @code
    MOS_ParamLocation;
    @endcode
*/
// Created: APE 2004-05-06
// =============================================================================
class MOS_ParamLocation : public QHBox, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamLocation );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ParamLocation( ASN1T_Localisation& asnListPoint, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOutsideData = false );
    virtual ~MOS_ParamLocation();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
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

    MOS_ShapeEditorMapEventFilter* pLineEditor_;
    //@}
};

#   include "MOS_ParamLocation.inl"

#endif // __MOS_ParamLocation_h_
