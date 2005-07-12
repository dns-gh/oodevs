//*****************************************************************************
// Name   : MOS_ShapePatch.cpp
// Created: FBD 02-07-01
//*****************************************************************************

#include "MOS_pch.h"
#include "MOS_ShapePatch.h"

#include "MOS_Patch.h"
#include "MOS_ShapePatch.h"
#include "MOS_World.h"
#include "MOS_ObjectManager.h"
#include "MOS_Tri.h"
#include "MOS_Line.h"
#include "MOS_App.h"
#include "MOS_Object.h"


//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch constructor
// Created: FBD 03-05-23
//-----------------------------------------------------------------------------
MOS_ShapePatch::MOS_ShapePatch()
    : pObject_              ( 0 )
    , pPatch_               ( 0 )
    , nodePtrVector_        ()
    , linePtrVector_        ()
    , triPtrVector_         ()
{
    
}



//-----------------------------------------------------------------------------
//  Name  :  MOS_ShapePatch constructor
// Created: FBD 02-11-18
//-----------------------------------------------------------------------------
MOS_ShapePatch::MOS_ShapePatch( MOS_Object& object, MOS_Patch& patch )
    : pObject_              ( &object )
    , pPatch_               ( &patch )
    , nodePtrVector_        ()
    , linePtrVector_        ()
    , triPtrVector_         ()
{
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_ShapePatch destructor
// Created: FBD 02-07-02
//-----------------------------------------------------------------------------
MOS_ShapePatch::~MOS_ShapePatch()
{
}

//=============================================================================
// IN/OUT
//=============================================================================

//-----------------------------------------------------------------------------
//  Name  :  MOS_ShapePatch::Read
// Created: FBD 02-09-05
//-----------------------------------------------------------------------------
int MOS_ShapePatch::Read( MT_InputArchive_ABC& archive )
{
    MOS_ObjectManager& objectManager = MOS_App::GetApp().GetObjectManager();

    // Bouding box
    MT_Float rValue;
    archive.ReadField( "rXMin_", rValue );
    archive.ReadField( "rXMax_", rValue );
    archive.ReadField( "rYMin_", rValue );
    archive.ReadField( "rYMax_", rValue );

    // Nodes
    uint nNodes = 0;
    archive.ReadField( "nNodes", nNodes );
    for( uint i = 0 ; i < nNodes ; ++i )
    {
        uint nNode = 0;
        archive.ReadField( "nNode", nNode );
        if ( GetObjectShape().IsItPoint() )
        {
            MOS_Node* pNode = objectManager.GetNodeFromIdx( nNode );
            assert( pNode );
            nodePtrVector_.push_back( pNode );
        }
    }

    // Lines
    uint nLines = 0;
    archive.ReadField( "nLines", nLines );
    for( i = 0 ; i < nLines ; ++i )
    {
        uint nLine = 0;
        archive.ReadField( "nLine", nLine );
        MOS_Line* pLine = objectManager.GetLineFromIdx( nLine );
        assert( pLine );
        linePtrVector_.push_back( pLine );
    }

    // Tris
    uint nTris = 0;
    archive.ReadField( "nTris", nTris );
    for( i = 0 ; i < nTris ; ++i )
    {
        uint nTri = 0;
        archive.ReadField( "nTri", nTri );
        MOS_Tri* pTri = objectManager.GetTriFromIdx( nTri );
        assert( pTri );
        triPtrVector_.push_back( pTri );

    }
    return 0;
}

//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch::Draw
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------

void MOS_ShapePatch::Draw( uint nFrame, MT_Float rDistPercent )
{
    MT_Float rMeterPerPixel = MOS_App::GetApp().GetWorld().GetExtent().GetMeterPerPixel();

    MT_Float rDistLossyTri = ( 100. - rDistPercent ) * 5. / rMeterPerPixel;
    rDistLossyTri *= rDistLossyTri;

    assert( pObject_ );
    GFX_Color baseColor = GFX_Tools::GetColorForObjectType( pObject_->GetType() );
    baseColor.SetAlpha( 0.8 );  

    if ( pObject_->GetType() == eTypeTerrainNothing  )
    {
//        baseColor.SetAlpha( 0.5 );
//        CreateTrianglesGL( baseColor, 0., nFrame );
        return;
    }

    if ( IsItArea() )
    {
        if ( GetObjectShape().GetType() == eTypeTerrainUrban )
        {
            if ( rDistPercent > 50. )
            {
                if  ( GetObjectShape().GetValue() != 0. )
                {
                    baseColor.SetAlpha( 0.25 );
                    CreateTrianglesGL( baseColor, rDistLossyTri, nFrame );

                }

                if ( rDistPercent > 70. )
                {
                    baseColor.SetAlpha( 0.25 );
                    CreateTrianglesGL( baseColor, rDistLossyTri, nFrame );
    
                }

                if ( rDistPercent > 90. )
                {
                    baseColor.SetAlpha( 1.0 );
                    if ( linePtrVector_.size() )
                        CreateBorderLinesGl( baseColor, nFrame );
                }
            }        
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainForest ||  GetObjectShape().GetType() == eTypeTerrainForestBig )
        {
            if ( rDistPercent > 70. )
            {
                baseColor.SetAlpha( 0.25 );
                CreateTrianglesGL( baseColor, rDistLossyTri, nFrame );
                baseColor.SetAlpha( 1.0 );
                if ( linePtrVector_.size() )
                    CreateBorderLinesGl( baseColor, nFrame );
            }
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainFleuve || GetObjectShape().GetType() == eTypeTerrainLake  )
        {
//            if ( rDistPercent > 30. )
            {
                baseColor.SetAlpha( 0.55 );
                CreateTrianglesGL( baseColor, rDistLossyTri, nFrame );
                baseColor.SetAlpha( 1.0 );
                if ( rDistPercent > 80. )
                {
                    if ( linePtrVector_.size() )
                        CreateBorderLinesGl( baseColor, nFrame );
                }
            }
/*            else
            {
                baseColor.SetAlpha( 1.0 );
                if ( linePtrSet_.size() )
                    CreateBorderLinesGl( baseColor, nFrame );
            }
*/
                
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainPlantation  )
        {
            if ( rDistPercent > 70. )
            {
                baseColor.SetAlpha( 0.25 );
                CreateTrianglesGL( baseColor, rDistLossyTri, nFrame );
                baseColor.SetAlpha( 1.0 );
                if ( linePtrVector_.size() )
                    CreateBorderLinesGl( baseColor, nFrame );
            }
        }
        else
        {
                baseColor.SetAlpha( 0.55 );
            CreateTrianglesGL( baseColor, rDistLossyTri, nFrame );
            baseColor.SetAlpha( 1.0 );
            if ( linePtrVector_.size() )
                CreateBorderLinesGl( baseColor, nFrame );
        }

    }
    else
    if ( IsItLine() )
    {
        MT_Float rSize = 1.;
        rSize = GetObjectShape().GetValue();
        if ( GetObjectShape().GetType() == eTypeTerrainRoadAutoroute )
        {
            CreateLinesGlAutoroute( baseColor, rSize, nFrame );
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainRoadNational )
        {
            CreateLinesGl( baseColor, rSize, nFrame );
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainRoadDepartemental )
        {
            if ( rDistPercent > 60. )
            {
                GFX_Color color( 255 ,255., 0., 1. );
                CreateLinesGl( color, rSize, nFrame );
            }
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainRoadChemin )
        {
            if ( rDistPercent > 70. )
            {
                GFX_Color colorChemin( 255 ,255., 0., 1. );
                CreateLinesGl( colorChemin, rSize, nFrame );
            }
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainRiverRuisseau )
        {
            if ( rDistPercent > 60. )
            {
                rSize = GetObjectShape().GetValue();
                rSize /= 5.;
                CreateLinesGl( baseColor, rSize , nFrame );
            }
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainRiver )
        {
            if ( rDistPercent > 60. )
            {
                rSize = GetObjectShape().GetValue();
                rSize /= 5.;
                CreateLinesGl( baseColor, rSize , nFrame );
            }
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainRiverBig )
        {
            if ( rDistPercent > 60. )
            {
                rSize = GetObjectShape().GetValue();
                rSize /= 5.;
                CreateLinesGl( baseColor, rSize , nFrame );
            }
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainRailRoad )
        {
            if ( rDistPercent > 60. )
            {
                CreateLinesGlRailRoad( baseColor, nFrame );
            }
        }
        else
        if ( GetObjectShape().GetType() == eTypeTerrainCrest )
        {
            if ( rDistPercent > 60. )
                CreateLinesGl( baseColor, 1.0, nFrame );

        }
        else
            CreateLinesGl( baseColor, rSize, nFrame );
    }
    else
    if ( GetObjectShape().GetType() == eTypeTerrainCrossRoad && nodePtrVector_.size() )
    {
        if ( rDistPercent > 90. )
        {
            for ( IT_NodePtrVector itNode = nodePtrVector_.begin() ; itNode != nodePtrVector_.end() ; ++itNode )
            {
                MOS_Node& node = **itNode;
              GFX_Tools::CreateGLCircle( node.GetPos(), 100. / rMeterPerPixel, baseColor );
            }
        }
    }
    else
    if ( GetObjectShape().GetType() == eTypeTerrainBridge && nodePtrVector_.size() )
    {
        if ( rDistPercent > 90. )
        {
            for ( IT_NodePtrVector itNode = nodePtrVector_.begin() ; itNode != nodePtrVector_.end() ; ++itNode )
            {
                MT_Float rSize = GetObjectShape().GetValue();
                MOS_Node& node = **itNode;
                GFX_Tools::CreateGLCircle( node.GetPos(), rSize / rMeterPerPixel, baseColor );
            }
        }
    }
    else
    if ( GetObjectShape().GetType() == eTypeTerrainInterchange && nodePtrVector_.size() )
    {
      if ( rDistPercent > 90. )
        {

            for ( IT_NodePtrVector itNode = nodePtrVector_.begin() ; itNode != nodePtrVector_.end() ; ++itNode )
            {
                MOS_Node& node = **itNode;
                GFX_Tools::CreateGLCircle( node.GetPos(), 10. / rMeterPerPixel, baseColor );
            }
        }
    }


}



//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch::CreateLinesGlAutoroute
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
void MOS_ShapePatch::CreateLinesGlAutoroute( GFX_Color& color, MT_Float rSize, uint nFrame )
{
    GFX_Color colorInside( 255 ,255., 0., 1. );
    

    for ( CIT_MOSLinePtrVector itLine = linePtrVector_.begin() ; itLine != linePtrVector_.end() ; ++itLine )
    {
        MOS_Line& line = **itLine;

        if ( line.GetFrame() == nFrame )
            continue;

        line.SetFrame( nFrame );
        MT_Line lineGFX = line.GetLine();
        GFX_Tools::CreateGLLine( lineGFX, rSize + 1.0 , color );
        GFX_Tools::CreateGLLine( lineGFX, 2.0 , colorInside );
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch::CreateLinesGlRailRoad
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
void MOS_ShapePatch::CreateLinesGlRailRoad( GFX_Color& color, uint nFrame )
{
    GFX_Color colorInside( 128 ,128., 0., 1. );

//    MT_Float rMeterPerPixel = MOS_App::GetApp().GetWorld().GetExtent().GetMeterPerPixel();
    for ( CIT_MOSLinePtrVector itLine = linePtrVector_.begin() ; itLine != linePtrVector_.end() ; ++itLine )
    {
        MOS_Line& line = **itLine;

        if ( line.GetFrame() == nFrame )
            continue;

        GFX_Tools::CreateGLLine( line.GetLine(), 2. , colorInside );
        line.SetFrame( nFrame );

        /*
        MT_Line lineGFX = line.GetLine();
        GFX_Tools::CreateGLLine( lineGFX, 2. , color );
        MT_Float rLenLine = lineGFX.GetPosEnd().Distance( lineGFX.GetPosStart() );
        MT_Vector2D vPente( MT_Vector2D( lineGFX.GetPosEnd() - lineGFX.GetPosStart() ).Normalize() );

        MT_Vector2D vCote1( MT_Vector2D( -vPente.rY_, vPente.rX_ ) * ( 30. / rMeterPerPixel ) );
        MT_Vector2D vCote2( MT_Vector2D( vPente.rY_, -vPente.rX_ ) * ( 30. / rMeterPerPixel ) );

        for ( MT_Float rLen = 0. ; rLen < rLenLine ; rLen += 40. / rMeterPerPixel )
        {
            MT_Vector2D vPos( lineGFX.GetPosStart() + ( vPente * rLen ) );
            MT_Line line( vPos + vCote1, vPos + vCote2  );
            GFX_Tools::CreateGLLine( line, 2. , color );
        }
*/
    }
}

//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch::CreateLinesGl
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
void MOS_ShapePatch::CreateLinesGl( GFX_Color& color, MT_Float rSize, uint nFrame )
{
    for ( CIT_MOSLinePtrVector itLine = linePtrVector_.begin() ; itLine != linePtrVector_.end() ; ++itLine )
    {
        MOS_Line& line = **itLine;

        if ( line.GetFrame() == nFrame )
            continue;

        line.SetFrame( nFrame );
        MT_Line lineGFX = line.GetLine();
        GFX_Tools::CreateGLLine( lineGFX, rSize, color );
    }

}


//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch::CreateLinesGl
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
void MOS_ShapePatch::CreateBorderLinesGl( GFX_Color& color, uint nFrame )
{
    for ( CIT_MOSLinePtrVector itLine = linePtrVector_.begin() ; itLine != linePtrVector_.end() ; ++itLine )
    {
        MOS_Line& line = **itLine;
        if ( line.GetFrame() == nFrame )
            continue;

        line.SetFrame( nFrame );
        MT_Line lineGFX = line.GetLine();
        GFX_Tools::CreateGLLine( lineGFX, 1.0, color );
    }
}

//-----------------------------------------------------------------------------
//  Name  :  MOS_ShapePatch::CreateTrianglesGL
// Created: FBD 02-08-26
//-----------------------------------------------------------------------------
void MOS_ShapePatch::CreateTrianglesGL( GFX_Color& color, MT_Float rDistLossy, uint nFrame )
{
    for ( CIT_TriPtrVector itTriangle = triPtrVector_.begin() ; itTriangle != triPtrVector_.end() ; ++itTriangle )
    {
        MOS_Tri& tri = **itTriangle;
        if ( tri.GetFrame() == nFrame )
           continue;

/*      if ( object_.GetType() == eTypeTerrainNothing  )
        {
           color.SetRed( (MT_Float)( tri.GetIdx() % 255 ) / 1. );
           color.SetGreen( (MT_Float)( tri.GetIdx() % 255 ) / 1.  );
           color.SetBlue( (MT_Float)( tri.GetIdx() % 255 ) / 1. );
        }*/

        if ( tri.GetSize() > rDistLossy )
        {
            tri.SetFrame( nFrame );
            GFX_Tools::CreateGLTriangle( tri.GetPos1().GetPos(),tri.GetPos2().GetPos(), tri.GetPos3().GetPos(), color );
        }
    }
}


//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch::CreateLinesTrianglesGL
// Created: FBD 02-11-22
//-----------------------------------------------------------------------------
void MOS_ShapePatch::CreateLinesTrianglesGL( GFX_Color& color )
{
    for ( CIT_TriPtrVector itTriangle = triPtrVector_.begin() ; itTriangle != triPtrVector_.end() ; ++itTriangle )
    {
        const MOS_Tri& triangle = **itTriangle;
        MT_Line line1( triangle.GetPos1().GetPos(), triangle.GetPos2().GetPos() );
        MT_Line line2( triangle.GetPos2().GetPos(), triangle.GetPos3().GetPos() );
        MT_Line line3( triangle.GetPos3().GetPos(), triangle.GetPos1().GetPos() );
        GFX_Tools::CreateGLLine( line1, 8.0, color );
    }
}

    MOS_Object*             pGetObjectShape();
    MOS_Patch*              pPatch_;
    T_NodePtrVector         nodePtrVector_;
    T_MOSLinePtrVector      linePtrVector_;
    T_TriPtrVector          triPtrVector_;

//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch constructor
// Created: FBD 03-05-23
//-----------------------------------------------------------------------------
MOS_ShapePatch::MOS_ShapePatch( const MOS_ShapePatch& rhs )
:   pObject_        ( rhs.pObject_ )
,   pPatch_         ( rhs.pPatch_ )
,   nodePtrVector_  ( rhs.nodePtrVector_ )
,   linePtrVector_  ( rhs.linePtrVector_ )
,   triPtrVector_   ( rhs.triPtrVector_ )
{
}


//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch::=( const 
// Created: FBD 03-05-23
//-----------------------------------------------------------------------------
MOS_ShapePatch& MOS_ShapePatch::operator = ( const MOS_ShapePatch& rhs )
{
    pObject_        = rhs.pObject_;
    pPatch_         = rhs.pPatch_;
    nodePtrVector_  = rhs.nodePtrVector_;
    linePtrVector_  = rhs.linePtrVector_;
    triPtrVector_   = rhs.triPtrVector_;
    return *this;
}


//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch::IsItArea
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
bool MOS_ShapePatch::IsItArea() const
{
    return GetObjectShape().IsItArea();
}


//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch::IsItLine
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
bool MOS_ShapePatch::IsItLine() const
{
    return GetObjectShape().IsItLine();
}

//-----------------------------------------------------------------------------
// Name: MOS_ShapePatch::IsItPoint
// Created: FBD 03-03-11
//-----------------------------------------------------------------------------
bool MOS_ShapePatch::IsItPoint() const
{
    return GetObjectShape().IsItPoint();
}


