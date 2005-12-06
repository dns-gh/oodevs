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
class QGrid;

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
    explicit MOS_ObjectPanel( QWidget* pParent );
    virtual ~MOS_ObjectPanel();
    //@}

private:
    //! @name Operations
    //@{
    virtual void OnClearSelection();
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
    void InitializeGeneric             ( QWidget* pParent );
    void InitializeSiteFranchissement  ( QWidget* pParent );
    void InitializeCamp                ( QWidget* pParent );
    void InitializeNBC                 ( QWidget* pParent );
    void InitializeROTA                ( QWidget* pParent );
    void InitializeItineraireLogistique( QWidget* pParent );

    void UpdateGeneric             ( MOS_Object_ABC& object );
    void UpdateSiteFranchissement  ( MOS_Object_ABC& object );
    void UpdateCamp                ( MOS_Object_ABC& object );
    void UpdateNBC                 ( MOS_Object_ABC& object );
    void UpdateROTA                ( MOS_Object_ABC& object );
    void UpdateItineraireLogistique( MOS_Object_ABC& object );    
    //@}

    //! @name Widget construction Helpers
    //@{
    void AddRow( const QString& strLabel, QLabel*&    pValueLabel   , QWidget* pParent );
    void AddRow( const QString& strLabel, QSpinBox*&  pValueSpinBox , QWidget* pParent );
    void AddRow( const QString& strLabel, QCheckBox*& pValueCheckBox, QWidget* pParent );
    //@}

private:
    //! @name Member data
    //@{
    QGroupBox* pGenericGroup_;

    QLabel*    pIdLabel_;
    QLabel*    pNameLabel_;
    QLabel*    pObjectTypeLabel_;
    QLabel*    pPositionLabel_;
    QSpinBox*  pPercentBuiltEdit_;
    QSpinBox*  pPercentValueEdit_;
    QSpinBox*  pPercentAroundEdit_;
    QCheckBox* pIsUnderPrepCheckBox_;
    QLabel*    pDotationConstructionLabel_;
    QLabel*    pDotationValorisationLabel_;

    // crossing
    QGroupBox* pCrossingGroup_;
    QLabel*    pCrossingWidthLabel_;
    QLabel*    pCrossingDepthLabel_;
    QLabel*    pCrossingRiverSpeedLabel_;
    QLabel*    pCrossingBanksNeedWorkLabel_;

    // NBC
    QGroupBox* pNBCGroup_;
    QLabel*    pNBCAgentLabel_;

    // Camp
    QGroupBox* pCampGroup_;
	QLabel*    pTC2Label_;

    // ROTA
    QGroupBox* pROTAGroup_;
    QLabel*    pROTADangerLabel_;
    QLabel*    pROTAAgentsNbcLabel_;

    // Logistic route
    QGroupBox* pItLogGroup_;
    QLabel*    pItLogEquippedLabel_;
    QLabel*    pItLogFlowLabel_;
    QLabel*    pItLogWidthLabel_;
    QLabel*    pItLogLengthLabel_;
    QLabel*    pItLogMaxWeightLabel_;

    QPushButton* pApplyButton_;
    QPushButton* pCancelButton_;
    //@}
};

#endif // __MOS_ObjectPanel_h_
