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

#include "MOS_InfoPanel_ABC.h"

class MOS_Object_ABC;
class QCheckBox;
class QGrid;
class MOS_Display;

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
    MOS_Display* display_;
    QSpinBox*  pPercentBuiltEdit_;
    QSpinBox*  pPercentValueEdit_;
    QSpinBox*  pPercentAroundEdit_;
    QCheckBox* pIsUnderPrepCheckBox_;

    QPushButton* pApplyButton_;
    QPushButton* pCancelButton_;
    //@}
};

#endif // __MOS_ObjectPanel_h_
