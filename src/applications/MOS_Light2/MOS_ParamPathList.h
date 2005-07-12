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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamPathList.h $
// $Author: Ape $
// $Modtime: 1/09/04 15:36 $
// $Revision: 5 $
// $Workfile: MOS_ParamPathList.h $
//
// *****************************************************************************

#ifndef __MOS_ParamPathList_h_
#define __MOS_ParamPathList_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"
#include "MOS_ParamListView.h"

class MOS_ShapeEditorMapEventFilter;


// =============================================================================
/** @class  MOS_ParamPathList
    @brief  MOS_ParamPathList
    @par    Using example
    @code
    MOS_ParamPathList;
    @endcode
*/
// Created: APE 2004-05-11
// =============================================================================
class MOS_ParamPathList : public MOS_ParamListView, public MOS_Param_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ParamPathList );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
    MOS_ParamPathList( ASN1T_ListItineraire& asnPathList, const std::string strLabel, const std::string strMenuText, int nMinItems, int nMaxItems, QWidget* pParent );
    ~MOS_ParamPathList();
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

    MOS_ShapeEditorMapEventFilter* pLineEditor_;
    //@}
};


#ifdef MOS_USE_INLINE
#   include "MOS_ParamPathList.inl"
#endif

#endif // __MOS_ParamPathList_h_
