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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MOS_DynaObjectPanel.h $
// $Author: Age $
// $Modtime: 5/04/05 17:11 $
// $Revision: 3 $
// $Workfile: MOS_DynaObjectPanel.h $
//
// *****************************************************************************

#ifndef __MOS_DynaObjectPanel_h_
#define __MOS_DynaObjectPanel_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MOS_InfoPanel_ABC.h"

class MOS_DynaObject_ABC;
class QCheckBox;

// =============================================================================
/** @class  MOS_DynaObjectPanel
    @brief  MOS_DynaObjectPanel
*/
// Created: APE 2004-06-11
// =============================================================================
class MOS_DynaObjectPanel : public MOS_InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MOS_DynaObjectPanel );

public:
    //! @name Constructors/Destructor
    //@{
     MOS_DynaObjectPanel( QWidget* pParent );
    ~MOS_DynaObjectPanel();
    //@}

private slots:
    //! @name Helpers
    //@{
    virtual void OnUpdate();
    virtual void OnDynaObjectUpdated( MOS_DynaObject_ABC& object );

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

    void UpdateGeneric             ( MOS_DynaObject_ABC& object );
    void UpdateSiteFranchissement  ( MOS_DynaObject_ABC& object );
    void UpdateCamp                ( MOS_DynaObject_ABC& object );
    void UpdateNBC                 ( MOS_DynaObject_ABC& object );
    void UpdateROTA                ( MOS_DynaObject_ABC& object );
    void UpdateItineraireLogistique( MOS_DynaObject_ABC& object );    
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


#ifdef MOS_USE_INLINE
#   include "MOS_DynaObjectPanel.inl"
#endif

#endif // __MOS_DynaObjectPanel_h_
