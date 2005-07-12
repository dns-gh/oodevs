// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2004 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************
//
// $Created: APE 2004-06-24 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light2/src/MT_GLDShape.cpp $
// $Author: Age $
// $Modtime: 17/03/05 11:44 $
// $Revision: 3 $
// $Workfile: MT_GLDShape.cpp $
//
// *****************************************************************************

#ifdef __GNUG__
#   pragma implementation
#endif

#include "MOS_Light2_Pch.h"
#include "MT_GLDShape.h"
#include "moc_MT_GLDShape.cpp"

#include "MT_GLDrawer.h"
#include "MT_GLFont.h"

#include <qimage.h>
#include <qfiledialog.h>

#ifndef MT_USE_INLINE
#   include "MT_GLDShape.inl"
#endif


// -----------------------------------------------------------------------------
// Name: MT_GLDShape constructor
/** @param  nType 
*/
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
MT_GLDShape::MT_GLDShape( MT_GLDrawer& glDrawer, E_ShapeType nType )
    : MT_GLDShape_ABC   ( glDrawer )
    , nType_            ( nType )
    , properties_       ( glDrawer.GetDefautShapeProperties() )
    , pMousePoint_      ( 0 )
    , nGLDisplayList_   ( 0 )
{
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape destructor
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
MT_GLDShape::~MT_GLDShape()
{
    if( nGLDisplayList_ != 0 )
        glDeleteLists( nGLDisplayList_, 1 );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::Draw
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
void MT_GLDShape::Draw()
{
    if( points_.empty() )
        return;

    glLineWidth( properties_.nLineWidth_ );
    this->SetColor( properties_.lineColor_ );
    if( properties_.nLinePattern_ != MT_GLDShapeProperties::eFull )
    {
        glLineStipple( 2, properties_.nLinePattern_ );
        glEnable( GL_LINE_STIPPLE );
    }

    if( pMousePoint_ != 0 )
        points_.push_back( *pMousePoint_ );

    switch( nType_ )
    {
        case ePoint:
        {
            MT_Vector2D point = points_.front();
            float rSize = properties_.rSize_;
            glBegin( GL_LINES );
                glVertex2d( point.rX_ - rSize, point.rY_ - rSize );
                glVertex2d( point.rX_ + rSize, point.rY_ + rSize );
                glVertex2d( point.rX_ + rSize, point.rY_ - rSize );
                glVertex2d( point.rX_ - rSize, point.rY_ + rSize );
            glEnd();
            glDrawer_.GetFont().Print( point, std::string( "  " ) + properties_.strText_.ascii(), properties_.rFontSize_, 0, MT_GLFont::eVCenter );
            break;
        }
        case eRectangle:
        {
            if( points_.size() > 1 )
            {
                MT_Vector2D point1 = points_[0];
                MT_Vector2D point2 = points_[1];
                
                this->SetColor( properties_.fillColor_ );
                glBegin( GL_TRIANGLE_FAN );
                    glVertex2d( point1.rX_, point1.rY_ );
                    glVertex2d( point1.rX_, point2.rY_ );
                    glVertex2d( point2.rX_, point2.rY_ );
                    glVertex2d( point2.rX_, point1.rY_ );
                glEnd();
                this->SetColor( properties_.lineColor_ );
                glBegin( GL_LINE_LOOP );
                    glVertex2d( point1.rX_, point1.rY_ );
                    glVertex2d( point1.rX_, point2.rY_ );
                    glVertex2d( point2.rX_, point2.rY_ );
                    glVertex2d( point2.rX_, point1.rY_ );
                glEnd();
                glDrawer_.GetFont().Print( point1, std::string( "  " ) + properties_.strText_.ascii(), properties_.rFontSize_, 0, MT_GLFont::eVBottom );
            }
            break;
        }
        case eLine:
        {
            if( points_.size() > 1 )
            {
                MT_Vector2D firstPoint = this->DrawArrowhead( points_[1], points_[0], properties_.nArrowStart_);
                MT_Vector2D lastPoint  = this->DrawArrowhead( points_[points_.size()-2], points_[points_.size()-1], properties_.nArrowEnd_);
 
                glBegin( GL_LINE_STRIP );
                    glVertex2d( firstPoint.rX_, firstPoint.rY_ );
                    for( uint n = 1; n < points_.size() - 1; ++n )
                        glVertex2d( points_[n].rX_, points_[n].rY_ );
                    glVertex2d( lastPoint.rX_, lastPoint.rY_ );
                glEnd();
                glDrawer_.GetFont().Print( points_.front(), std::string( "  " ) + properties_.strText_.ascii(), properties_.rFontSize_, 0, MT_GLFont::eVCenter );

                
            }
            break;
        }
        case ePolygon:
        {
            if( points_.size() > 1 )
            {
                glBegin( GL_LINE_LOOP );
                    for( IT_PointVector it = points_.begin(); it != points_.end(); ++it )
                        glVertex2d( (*it).rX_, (*it).rY_ );
                glEnd();
                glDrawer_.GetFont().Print( points_.front(), std::string( "  " ) + properties_.strText_.ascii(), properties_.rFontSize_, 0, MT_GLFont::eVCenter );

            }
            break;
        }
        case eCircle:
        {
            if( points_.size() > 1 )
            {
                MT_Vector2D center = points_[0];
                MT_Vector2D point = points_[1];
                double rRadius = center.Distance( point );
                this->SetColor( properties_.fillColor_ );
                glBegin( GL_TRIANGLE_FAN );
                    glVertex2d( center.rX_, center.rY_ );
                    for( double rAngle2 = 0; rAngle2 < 2 * MT_PI; rAngle2 += 0.3f )
                    {
                        double rX = center.rX_ + ( rRadius * cos( rAngle2 ) );
                        double rY = center.rY_ + ( rRadius * sin( rAngle2 ) );
                        glVertex2d( rX, rY );
                    }
                    glVertex2d( center.rX_ + rRadius, center.rY_ );
                glEnd();
                this->SetColor( properties_.lineColor_ );
                glBegin( GL_LINE_LOOP );
                    for( double rAngle = 0; rAngle < 2 * MT_PI; rAngle += 0.3f )
                    {
                        double rX = center.rX_ + ( rRadius * cos( rAngle ) );
                        double rY = center.rY_ + ( rRadius * sin( rAngle ) );
                        glVertex2d( rX, rY );
                    }
                glEnd();
                glDrawer_.GetFont().Print( center, properties_.strText_.ascii(), properties_.rFontSize_, 0, MT_GLFont::eCenter );

            }
            break;
        }
        case eText:
        {
            glDrawer_.GetFont().Print( points_.front(), properties_.strText_.ascii(), properties_.rFontSize_ );
            break;
        }
        case eImage:
        {
            if( points_.size() > 1 )
            {
                this->SetColor( properties_.fillColor_ );
                glPushMatrix();
                MT_Rect rect;
                rect.Set( points_[0], points_[1] );
                glTranslatef( rect.GetPointUpLeft().rX_, rect.GetPointUpLeft().rY_, 0.0f );
                glScalef( rect.GetWidth(), rect.GetHeight(), 0 );
                glCallList( nGLDisplayList_ );
                glPopMatrix();
            }
            break;
        }
        default:
            assert( 0 );
    }

    if( nState_ == eSelected || nState_ == eCreation )
    {
        glPointSize( properties_.nLineWidth_ + 4 );
        glColor4d( 0.0, 0.0, 0.0, 1.0 );
        glBegin( GL_POINTS );
            for( IT_PointVector it = points_.begin(); it != points_.end(); ++it )
                glVertex2d( (*it).rX_, (*it).rY_ );
        glEnd();
        glPointSize( properties_.nLineWidth_ + 2 );
        glColor4d( 1.0, 1.0, 1.0, 1.0 );
        glBegin( GL_POINTS );
            for( it = points_.begin(); it != points_.end(); ++it )
                glVertex2d( (*it).rX_, (*it).rY_ );
        glEnd();
    }

    if( pMousePoint_ != 0 )
        points_.pop_back();

    glDisable( GL_LINE_STIPPLE );
}


const double rMinDist = 2;

// -----------------------------------------------------------------------------
// Name: MT_GLDShape::IsAtPos
/** @param  vPos 
    @return 
*/
// Created: APE 2004-06-28
// -----------------------------------------------------------------------------
bool MT_GLDShape::IsAtPos( const MT_Vector2D& vPos )
{
    assert( ! points_.empty() );

    if( nType_ == ePoint )
        return vPos.SquareDistance( points_.front() ) < (rMinDist * rMinDist);

    if( nType_ == eRectangle )
    {
        MT_Vector2D p1( points_[0].rX_, points_[1].rY_ );
        MT_Vector2D p2( points_[1].rX_, points_[0].rY_ );
        MT_Line tmpLine1( points_[0], p1 );
        MT_Line tmpLine2( points_[0], p2 );
        MT_Line tmpLine3( points_[1], p1 );
        MT_Line tmpLine4( points_[1], p2 );
        return(    tmpLine1.IsInside( vPos, rMinDist )
                || tmpLine2.IsInside( vPos, rMinDist )
                || tmpLine3.IsInside( vPos, rMinDist )
                || tmpLine4.IsInside( vPos, rMinDist ) );
    }

    if( nType_ == eCircle )
    {
        float rDist = vPos.Distance( points_.front() );
        float rRadius = points_.front().Distance( points_[1] );
        float rDiff = fabsf(rDist - rRadius);
        return( rDiff < rMinDist || rDist < rMinDist );

    }

    if( nType_ == eText )
    {
        MT_Vector2D vTextSize = glDrawer_.GetFont().GetTextSize( properties_.strText_.ascii() );
        MT_Rect rect;
        rect.Set( points_.front(), points_.front() + ((double)properties_.rFontSize_ * vTextSize) );
        if( rect.IsInside( vPos ) )
            return true;
    }

    if( nType_ == eImage )
    {
        MT_Rect rect;
        rect.Set( points_[0], points_[1] );
        return rect.IsInside( vPos, rMinDist );
    }
    
    for( uint n = 0; n < points_.size() - 1; ++n )
    {
        MT_Line tmpLine( points_[n], points_[n+1] );
        if( tmpLine.IsInside( vPos, rMinDist ) )
            return true;
    }

    if( nType_ == ePolygon )
    {
        MT_Line tmpLine( points_[points_.size()-1], points_[0] );
        if( tmpLine.IsInside( vPos, rMinDist ) )
            return true;
    }

    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::GetPointAt
/** @param  vPos 
    @return 
*/
// Created: APE 2004-07-01
// -----------------------------------------------------------------------------
MT_Vector2D* MT_GLDShape::GetPointAt( const MT_Vector2D& vPos )
{
    for( IT_PointVector it = points_.begin(); it != points_.end(); ++it )
        if( vPos.SquareDistance( *it ) < rMinDist )
            return &(*it);
    return 0;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::StartCreation
/** @param  pFirstPoint 
*/
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
void MT_GLDShape::StartCreation( const MT_Vector2D* pFirstPoint )
{
    if( nType_ == ePoint && pFirstPoint != 0 )
    {
        points_.push_back( *pFirstPoint );
        emit CreationDone();
        return;
    }

    if( nType_ == eText && pFirstPoint != 0 )
        points_.push_back( *pFirstPoint );

    if( nType_ == eImage )
    {
        if( ! this->GetImageFileName() || ! this->InitImage() )
        {
            emit CreationCanceled();
            return;
        }
    }

    glDrawer_.GetGLWidget().installEventFilter( this );
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::StopCreation
/** @param  bOk 
*/
// Created: APE 2004-06-25
// -----------------------------------------------------------------------------
void MT_GLDShape::StopCreation( bool bOk )
{
    delete pMousePoint_;
    pMousePoint_ = 0;
    glDrawer_.GetGLWidget().removeEventFilter( this );

    if( bOk )
        emit CreationDone();
    else
        emit CreationCanceled();
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::eventFilter
/** @param  pObject 
    @param  pEvent 
    @return 
*/
// Created: APE 2004-06-24
// -----------------------------------------------------------------------------
bool MT_GLDShape::eventFilter( QObject* /*pObject*/, QEvent* pEvent )
{
    if( pEvent->type() == QEvent::KeyPress )
    {
        QKeyEvent* pKeyEvent = (QKeyEvent*)pEvent;
        switch( pKeyEvent->key() )
        {
            case Qt::Key_Backspace:
            {
                if( nType_ == eText )
                {
                    if( properties_.strText_.length() > 0 )
                        properties_.strText_.truncate( properties_.strText_.length() - 1 );
                }
                else
                {
                    if( ! points_.empty() )
                        points_.pop_back();
                }
                return true;
            }
            case Qt::Key_Delete:
            {
                if( ! points_.empty() )
                    points_.pop_back();
                return true;
            }
            case Qt::Key_Escape:
            {
                this->StopCreation( false );
                return true;
            }
            case Qt::Key_Return:
            case Qt::Key_Enter:
            {
                if(    ( nType_ == eText && ! points_.empty() )
                    || ( nType_ == eLine && points_.size() >= 2 ) 
                    || ( nType_ == ePolygon && points_.size() >= 3 ) )
                    this->StopCreation( true );
                return true;
            }
            default:
            {
                properties_.strText_ += pKeyEvent->text();
                return true;
            }
        }
    }

    if( pEvent->type() == QEvent::MouseButtonPress )
    {
        QMouseEvent* pMouseEvent = (QMouseEvent*)pEvent;
        if( pMouseEvent->button() & Qt::LeftButton )
        {
            MT_Vector2D vScreenPos( pMouseEvent->x(), pMouseEvent->y() );
            MT_Vector2D vGLPos = glDrawer_.ScreenToGL( vScreenPos );
            if( nType_ == eText )
            {
                points_.clear();
                points_.push_back( vGLPos );
                return true;
            }
            points_.push_back( vGLPos );
            if(      nType_ == ePoint 
                || ( nType_ == eCircle && points_.size() == 2 )
                || ( nType_ == eRectangle && points_.size() == 2 )
                || ( nType_ == eImage && points_.size() == 2 ) )
            {
                this->StopCreation( true );
            }
            return true;
        }
        if( pMouseEvent->button() & Qt::RightButton )
            return true;
    }

    if( pEvent->type() == QEvent::MouseButtonDblClick )
    {
        if( ( nType_ != ePolygon && points_.size() >= 2 ) || points_.size() >= 3 )
            this->StopCreation( true );
        else
            this->StopCreation( false );

        return true;
    }

    if( pEvent->type() == QEvent::MouseMove )
    {
        QMouseEvent* pMouseEvent = (QMouseEvent*)pEvent;
        MT_Vector2D vScreenPos( pMouseEvent->x(), pMouseEvent->y() );
        if( pMousePoint_ == 0 )
            pMousePoint_ = new MT_Vector2D();
        *pMousePoint_ = glDrawer_.ScreenToGL( vScreenPos );
    }

    return false;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::GetImageFileName
/** @return 
*/
// Created: APE 2004-07-06
// -----------------------------------------------------------------------------
bool MT_GLDShape::GetImageFileName()
{
    QString strFileName = QFileDialog::getOpenFileName(
                    "",
                    "Images (*.bmp *.png *.xpm *.jpg)",
                    & glDrawer_.GetGLWidget(),
                    "",
                    tr( "Choisir un fichier d'image" ) );

    if( strFileName.isNull() )
        return false;

    strFileName_ = strFileName.ascii();
    return true;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::InitImage
/** @return 
*/
// Created: APE 2004-07-02
// -----------------------------------------------------------------------------
bool MT_GLDShape::InitImage()
{
    QImage image( strFileName_.c_str() );
    if( image.isNull() )
        return false;

    int nTrueWidth = image.width();
    int nTrueHeight = image.height();
    int nWidth = 2;
    int nHeight = 2;

    while( nWidth < nTrueWidth )
        nWidth *= 2;
    while( nHeight < nTrueHeight )
        nHeight *= 2;

    QImage resizedImage = image.copy( 0, 0, nWidth, nHeight );
    resizedImage = resizedImage.convertDepth( 32 );

    nGLDisplayList_ = glGenLists(1);
    glNewList( nGLDisplayList_, GL_COMPILE);

    glNormal3f( 0.0, 0.0, 1.0 );

	glTexImage2D( GL_TEXTURE_2D, 0, 4, resizedImage.width(), resizedImage.height(), 0, GL_BGRA_EXT, GL_UNSIGNED_BYTE, resizedImage.bits() );
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST ); 
	glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

    float rWidthRatio = (float)nTrueWidth / (float)nWidth;
    float rHeightRatio = (float)nTrueHeight / (float)nHeight;

	glBegin( GL_QUADS );
	    glTexCoord2f( 0, rHeightRatio );
	    glVertex2f( 0, 0 );
	    glTexCoord2f( rWidthRatio, rHeightRatio );
	    glVertex2f( 1, 0 );
	    glTexCoord2f( rWidthRatio, 0 );
	    glVertex2f( 1, 1 );
	    glTexCoord2f( 0, 0 );
	    glVertex2f( 0, 1 );
    glEnd();

	glEndList();

    return true;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::DrawArrowhead
/** @param  src 
    @param  dest 
    @param  nArrowType 
*/
// Created: APE 2004-07-05
// -----------------------------------------------------------------------------
MT_Vector2D MT_GLDShape::DrawArrowhead( const MT_Vector2D& src, const MT_Vector2D& dest, MT_GLDShapeProperties::E_ArrowHead nArrowType )
{
    if ( src == dest || nArrowType == MT_GLDShapeProperties::eNoArrow )
        return dest;

    MT_Vector2D dir( (src - dest).Normalized() * (MT_Float)(properties_.rSize_) );
    MT_Vector2D tmp( (src - dest).Normalized() * (MT_Float)(properties_.rSize_) );

    std::swap( tmp.rX_, tmp.rY_ );
    tmp.rX_ = -tmp.rX_;

    double rPointyFactor = 1;
    if( nArrowType == MT_GLDShapeProperties::ePointyArrow )
        rPointyFactor = 1.5;

    if( nArrowType == MT_GLDShapeProperties::eDiamondArrow )
        rPointyFactor = 0.75;

    MT_Vector2D pos1 ( dest + tmp + dir * rPointyFactor * 2.0 );
    MT_Vector2D pos2 ( dest - tmp + dir * rPointyFactor * 2.0 );
    MT_Vector2D pos3 ( dest + dir * 2.0 / rPointyFactor );

    glBegin( GL_TRIANGLE_FAN );
	    glVertex2f( dest.rX_, dest.rY_ );
        glVertex2f( pos1.rX_, pos1.rY_ );
        glVertex2f( pos3.rX_, pos3.rY_ );
	    glVertex2f( pos2.rX_, pos2.rY_ );
	glEnd();

    return pos3;
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::Read
/** @param  archive 
*/
// Created: APE 2004-07-06
// -----------------------------------------------------------------------------
void MT_GLDShape::Read( MT_XXmlInputArchive& archive )
{
    archive.Section( "Shape" );
    nState_ = MT_GLDShape_ABC::eNormal;
    archive.ReadAttribute( "Type", (int&)nType_ );
    if( nType_ == eImage )
    {
        archive.ReadField( "Filename", strFileName_ );
        this->InitImage();
    }

    properties_.Read( archive );

    archive.BeginList( "Points" );
    while( archive.NextListElement() )
    {
        archive.Section( "Point" );
        MT_Vector2D vPoint;
        vPoint.Read( archive );
        points_.push_back( vPoint );
        archive.EndSection();
    }
    archive.EndList();
    archive.EndSection();
}


// -----------------------------------------------------------------------------
// Name: MT_GLDShape::Write
/** @param  archive 
*/
// Created: APE 2004-07-06
// -----------------------------------------------------------------------------
void MT_GLDShape::Write( MT_XXmlOutputArchive& archive ) const
{
    archive.Section( "Shape" );
    archive.WriteAttribute( "Type", (int)nType_ );
    if( nType_ == eImage )
        archive.WriteField( "Filename", strFileName_ );

    properties_.Write( archive );

    archive.BeginList( "Points", points_.size() );
    for( CIT_PointVector it = points_.begin(); it != points_.end(); ++it )
    {
        archive.Section( "Point" );
        (*it).Write( archive );
        archive.EndSection();
    }
    archive.EndList();
    archive.EndSection();
}
