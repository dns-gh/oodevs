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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamLocationList.h $
// $Author: Ape $
// $Modtime: 7/09/04 17:16 $
// $Revision: 4 $
// $Workfile: ParamLocationList.h $
//
// *****************************************************************************

#ifndef __ParamLocationList_h_
#define __ParamLocationList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"
#include "ParamListView.h"

class ShapeEditorMapEventFilter;


// =============================================================================
/** @class  ParamLocationList
    @brief  ParamLocationList
    @par    Using example
    @code
    ParamLocationList;
    @endcode
*/
// Created: APE 2004-05-07
// =============================================================================
class ParamLocationList : public ParamListView, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamLocationList );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     ParamLocationList( ASN1T_ListLocalisation& asnListLoc,   const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
     ParamLocationList( ASN1T_ListPolygon&      asnListPoly,  const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
     ParamLocationList( ASN1T_ListPoint&        asnListPoint, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOptional );
    ~ParamLocationList();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const ActionContext& context );
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

    ShapeEditorMapEventFilter* pLineEditor_;
    //@}
};

#   include "ParamLocationList.inl"

#endif // __ParamLocationList_h_
