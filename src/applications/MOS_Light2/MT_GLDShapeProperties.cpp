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
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShapeProperties.cpp $
// $Author: Ape $
// $Modtime: 7/07/04 17:00 $
// $Revision: 1 $
// $Workfile: MT_GLDShapeProperties.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MT_GLDShapeProperties.h"

#ifndef MT_USE_INLINE
#   include "MT_GLDShapeProperties.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeProperties constructor
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
MT_GLDShapeProperties::MT_GLDShapeProperties()
    : nProperties_ ( eAll )
    , strText_     ( "" )
    , rFontSize_   ( 1 )
    , lineColor_   ( qRgba( 255, 255, 255, 255 ) )
    , nLineWidth_  ( 2 )
    , nLinePattern_( eFull )
    , nArrowStart_ ( eNoArrow )
    , nArrowEnd_   ( eNoArrow )
    , fillColor_   ( qRgba( 255, 255, 255, 125 ) )
    , rSize_       ( 1 )
{
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeProperties destructor
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
MT_GLDShapeProperties::~MT_GLDShapeProperties()
{
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeProperties::IntersectWith
/** @param  rhs 
    @return 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
MT_GLDShapeProperties MT_GLDShapeProperties::IntersectWith( const MT_GLDShapeProperties& rhs ) const
{
    MT_GLDShapeProperties result;

    if( nProperties_ & eText && rhs.nProperties_ & eText )
    {
        if( strText_ == rhs.strText_ )
            result.strText_ = strText_;
    }

    if( nProperties_ & eTextSize && rhs.nProperties_ & eTextSize )
    {
        if( rFontSize_ == rhs.rFontSize_ )
            result.rFontSize_ = rFontSize_;
    }

    if( nProperties_ & eLineColor && rhs.nProperties_ & eLineColor )
    {
        if( lineColor_ == rhs.lineColor_ )
            result.lineColor_ = lineColor_;
    }

    if( nProperties_ & eLineWidth && rhs.nProperties_ & eLineWidth )
    {
        if( nLineWidth_ == rhs.nLineWidth_ )
            result.nLineWidth_ = nLineWidth_;
    }

    if( nProperties_ & eLinePattern && rhs.nProperties_ & eLinePattern )
    {
        if( nLinePattern_ == rhs.nLinePattern_ )
            result.nLinePattern_ = nLinePattern_;
    }

    if( nProperties_ & eArrowStart && rhs.nProperties_ & eArrowStart )
    {
        if( nArrowStart_ == rhs.nArrowStart_ )
            result.nArrowStart_ = nArrowStart_;
    }

    if( nProperties_ & eArrowEnd && rhs.nProperties_ & eArrowEnd )
    {
        if( nArrowEnd_ == rhs.nArrowEnd_ )
            result.nArrowEnd_ = nArrowEnd_;
    }

    if( nProperties_ & eFillColor && rhs.nProperties_ & eFillColor )
    {
        if( qRgb( qRed(fillColor_), qGreen(fillColor_), qBlue(fillColor_) ) == qRgb( qRed(rhs.fillColor_), qGreen(rhs.fillColor_), qBlue(rhs.fillColor_) ) )
            result.fillColor_ = qRgba( qRed(fillColor_), qGreen(fillColor_), qBlue(fillColor_), qAlpha(result.fillColor_) );
    }

    if( nProperties_ & eFillOpacity && rhs.nProperties_ & eFillOpacity )
    {
        if( qAlpha( fillColor_ ) == qAlpha( rhs.fillColor_ ) )
            result.fillColor_ = qRgba( qRed(result.fillColor_), qGreen(result.fillColor_), qBlue(result.fillColor_), qAlpha(fillColor_) );
    }

    if( nProperties_ & eSize && rhs.nProperties_ & eSize )
    {
        if( rSize_ == rhs.rSize_ )
            result.rSize_ = rSize_;
    }

    result.nProperties_ = (E_Properties)(nProperties_ & rhs.nProperties_);

    return result;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeProperties::Read
/** @param  archive 
    @return 
*/
// Created: APE 2004-07-06
// -----------------------------------------------------------------------------
void MT_GLDShapeProperties::Read( MT_XXmlInputArchive& archive )
{
    std::string strText;
    archive.ReadField( "AvailableProperties", (int&)nProperties_ );
    archive.ReadField( "Text",                strText );
    archive.ReadField( "FontSize",            rFontSize_  );
    archive.ReadField( "LineColor",           lineColor_ );
    archive.ReadField( "LineWidth",           nLineWidth_ );
    archive.ReadField( "LinePattern",         (int&)nLinePattern_ );
    archive.ReadField( "ArrowStart",          (int&)nArrowStart_ );
    archive.ReadField( "ArrowEnd",            (int&)nArrowEnd_ );
    archive.ReadField( "FillColor",           fillColor_ );
    archive.ReadField( "Size",                rSize_ );
    strText_ = QString( strText.c_str() );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShapeProperties::Write
/** @param  archive 
*/
// Created: APE 2004-07-06
// -----------------------------------------------------------------------------
void MT_GLDShapeProperties::Write( MT_XXmlOutputArchive& archive ) const
{
    std::string strText = strText_.ascii();
    archive.WriteField( "AvailableProperties", nProperties_ );
    archive.WriteField( "Text",                strText );
    archive.WriteField( "FontSize",            rFontSize_  );
    archive.WriteField( "LineColor",           lineColor_ );
    archive.WriteField( "LineWidth",           nLineWidth_ );
    archive.WriteField( "LinePattern",         nLinePattern_ );
    archive.WriteField( "ArrowStart",          nArrowStart_ );
    archive.WriteField( "ArrowEnd",            nArrowEnd_ );
    archive.WriteField( "FillColor",           fillColor_ );
    archive.WriteField( "Size",                rSize_ );
}
