// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-05-18 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ParamObstacle.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:35 $
// $Revision: 4 $
// $Workfile: MOS_ParamObstacle.h $
//
// *****************************************************************************

#ifndef __MOS_ParamObstacle_h_
#define __MOS_ParamObstacle_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_ASN_Types.h"
#include "MOS_Param_ABC.h"

class MOS_ParamLocation;


// =============================================================================
/** @class  MOS_ParamObstacle
    @brief  MOS_ParamObstacle
    @par    Using example
    @code
    MOS_ParamObstacle;
    @endcode
*/
// Created: APE 2004-05-18
// =============================================================================
class MOS_ParamObstacle : public QGroupBox, public MOS_Param_ABC
{
    MT_COPYNOTALLOWED( MOS_ParamObstacle );
    friend class MOS_GLTool;

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ParamObstacle( ASN1T_MissionGenObject& asnObject, const std::string strLabel, const std::string strMenuText, QWidget* pParent, bool bOutsideData = false );
    ~MOS_ParamObstacle();
    //@}

    //! @name Operations
    //@{
    void Draw();
    void FillRemotePopupMenu( QPopupMenu& popupMenu, const MOS_ActionContext& context );
    bool CheckValidity();
    void WriteMsg( std::stringstream& strMsg );
    //@}

    const T_PointVector& GetPointList();

private:
    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::string             strMenuText_;
    ASN1T_MissionGenObject& asnObject_;

    QComboBox* pTypeCombo_;
    QComboBox* pUrgencyCombo_;
    QComboBox* pPreliminaryCombo_;
    QComboBox* pPriorityCombo_;

    MOS_ParamLocation* pLocation_;
    //@}

private:
    static const int nNbrObjType_;
    static const int nNbrUrgencyType_;
    static const int nNbrPreliminaryType_;
    static const int nNbrPriorityType_;
};


#ifdef MOS_USE_INLINE
#   include "MOS_ParamObstacle.inl"
#endif

#endif // __MOS_ParamObstacle_h_
