// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-28 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShapeDialog.h $
// $Author: Ape $
// $Modtime: 23/09/04 17:29 $
// $Revision: 2 $
// $Workfile: MT_GLDShapeDialog.h $
//
// *****************************************************************************

#ifndef __MT_GLDShapeDialog_h_
#define __MT_GLDShapeDialog_h_

#ifdef __GNUG__
#   pragma interface
#endif

#include "MT_GLDShape_ABC.h"
#include "MT_GLDShapeProperties.h"

class MT_GLDrawer;
class MT_Slider;
class QSlider;

// =============================================================================
// Created: APE 2004-06-28
// =============================================================================
class MT_GLDShapeDialog : public QDialog
{
    Q_OBJECT;
    MT_COPYNOTALLOWED( MT_GLDShapeDialog );

public:
    //! @name Constructors/Destructor
    //@{
     MT_GLDShapeDialog( QWidget* pParent, MT_GLDrawer& glDrawer );
    ~MT_GLDShapeDialog();
    //@}

    //! @name Operations
    //@{
    void SetShapes( const MT_GLDShape_ABC::T_ShapeVector& shapes );
    //@}

private slots:
    //! @name Helpers
    //@{
    void OnApply();

    void OnText();
    void OnFontSize();
    void OnLineColor();
    void OnLineWidth();
    void OnLinePattern();
    void OnArrowStart();
    void OnArrowEnd();
    void OnSize();
    void OnFillColor();
    void OnFillOpacity();
    //@}

private:
    //! @name Member data
    //@{
    MT_GLDShape_ABC::T_ShapeVector shapes_;

    QGroupBox* pTextGroup_;
    QGroupBox* pLineGroup_;
    QGroupBox* pArrowGroup_;
    QGroupBox* pSizeGroup_;
    QGroupBox* pFillGroup_;

    QLineEdit* pTextEdit_;
    QSlider* pFontSlider_;

    QPushButton* pLineColorButton_;
    MT_Slider* pLineWidthSlider_;
    MT_ValuedComboBox<MT_GLDShapeProperties::E_LinePattern>* pLinePatternCombo_;

    MT_ValuedComboBox<MT_GLDShapeProperties::E_ArrowHead>* pArrowStartCombo_;
    MT_ValuedComboBox<MT_GLDShapeProperties::E_ArrowHead>* pArrowEndCombo_;

    QSlider* pSizeSlider_;

    QPushButton* pFillColorButton_;
    MT_Slider* pFillOpacitySlider_;

    QPushButton* pOkButton_;
    QPushButton* pApplyButton_;
    QPushButton* pCancelButton_;

    QLabel* pEmptyLabel_;

    MT_GLDShapeProperties::E_Properties nModifiedProperties_;
    //@}
};

#endif // __MT_GLDShapeDialog_h_
