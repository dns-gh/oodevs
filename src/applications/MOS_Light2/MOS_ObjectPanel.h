// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-11 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_ObjectPanel.h $
// $Author: Age $
// $Modtime: 5/04/05 17:11 $
// $Revision: 3 $
// $Workfile: MOS_ObjectPanel.h $
//
// *****************************************************************************

#ifndef __MOS_ObjectPanel_h_
#define __MOS_ObjectPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_InfoPanel_ABC.h"

class MOS_Object_ABC;
class QCheckBox;

// =============================================================================
/** @class  MOS_ObjectPanel
    @brief  MOS_ObjectPanel
*/
// Created: APE 2004-06-11
// =============================================================================
class MOS_ObjectPanel : public MOS_InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_ObjectPanel );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_ObjectPanel( QWidget* pParent );
    ~MOS_ObjectPanel();
    //@}

private slots:
    //! @name Helpers
    //@{
    virtual void OnUpdate();
    virtual void OnObjectUpdated( MOS_Object_ABC& object );

    void OnApply();
    void OnCancel();
    //@}

private:
    //! @name Object types Helpers
    //@{
    void InitializeGeneric             ();
    void InitializeSiteFranchissement  ();
    void InitializeCamp                ();
    void InitializeNBC                 ();
    void InitializeROTA                ();
    void InitializeItineraireLogistique();

    void UpdateGeneric             ( MOS_Object_ABC& object );
    void UpdateSiteFranchissement  ( MOS_Object_ABC& object );
    void UpdateCamp                ( MOS_Object_ABC& object );
    void UpdateNBC                 ( MOS_Object_ABC& object );
    void UpdateROTA                ( MOS_Object_ABC& object );
    void UpdateItineraireLogistique( MOS_Object_ABC& object );    
    //@}

private:
    //! @name Member data
    //@{
    QLabel* pIdLabel_;
    QLabel* pObjectTypeLabel_;
    QLabel* pPositionLabel_;
    QSpinBox* pPercentBuiltEdit_;
    QSpinBox* pPercentValueEdit_;
    QSpinBox* pPercentAroundEdit_;
    QCheckBox* pIsUnderPrepCheckBox_;

    QLabel* pCrossingWidthTitle_;
    QLabel* pCrossingWidthLabel_;
    QLabel* pCrossingDepthTitle_;
    QLabel* pCrossingDepthLabel_;
    QLabel* pCrossingRiverSpeedTitle_;
    QLabel* pCrossingRiverSpeedLabel_;
    QLabel* pCrossingBanksNeedWorkTitle_;
    QLabel* pCrossingBanksNeedWorkLabel_;

    QLabel* pNBCAgentTitle_;
    QLabel* pNBCAgentLabel_;

	QLabel* pTC2Title_;
	QLabel* pTC2Label_;

    QLabel* pROTADangerTitle_;
    QLabel* pROTADangerLabel_;
    QLabel* pROTAAgentsNbcTitle_;
    QLabel* pROTAAgentsNbcLabel_;

    QLabel* pItLogEquippedTitle_;
    QLabel* pItLogEquippedLabel_;
    QLabel* pItLogFlowTitle_;
    QLabel* pItLogFlowLabel_;
    QLabel* pItLogWidthTitle_;
    QLabel* pItLogWidthLabel_;
    QLabel* pItLogLengthTitle_;
    QLabel* pItLogLengthLabel_;
    QLabel* pItLogMaxWeightTitle_;
    QLabel* pItLogMaxWeightLabel_;

    QPushButton* pApplyButton_;
    QPushButton* pCancelButton_;
    //@}
};

#endif // __MOS_ObjectPanel_h_
