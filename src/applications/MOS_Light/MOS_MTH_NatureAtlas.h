// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2003 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: NLD 2003-12-03 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_NatureAtlas.h $
// $Author: Nld $
// $Modtime: 18/03/05 14:17 $
// $Revision: 4 $
// $Workfile: MOS_MTH_NatureAtlas.h $
//
// *****************************************************************************

#ifndef __MOS_MTH_NatureAtlas_h_
#define __MOS_MTH_NatureAtlas_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_Types.h"
#include "MOS_MissionTool_ABC.h"

#include "MOS_ASN_Types.h"

class QCheckBox;

// =============================================================================
// Created: NLD 2003-12-03
// =============================================================================
class MOS_MTH_NatureAtlas : public MOS_MissionTool_ABC, public QWidget
{
    MT_COPYNOTALLOWED( MOS_MTH_NatureAtlas )

public:
             MOS_MTH_NatureAtlas( ASN1T_NatureAtlas& asnNatureAtlas, QWidget* pParent, const std::string& sName );
    virtual ~MOS_MTH_NatureAtlas();

    //! @name Init
    //@{
    virtual void Initialize();
    virtual void Terminate ();
    //@}

    //! @name Tools
    //@{
    virtual void SetMission( MOS_Mission_ABC* pMission );
    virtual void SetWidgetTab( QWidget* pWidget );
    //@}

    //! @name Agent
    //@{
    virtual void SetAgent( MOS_Agent* pAgent );
    //@}

    //! @name DIN
    //@{
    virtual void WriteMsg               ( std::string& sParam );
    virtual void FillRandomParameters   ();
    virtual void ReloadParameters       ( MT_InputArchive_ABC& archive );
    virtual void ResaveParameters       ( MT_OutputArchive_ABC& archive );
    //@}

    //! @name Notify
    //@{
    virtual void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos );
    //@}

private:
    ASN1T_NatureAtlas* pAsnNatureAtlas_;

    QCheckBox* pNatureAtlasBlindeCheckBox_;
    QCheckBox* pNatureAtlasASSCheckBox_;
    QCheckBox* pNatureAtlasPCCheckBox_;
    QCheckBox* pNatureAtlasLogCheckBox_;
    QCheckBox* pNatureAtlasMortierCheckBox_;
    QCheckBox* pNatureAtlasLanceRoquetteCheckBox_;
    QCheckBox* pNatureAtlasVehiculeCheckBox_;
    QCheckBox* pNatureAtlasPersonnelAPiedCheckBox_;
    QCheckBox* pNatureAtlasHelicoptereCheckBox_;
    QCheckBox* pNatureAtlasIndefiniCheckBox_;
    QCheckBox* pNatureAtlasASACheckBox_;
    QCheckBox* pNatureAtlasNoneCheckBox_;
};

#ifdef MOS_USE_INLINE
#   include "MOS_MTH_NatureAtlas.inl"
#endif

#endif // __MOS_MTH_NatureAtlas_h_
