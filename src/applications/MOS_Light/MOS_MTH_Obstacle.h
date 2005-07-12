// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: AGN 2004-04-26 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_MTH_Obstacle.h $
// $Author: Nld $
// $Modtime: 10/09/04 11:42 $
// $Revision: 2 $
// $Workfile: MOS_MTH_Obstacle.h $
//
// *****************************************************************************

#ifndef __MOS_MTH_Obstacle_h_
#define __MOS_MTH_Obstacle_h_

#include "MOS_Types.h"

#include "MOS_MissionTool_ABC.h"
#include "MOS_ASN_Types.h"

#include <qwidget.h>

class MOS_MTH_Localisation;
class QComboBox;

// =============================================================================
/** @class  MOS_MTH_Obstacle
    @brief  MOS_MTH_Obstacle
    @par    Using example
    @code
    MOS_MTH_Obstacle;
    @endcode
*/
// Created: AGN 2004-04-26
// =============================================================================
class MOS_MTH_Obstacle
: public QWidget
, public MOS_MissionTool_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    MOS_MTH_Obstacle( ASN1T_MissionGenObject& asnObjet, QWidget* pParent, const std::string& strName );
    virtual ~MOS_MTH_Obstacle();
    //@}

    //-------------------------------------------------------------------------
    /** @name Tools*/
    //-------------------------------------------------------------------------
    //@{
    virtual void SetMission( MOS_Mission_ABC* pMission ) ;
    virtual void SetWidgetTab( QWidget* pWidget ) ;
    //@}

    //-------------------------------------------------------------------------
    /** @name Agent*/
    //-------------------------------------------------------------------------
    //@{
    virtual void SetAgent( MOS_Agent* pAgent ) ;
    //@}

    //-------------------------------------------------------------------------
    /** @name DIN*/
    //-------------------------------------------------------------------------
    //@{
    virtual void WriteMsg( std::string& sParam ) ;
    void FillRandomParameters();
    void ReloadParameters( MT_InputArchive_ABC& archive );
    void ResaveParameters( MT_OutputArchive_ABC& archive );
    //@}

    //-------------------------------------------------------------------------
    /** @name Notify*/
    //-------------------------------------------------------------------------
    //@{
    virtual void NotifyAgentHasMoved( MOS_Agent& agent, const MT_Vector2D& vPos ) ;
    //@}

    //-------------------------------------------------------------------------
    /** @name Init*/
    //-------------------------------------------------------------------------
    //@{
    virtual void Initialize() ;
    virtual void Terminate() ;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    MOS_MTH_Obstacle( const MOS_MTH_Obstacle& );            //!< Copy constructor
    MOS_MTH_Obstacle& operator=( const MOS_MTH_Obstacle& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    MOS_Agent* pAgent_;
    ASN1T_MissionGenObject& genObject_;

    QComboBox* pTypeObstacleGUI_;
    
    MOS_MTH_Localisation* pLocalisation_;

    QComboBox* pUrgenceGUI_;
    QComboBox* pPreliminaireGUI_;
    QComboBox* pPrioriteGUI_;
    //@}
};

#ifdef MOS_USE_INLINE
#   include "MOS_MTH_Obstacle.inl"
#endif

#endif // __MOS_MTH_Obstacle_h_
