// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-29 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_Slider.h $
// $Author: Ape $
// $Modtime: 29/06/04 11:25 $
// $Revision: 1 $
// $Workfile: MT_Slider.h $
//
// *****************************************************************************

#ifndef __MT_Slider_h_
#define __MT_Slider_h_

#ifdef __GNUG__
#   pragma interface
#endif

class QSlider;


// =============================================================================
/** @class  MT_Slider
    @brief  MT_Slider
    @par    Using example
    @code
    MT_Slider;
    @endcode
*/
// Created: APE 2004-06-29
// =============================================================================
class MT_Slider : public QWidget
{
    Q_OBJECT
    MT_COPYNOTALLOWED( MT_Slider );

public:
    enum E_LabelPos
    {
        eTopLeft,
        eBottomRight
    };

public:
    //! @name Constructors/Destructor
    //@{
     MT_Slider( int nMinValue, int nMaxValue, int nPageStep, int nValue, const QString& strLabelPostfix, E_LabelPos nLabelPos, Orientation nOrientation, QWidget* pParent, const char* szName = 0 );
     MT_Slider( float nMinValue, float nMaxValue, float nPageStep, float nValue, const QString& strLabelPostfix, E_LabelPos nLabelPos, Orientation nOrientation, QWidget* pParent, const char* szName = 0 );
    ~MT_Slider();
    //@}

    //! @name Accessors
    //@{
    QSlider& GetSlider();
    QLabel&  GetLabel();

    void  SetValue( int nValue );
    void  SetValue( float rValue );
    float GetValue();
    //@}

private slots:
    void OnValueChanged( int nValue );

private:
    //! @name Member data
    //@{
    QSlider* pSlider_;
    QLabel*  pLabel_;

    QString strLabelPostfix_;

    bool bFloat_;
    //@}
};

#   include "MT_Slider.inl"

#endif // __MT_Slider_h_
