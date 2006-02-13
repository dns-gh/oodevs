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
// $Archive: /MVW_v10/Build/SDK/Light2/src/ObjectPanel.h $
// $Author: Age $
// $Modtime: 5/04/05 17:11 $
// $Revision: 3 $
// $Workfile: ObjectPanel.h $
//
// *****************************************************************************

#ifndef __ObjectPanel_h_
#define __ObjectPanel_h_

#include "InfoPanel_ABC.h"

class Object_ABC;
class QCheckBox;
class QGrid;
class Display;

// =============================================================================
/** @class  ObjectPanel
    @brief  ObjectPanel
*/
// Created: APE 2004-06-11
// =============================================================================
class ObjectPanel : public InfoPanel_ABC
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( ObjectPanel );

public:
    //! @name Constructors/Destructor
    //@{
    explicit ObjectPanel( QWidget* pParent );
    virtual ~ObjectPanel();
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
    virtual void OnObjectUpdated( Object_ABC& object );

    void OnApply();
    void OnCancel();
    //@}

private:
    //! @name Object types Helpers
    //@{
    void UpdateGeneric             ( Object_ABC& object );
    void UpdateSiteFranchissement  ( Object_ABC& object );
    void UpdateCamp                ( Object_ABC& object );
    void UpdateNBC                 ( Object_ABC& object );
    void UpdateROTA                ( Object_ABC& object );
    void UpdateItineraireLogistique( Object_ABC& object );    
    //@}

private:
    //! @name Member data
    //@{
    Display* display_;
    QSpinBox*  pPercentBuiltEdit_;
    QSpinBox*  pPercentValueEdit_;
    QSpinBox*  pPercentAroundEdit_;
    QCheckBox* pIsUnderPrepCheckBox_;

    QPushButton* pApplyButton_;
    QPushButton* pCancelButton_;
    //@}
};

#endif // __ObjectPanel_h_
