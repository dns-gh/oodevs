// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-11 $
// $Archive: /MVW_v10/Build/SDK/Light2/src/ParamPathList.h $
// $Author: Ape $
// $Modtime: 1/09/04 15:36 $
// $Revision: 5 $
// $Workfile: ParamPathList.h $
//
// *****************************************************************************

#ifndef __ParamPathList_h_
#define __ParamPathList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "ASN_Types.h"
#include "Param_ABC.h"
#include "ParamListView.h"

class ShapeEditorMapEventFilter;


// =============================================================================
/** @class  ParamPathList
    @brief  ParamPathList
    @par    Using example
    @code
    ParamPathList;
    @endcode
*/
// Created: APE 2004-05-11
// =============================================================================
class ParamPathList : public ParamListView, public Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ParamPathList );
    friend class GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    ParamPathList( ASN1T_ListItineraire& asnPathList, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent, OptionalParamFunctor_ABC* pOptional );
    ~ParamPathList();
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
    typedef std::vector< ASN1T_CoordUTM* > T_ASNUMTCoordPrtVector;
    typedef T_ASNUMTCoordPrtVector::iterator IT_ASNUMTCoordPrtVector;

private:
    //! @name Member data
    //@{
    std::string strMenuText_;
    ASN1T_ListItineraire& asnPathList_;
    ASN1T_Itineraire* pLocalisations_;
    T_ASNUMTCoordPrtVector asnUMTCoordPtrList_;
    int nMinItems_;
    int nMaxItems_;

    ShapeEditorMapEventFilter* pLineEditor_;
    //@}
};

#   include "ParamPathList.inl"

#endif // __ParamPathList_h_
