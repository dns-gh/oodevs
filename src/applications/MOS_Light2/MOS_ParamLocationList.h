// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-07 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamLocationList.h $
// $Author: Ape $
// $Modtime: 7/09/04 17:16 $
// $Revision: 4 $
// $Workfile: MOS_ParamLocationList.h $
//
// *****************************************************************************

#ifndef __MOS_ParamLocationList_h_
#define __MOS_ParamLocationList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"
#include "MOS_ParamListView.h"

class MOS_ShapeEditorMapEventFilter;


// =============================================================================
/** @class  MOS_ParamLocationList
    @brief  MOS_ParamLocationList
    @par    Using example
    @code
    MOS_ParamLocationList;
    @endcode
*/
// Created: APE 2004-05-07
// =============================================================================
class MOS_ParamLocationList : public MOS_ParamListView, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamLocationList );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ParamLocationList( ASN1T_ListLocalisation& asnListLoc,   const std::string strLabel, const std::string strMenuText, QWidget* pParent );
     MOS_ParamLocationList( ASN1T_ListPolygon&      asnListPoly,  const std::string strLabel, const std::string strMenuText, QWidget* pParent );
     MOS_ParamLocationList( ASN1T_ListPoint&        asnListPoint, const std::string strLabel, const std::string strMenuText, QWidget* pParent );
    ~MOS_ParamLocationList();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    //@}

private slots:
    void StartTracing( int nLocationType );
    void TracingDone();

private:
    typedef std::pair< ASN1T_EnumTypeLocalisation, T_PointVector > T_Location;
    typedef MT_ValuedListViewItem<T_Location> T_LocationItem;

    typedef std::vector< ASN1T_CoordUTM* > T_ASNUMTCoordPrtVector;
    typedef T_ASNUMTCoordPrtVector::iterator IT_ASNUMTCoordPrtVector;

private:
    //! @name Member data
    //@{
    std::string strMenuText_;

    ASN1T_ListLocalisation& asnListLoc_;
    ASN1T_Localisation* pAsnLocalisationList_;
    T_ASNUMTCoordPrtVector asnUMTCoordPtrList_;

    QPopupMenu* pPopupMenu_;
    MT_Vector2D popupPoint_;
    T_LocationItem* pItemUnderConstruction_;

    MOS_ShapeEditorMapEventFilter* pLineEditor_;
    //@}
};

#   include "MOS_ParamLocationList.inl"

#endif // __MOS_ParamLocationList_h_
