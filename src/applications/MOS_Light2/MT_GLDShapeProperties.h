// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-07-02 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShapeProperties.h $
// $Author: Ape $
// $Modtime: 8/07/04 18:06 $
// $Revision: 1 $
// $Workfile: MT_GLDShapeProperties.h $
//
// *****************************************************************************

#ifndef __MT_GLDShapeProperties_h_
#define __MT_GLDShapeProperties_h_

#ifdef __GNUG__
#   pragma interface
#endif


// =============================================================================
/** @class  MT_GLDShapeProperties
    @brief  MT_GLDShapeProperties
    @par    Using example
    @code
    MT_GLDShapeProperties;
    @endcode
*/
// Created: APE 2004-07-02
// =============================================================================
class MT_GLDShapeProperties
{
public:
    enum E_Properties
    {
        eNone           = 0,
        eText           = 1,
        eTextSize       = 2,
        eLineColor      = 4,
        eLineWidth      = 8,
        eLinePattern    = 16,
        eArrowStart     = 32,
        eArrowEnd       = 64,
        eFillColor      = 128,
        eFillOpacity    = 256,
        eSize           = 512,
        eAll            = 1023
    };

    enum E_LinePattern
    {
        eFull    = 0xFFFF,
        eDash1   = 0xAAAA,
        eDash2   = 0x6666,
        eDash3   = 0xF0F0,
        eDash4   = 0xFF00,
        eDash5   = 0xFF99,
        eDash6   = 0xFF66
    };

    enum E_ArrowHead
    {
        eNoArrow,
        ePointyArrow,
        eNormalArrow,
        eDiamondArrow
    };

public:
    //! @name Constructors/Destructor
    //@{
     MT_GLDShapeProperties();
    ~MT_GLDShapeProperties();
    //@}

public:
    MT_GLDShapeProperties IntersectWith( const MT_GLDShapeProperties& rhs ) const;

    void Read( MT_XXmlInputArchive& archive );
    void Write( MT_XXmlOutputArchive& archive ) const;

public:
    //! @name Public member data
    //@{
    E_Properties nProperties_;

    QString strText_;
    float rFontSize_;

    QRgb lineColor_;
    int nLineWidth_;
    E_LinePattern nLinePattern_;
    E_ArrowHead nArrowStart_;
    E_ArrowHead nArrowEnd_;

    QRgb fillColor_;

    float rSize_;
    //@}
};


#ifdef MT_USE_INLINE
#   include "MT_GLDShapeProperties.inl"
#endif

#endif // __MT_GLDShapeProperties_h_
