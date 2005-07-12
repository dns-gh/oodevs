//*****************************************************************************
//
// $Created: FBD 02-11-21 $
// $Archive: /MVW_v10/Build/SDK/MOS_Light/src/MOS_Patch.cpp $
// $Author: Nld $
// $Modtime: 10/09/04 11:40 $
// $Revision: 11 $
// $Workfile: MOS_Patch.cpp $
//
//*****************************************************************************

#include "MOS_Pch.h"

#include "MOS_Patch.h"

#ifndef MOS_USE_INLINE
#	include "MOS_Patch.inl"
#endif

#include "MOS_ShapePatch.h"
#include "MOS_GLWidget.h"
#include "MOS_World.h"
#include "MOS_App.h"
#include "MOS_MainWindow.h"
#include "MOS_WorldWidget.h"
#include "MOS_Object.h"

#include <QImage.h>

//-----------------------------------------------------------------------------
// Name: MOS_Patch constructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
MOS_Patch::MOS_Patch()
    : rect_      ()
    , nIdxPatchX_( 0 )
    , nIdxPatchY_( 0 )
    , nFrame_    ( 0 )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_Patch destructor
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
MOS_Patch::~MOS_Patch()
{
}

//=============================================================================
// INITIALIZE
//=============================================================================

//-----------------------------------------------------------------------------
// Name: MOS_Patch::InitializeGL
// Created: NLD 2002-11-28
//-----------------------------------------------------------------------------
void MOS_Patch::InitializeGL()
{
    glRect_ = glGenLists( 1 );

    glNewList( glRect_, GL_COMPILE );
    GFX_Color color( 255, 255, 255, 0.1 );
    GFX_Tools::CreateGLRect( rect_, 1.0, color );
	glEndList();    

}


//=============================================================================
// DRAW
//=============================================================================

//-----------------------------------------------------------------------------
//  Name  :  SCP_GLPatch::IsOnScreen
// Created: FBD 02-07-04
//-----------------------------------------------------------------------------
bool MOS_Patch::IsOnScreen()
{
	GLdouble	modelMatrix[16], projMatrix[16];
	GLint		viewport[4];

    glPushMatrix ();

	glGetDoublev ( GL_MODELVIEW_MATRIX, modelMatrix ); 
	glGetDoublev ( GL_PROJECTION_MATRIX, projMatrix ); 
	glGetIntegerv( GL_VIEWPORT, viewport); 

	GLdouble rZ;
	GLdouble rX1PatchdProj, rY1PatchdProj; 
	GLdouble rX2PatchdProj, rY2PatchdProj;

	gluProject( rect_.GetLeft (), rect_.GetBottom(), 0.0, modelMatrix, projMatrix, viewport, &rX1PatchdProj, &rY1PatchdProj, &rZ );
	gluProject( rect_.GetRight(), rect_.GetTop   (), 0.0, modelMatrix, projMatrix, viewport, &rX2PatchdProj, &rY2PatchdProj, &rZ );
	
	glPopMatrix();

    MOS_GLWidget& glWidget = MOS_App::GetApp().GetMainWindow().GetWorldWidget().GetGLWidget();

	if ( rX2PatchdProj < 0 || rX1PatchdProj > glWidget.width())
        return false;
	
	if ( rY2PatchdProj < 0 || rY1PatchdProj > glWidget.height() )
        return false;

    return true;
}

//-----------------------------------------------------------------------------
// Name: MOS_Patch::Draw
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void MOS_Patch::DrawAltitudes()
{
    if( !IsOnScreen() )
        return;

    glCallList( nGLPatch_ );
}


//-----------------------------------------------------------------------------
// Name: MOS_Patch::Draw
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void MOS_Patch::DrawObjects( uint nFrame, MT_Float rDistPercent )
{
    if( !IsOnScreen() )
        return;

    nFrame_ = nFrame;

    for( IT_ShapePatchVector itShapePatch = shapePatchVector_.begin(); itShapePatch != shapePatchVector_.end(); ++itShapePatch )
    {
        MOS_ShapePatch& shapePatch = **itShapePatch;
        if( shapePatch.IsItLine() )
            shapePatch.Draw( nFrame, rDistPercent );
    }

    for( itShapePatch = shapePatchVector_.begin(); itShapePatch != shapePatchVector_.end(); ++itShapePatch )
    {
        MOS_ShapePatch& shapePatch = **itShapePatch;
        if( shapePatch.IsItArea() )
            shapePatch.Draw( nFrame, rDistPercent );
    }

    for( itShapePatch = shapePatchVector_.begin(); itShapePatch != shapePatchVector_.end(); ++itShapePatch )
    {
        MOS_ShapePatch& shapePatch = **itShapePatch;
        if( shapePatch.IsItPoint() )
            shapePatch.Draw( nFrame, rDistPercent );
    }

}

//-----------------------------------------------------------------------------
// Name: MOS_Patch::Draw
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void MOS_Patch::DrawBorders()
{
    if( !IsOnScreen() )
        return;

    glCallList( glRect_ );
}


//=============================================================================
// IN/OUT
//=============================================================================

//-----------------------------------------------------------------------------
//  Name  :  MOS_Patch::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
void MOS_Patch::Read( MT_InputArchive_ABC& archive )
{
    rect_.Read( archive );
    archive.ReadField( "nIdxPatchX_", nIdxPatchX_ );
    archive.ReadField( "nIdxPatchY_", nIdxPatchY_ );

    InitializeGL();
}


//-----------------------------------------------------------------------------
// Name: MOS_Patch::Draw
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void MOS_Patch::DrawNameObjects( uint nFrame, MT_Float rDistPercent )
{
    if ( nFrame_ != nFrame )
        return;

    for( IT_ShapePatchVector itShapePatch = shapePatchVector_.begin(); itShapePatch != shapePatchVector_.end(); ++itShapePatch )
    {
        MOS_ShapePatch& shapePatch = **itShapePatch;
        MOS_Object& object = shapePatch.GetObjectShape();
        if ( object.GetFrame() == nFrame )
            continue;

        object.SetFrame( nFrame );
        object.DrawObjectsName( rDistPercent );
    }
}
