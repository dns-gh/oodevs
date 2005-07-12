// If SINGLE is defined when triangle.o is compiled, it should also be defined here.  If not, it should not be defined here.
// #define SINGLE

#include "TER_pch.h"

#include "triangle.h"
#include "TriCall.h"
#include "MT_Line.h"
#include "MT_Triangle.h"
#include "MT/MT_Logger/MT_LogManager.h"
#include "MT/MT_Logger/MT_LogDefines.h"

/*
#include <functional>
#include <algorithm>
#include <vector>
#include <stdio.h>


#ifndef _STDLIB_H_
extern void *malloc();
extern void free();
#endif // _STDLIB_H_
*/

#define MULPRECISION	(MT_Float)8.0    
#define DECALPREC		(MT_Float)1000.0
#define PRECISION		(MT_Float)0.000001

void triangulate(char *, struct triangulateio *, struct triangulateio *, struct triangulateio *);
bool GetHoleCenterPoint( const T_PointPtrVector& hole, MT_Vector2D& vPos );
void DoTriangulate( char* sCommand, const T_RingVector& rings, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines, bool bOutSide );
void DoTriangulate( char* sCommand, const T_PointVector& ring, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines, bool bOutSide );
void DoTriangulate(char *, struct triangulateio&, struct triangulateio&, struct triangulateio& );
void DoTriangulateCom( char* sCommand, const T_RingVector& rings, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines, bool bOutSide );
void DoTriangulateCom( char* sCommand, const T_PointVector& ring, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines, bool bOutSide );
void DoTriangulateLine( char* sCommand, const T_LineVector& lines, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines );
void DoTriangulateLineInternal( char* sCommand, const T_LineVector& lines, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines );
void CleanAll( struct triangulateio& in, struct triangulateio& mid, struct triangulateio& vorout );


/*****************************************************************************/
/*                                                                           */
/*  main()   Create and refine a mesh.                                       */
/*                                                                           */
/*****************************************************************************/

void DoTriangulate( const T_RingVector& rings, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines, bool bOutSide )
{
    char sCommand[256];
    if ( !bOutSide )
    {
        strcpy( sCommand, "pQ" );
    }
    else
    {
        strcpy(  sCommand, "pcQ" );
    }
    DoTriangulate( sCommand, rings, vListPoints, vListTriangles, vListLines, bOutSide );
}

void DoTriangulate( const T_PointVector& ring, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines, bool bOutSide )
{
    char sCommand[256];
    if ( !bOutSide )
    {
        strcpy( sCommand, "pQ" );
    }
    else
    {
        strcpy(  sCommand, "pcQ" );
    }
    DoTriangulate( sCommand, ring, vListPoints, vListTriangles, vListLines, bOutSide );
}

void DoTriangulateLine( char* sCommand, const T_LineVector& lines, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines )
{
    try
    {
        DoTriangulateLineInternal( sCommand, lines, vListPoints, vListTriangles, vListLines );
    }
    catch( std::exception& e )
    {
        MT_LOG_FATAL_ERROR_MSG( "Exception caught in triangulation algorithm : " << e.what() );
        assert( false );
    }
    catch( ... )
    {
        _clearfp();
        MT_LOG_FATAL_ERROR_MSG( "Unknown exception caught in triangulation algorithm" );
        assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: DoTriangulate
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void DoTriangulate( char* sCommand, const T_PointVector& ring, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines, bool bOutSide )
{
    try
    {
        DoTriangulateCom( sCommand, ring, vListPoints, vListTriangles, vListLines, bOutSide );
    }
    catch( std::exception& e )
    {
        MT_LOG_FATAL_ERROR_MSG( "Exception caught in triangulation algorithm : " << e.what() );
        assert( false );
    }
    catch( ... )
    {
        _clearfp();
        MT_LOG_FATAL_ERROR_MSG( "Unknown exception caught in triangulation algorithm" );
        assert( false );
    }
}

// -----------------------------------------------------------------------------
// Name: DoTriangulate
// Created: AGE 2005-03-07
// -----------------------------------------------------------------------------
void DoTriangulate( char* sCommand, const T_RingVector& rings, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines, bool bOutSide )
{
    try
    {
        DoTriangulateCom( sCommand, rings, vListPoints, vListTriangles, vListLines, bOutSide );
    }
    catch( std::exception& e )
    {
        MT_LOG_FATAL_ERROR_MSG( "Exception caught in triangulation algorithm : " << e.what() );
        assert( false );
    }
    catch( ... )
    {
        _clearfp();
        MT_LOG_FATAL_ERROR_MSG( "Unknown exception caught in triangulation algorithm" );
        assert( false );
    }
}

template< typename T >
void Clean( T*& p )
{
    if( p )
    {
        free( p );
        p = 0;
    }
}

// -----------------------------------------------------------------------------
// Name: CleanAll
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
void CleanAll( struct triangulateio& in, struct triangulateio& mid, struct triangulateio& vorout )
{
    // Free all allocated arrays, including those allocated by Triangle.
	Clean( in.pointlist );
	Clean( in.pointattributelist );
	Clean( in.pointmarkerlist );
    Clean( in.trianglelist );
	Clean( in.triangleattributelist );
	Clean( in.triangleaMT_Floatist );
	Clean( in.neighborlist );
	Clean( in.segmentlist );
	Clean( in.segmentmarkerlist );
	Clean( in.edgelist );
	Clean( in.edgemarkerlist );

    Clean( mid.pointlist );
	Clean( mid.pointattributelist );
	Clean( mid.pointmarkerlist );
	Clean( mid.trianglelist );
	Clean( mid.triangleattributelist );
	Clean( mid.triangleaMT_Floatist );
	Clean( mid.neighborlist );
	Clean( mid.segmentlist );
	Clean( mid.segmentmarkerlist );
	Clean( mid.edgelist );
	Clean( mid.edgemarkerlist );

	Clean( vorout.pointlist );
	Clean( vorout.pointattributelist );
	Clean( vorout.pointmarkerlist );
    Clean( vorout.trianglelist );
	Clean( vorout.triangleattributelist );
	Clean( vorout.triangleaMT_Floatist );
	Clean( vorout.neighborlist );
	Clean( vorout.segmentlist );
	Clean( vorout.segmentmarkerlist );
	Clean( vorout.edgelist );
	Clean( vorout.edgemarkerlist );
}

// -----------------------------------------------------------------------------
// Name: DoTriangulate
// Created: AGE 2005-03-08
// -----------------------------------------------------------------------------
void DoTriangulate(char * sCommand, struct triangulateio& in, struct triangulateio& mid, struct triangulateio& vorout)
{
    try
    {
        triangulate(sCommand, &in, &mid, &vorout);
    }
    catch( ... )
    {
        _clearfp();
        CleanAll( in, mid, vorout );
        throw;
    }
}

//-----------------------------------------------------------------------------
// Name: tricall::DoTriangulateLineInternal
// Created: FBD 03-05-21
//-----------------------------------------------------------------------------
void DoTriangulateLineInternal( char* sCommand, const T_LineVector& lines, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines )
{
	uint i;
    
	struct triangulateio in, mid, vorout;
	
	memset( (void*)&in, 0, sizeof ( struct triangulateio ) );
	memset( (void*)&mid, 0, sizeof ( struct triangulateio ) );
	memset( (void*)&vorout, 0, sizeof ( struct triangulateio ) );

	int nTotalPoint = 0;
	int nTotalSegment = 0;
    nTotalPoint = lines.size() * 2;
    nTotalSegment = lines.size();

	in.numberofpoints = 0;
	in.pointlist = (MT_Float *) malloc( nTotalPoint * 2 * sizeof(MT_Float));
	in.numberofpointattributes = 1;
	in.pointattributelist = (MT_Float *) malloc( nTotalPoint * in.numberofpointattributes * sizeof(MT_Float));
	memset( (void*)in.pointattributelist, 0 , nTotalPoint * in.numberofpointattributes * sizeof(MT_Float) );

	in.pointmarkerlist = (int *) malloc( nTotalPoint * in.numberofpointattributes * sizeof(MT_Float));
	memset( (void*)in.pointmarkerlist, 0 , nTotalPoint * in.numberofpointattributes * sizeof(MT_Float) );
	in.numberofholes = 0;
	
    in.segmentlist = (int *) malloc( nTotalSegment * 2 * sizeof(int));
	in.segmentmarkerlist = (uint*) malloc( nTotalSegment * sizeof(uint));
	memset( (void*)in.segmentmarkerlist, 0 , nTotalSegment * sizeof(int) );

	int nIdxSegment = 0;
    for ( i = 0 ; i < nTotalSegment ; ++i )
	{
        MT_Float rX, rY;
        rX = ( lines[i].GetPosStart().rX_ * MULPRECISION ) + DECALPREC;
		rY = ( lines[i].GetPosStart().rY_ * MULPRECISION ) + DECALPREC;
        in.pointlist[ i * 4 + 0 + 0 ] = rX;
		in.pointlist[ i * 4 + 0 + 1 ] = rY;
        rX = ( lines[i].GetPosEnd().rX_ * MULPRECISION ) + DECALPREC;
		rY = ( lines[i].GetPosEnd().rY_ * MULPRECISION ) + DECALPREC;
        in.pointlist[ i * 4 + 2 + 0 ] = rX;
		in.pointlist[ i * 4 + 2 + 1 ] = rY;
		in.pointattributelist[i] = DEFATTRFREE;
		in.pointmarkerlist[i] = DEFATTRFREE;
		in.numberofpoints += 2;
	}
    
    for ( i = 0 ; i < nTotalSegment ; ++i )
	{
		in.segmentlist[i * 2 + 0] = i * 2 + 1;
		in.segmentlist[i * 2 + 1] = i * 2 + 2;
		in.segmentmarkerlist[i] = DEFATTRFREE;
		in.numberofsegments++;
	}
    
    // $$$ JOSS memory leak sinon!!!
    // Voir avec fabrice
    Clean( in.pointmarkerlist );

	in.pointmarkerlist = (int *) malloc( in.numberofpoints * sizeof(int) );
	for ( i = 0 ; i < in.numberofpoints ; ++i )
	{
		in.pointmarkerlist[i] = i;
	}

	// Make necessary initializations so that Triangle can return a triangulation in `mid' and a voronoi diagram in `vorout'.

	mid.pointlist = (MT_Float *) NULL;				// Not needed if -N switch used.

	// Not needed if -N switch used or number of point attributes is zero:
	mid.pointattributelist = (MT_Float *) NULL;
	mid.pointmarkerlist = (int *) NULL;			// Not needed if -N or -B switch used.
	mid.trianglelist = (int *) NULL;			// Not needed if -E switch used.
	
	// Not needed if -E switch used or number of triangle attributes is zero:
	mid.triangleattributelist = (MT_Float *) NULL;
	mid.neighborlist = (int *) NULL;			// Needed only if -n switch used.
	
	// Needed only if segments are output (-p or -c) and -P not used:
	mid.segmentlist = (int *) NULL;

	// Needed only if segments are output (-p or -c) and -P and -B not used:
	mid.segmentmarkerlist = (uint*) NULL;
	mid.edgelist = (int *) NULL;				// Needed only if -e switch used.
	mid.edgemarkerlist = (int *) NULL;			// Needed if -e used and -B not used.

	vorout.pointlist = (MT_Float *) NULL;			// Needed only if -v switch used.
	
	// Needed only if -v switch used and number of attributes is not zero:
	vorout.pointattributelist = (MT_Float *) NULL;
	vorout.edgelist = (int *) NULL;				// Needed only if -v switch used.
	vorout.normlist = (MT_Float *) NULL;			// Needed only if -v switch used.

	DoTriangulate(sCommand, in, mid, vorout);		//qa0.01

	MT_Vector2D vPos( 0., 0. );

	// On stock les points
	for ( i = 0 ; i < mid.numberofpoints ; ++i )
	{
		MT_Float rX1 = mid.pointlist[ i * 2 + 0 ];
		MT_Float rY1 = mid.pointlist[ i * 2 + 1 ];
		vPos.rX_ = ( rX1 - DECALPREC ) / MULPRECISION;
		vPos.rY_ = ( rY1 - DECALPREC ) / MULPRECISION;
        vListPoints.push_back( new MT_Vector2D( vPos ) );
	}

	// On stock les triangles
	for ( i = 0 ; i < mid.numberoftriangles ; ++i )
	{
		
        MT_Vector2D& point1 = *vListPoints[ mid.trianglelist[i * 3 + 0]-1];
		MT_Vector2D& point2 = *vListPoints[ mid.trianglelist[i * 3 + 1]-1];
		MT_Vector2D& point3 = *vListPoints[ mid.trianglelist[i * 3 + 2]-1];
		vListTriangles.push_back( new MT_Triangle( point1, point2, point3 ) );
	}
 
	// On stock le squelette
	for ( i = 0 ; i < mid.numberofsegments ; ++i )
	{
		if ( mid.segmentmarkerlist[i] == DEFATTRSQUELETTE  )
		{
			MT_Vector2D& point1 = *vListPoints[ mid.segmentlist[i * 2 + 0]-1];
			MT_Vector2D& point2 = *vListPoints[ mid.segmentlist[i * 2 + 1]-1];
			vListLines.push_back( new MT_Line( point1, point2 ) );
		}
	}
	
    CleanAll( in, mid, vorout );
}


//-----------------------------------------------------------------------------
// Name: tricall::DoTriangulateCom
// Created: FBD 03-05-06
//-----------------------------------------------------------------------------
void DoTriangulateCom( char* sCommand, const T_RingVector& rings, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines, bool bOutSide )
{
	uint i, j;
    
    const T_PointPtrVector& polygon = rings[0];

	struct triangulateio in, mid, vorout;
	
	memset( (void*)&in, 0, sizeof ( struct triangulateio ) );
	memset( (void*)&mid, 0, sizeof ( struct triangulateio ) );
	memset( (void*)&vorout, 0, sizeof ( struct triangulateio ) );

    uint nNbrHoles = 0;

    if ( !bOutSide )
        nNbrHoles = rings.size() - 1;

    if ( nNbrHoles )
    {
        in.holelist = (MT_Float*) malloc( nNbrHoles * 2 * sizeof(MT_Float));
    }

	int nTotalPoint = 0;
	int nTotalSegment = 0;
	for ( i = 0 ; i < rings.size() ; ++i )
	{
		nTotalPoint += rings[i].size();
		nTotalSegment += rings[i].size();
	}

	in.numberofpoints = 0;
	in.pointlist = (MT_Float *) malloc( nTotalPoint * 2 * sizeof(MT_Float));
	in.numberofpointattributes = 1;
	in.pointattributelist = (MT_Float *) malloc( nTotalPoint * in.numberofpointattributes * sizeof(MT_Float));
	memset( (void*)in.pointattributelist, 0 , nTotalPoint * in.numberofpointattributes * sizeof(MT_Float) );

	in.pointmarkerlist = (int *) malloc( nTotalPoint * in.numberofpointattributes * sizeof(MT_Float));
	memset( (void*)in.pointmarkerlist, 0 , nTotalPoint * in.numberofpointattributes * sizeof(MT_Float) );
	
	int nBeginPoint = 0;
	int nIdxPoint = 0;
	for ( i = nBeginPoint ; i < nBeginPoint + polygon.size() ; ++i )
	{
        MT_Float rX, rY;
        rX = ( polygon[nIdxPoint]->rX_ * MULPRECISION ) + DECALPREC;
		rY = ( polygon[nIdxPoint]->rY_ * MULPRECISION ) + DECALPREC;
        in.pointlist[ i * 2 + 0 ] = rX;
		in.pointlist[ i * 2 + 1 ] = rY;
		in.pointattributelist[i] = DEFATTRSQUELETTE;
		in.pointmarkerlist[i] = DEFATTRSQUELETTE;
		in.numberofpoints++;
		nIdxPoint++;
	}
    nBeginPoint += polygon.size();


    in.numberofsegments = 0;
	int nBeginSegment = 0;
    if ( !bOutSide )
    {
        in.segmentlist = (int *) malloc( nTotalSegment * 2 * sizeof(int));
	    in.segmentmarkerlist = (uint*) malloc( nTotalSegment * sizeof(uint));
	    memset( (void*)in.segmentmarkerlist, 0 , nTotalSegment * sizeof(int) );

	    for ( i = nBeginSegment ; i < nBeginSegment + polygon.size(); ++i )
	    {
		    in.segmentlist[i * 2 + 0] = i + 1;
            if ( i == nBeginSegment + polygon.size() - 1 )
            {
                in.segmentlist[i * 2 + 1] = nBeginSegment + 1;
            }
            else
            {
                in.segmentlist[i * 2 + 1] = i + 2;
            }

		    in.segmentmarkerlist[i] = DEFATTRSQUELETTE;
		    in.numberofsegments++;
	    }
	    nBeginSegment += polygon.size();
    }
    else
    {
        in.segmentlist = (int *) malloc( nTotalSegment * 2 * sizeof(int));
	    in.segmentmarkerlist = (uint*) malloc( nTotalSegment * sizeof(uint));
	    memset( (void*)in.segmentmarkerlist, 0 , nTotalSegment * sizeof(int) );

	    for ( i = nBeginSegment ; i < nBeginSegment + polygon.size() - 1; ++i )
	    {
		    in.segmentlist[i * 2 + 0] = i + 1;
            in.segmentlist[i * 2 + 1] = i + 2;
		    in.segmentmarkerlist[i] = DEFATTRSQUELETTE;
		    in.numberofsegments++;
	    }
	    nBeginSegment += polygon.size();

    }
	
    if ( !bOutSide )
    {
	    in.numberofholes = 0;
        for ( j = 1 ; j < rings.size() ; ++j )
	    {
		    const T_PointPtrVector& hole = rings[j];

		    MT_Vector2D vPosHole;
		    if ( GetHoleCenterPoint( hole, vPosHole ) )
		    {
			    in.holelist[ (j - 1) * 2 + 0] = vPosHole.rX_;
			    in.holelist[ (j - 1) * 2 + 1] = vPosHole.rY_;
			    int nIdxPointHole = 0;
			    for ( i = nBeginPoint ; i < nBeginPoint + hole.size() ; ++i )
			    {
                    MT_Float rX, rY;
                    rX = ( hole[nIdxPointHole]->rX_ * MULPRECISION) + DECALPREC;
				    rY = ( hole[nIdxPointHole]->rY_ * MULPRECISION) + DECALPREC;
				    in.pointlist[ i * 2 + 0 ] = rX;
				    in.pointlist[ i * 2 + 1 ] = rY;
				    in.pointattributelist[i] = DEFATTRHOLE + ( j - 1 );
				    in.pointmarkerlist[i] = DEFATTRHOLE + ( j - 1 );
				    in.numberofpoints++;
				    nIdxPointHole++;
			    }
			    nBeginPoint += hole.size();

			    for ( i = nBeginSegment ; i < nBeginSegment + hole.size(); ++i )
			    {
				    in.segmentlist[i * 2 + 0] = i + 1;
                    if ( i == nBeginSegment + hole.size() - 1 )
                    {
                        in.segmentlist[i * 2 + 1] = nBeginSegment + 1;
                    }
                    else
                    {
                        in.segmentlist[i * 2 + 1] = i + 2;
                    }
				    in.segmentmarkerlist[i] = DEFATTRHOLE + ( j - 1 );
				    in.numberofsegments++;
			    }
			    nBeginSegment += hole.size();
		    }
		    in.numberofholes++;
	    }
    }
    else
    {
	    in.numberofholes = 0;
        for ( j = 1 ; j < rings.size() ; ++j )
	    {
		    const T_PointPtrVector& hole = rings[j];
			int nIdxPointHole = 0;
			for ( i = nBeginPoint ; i < nBeginPoint + hole.size(); ++i )
			{
                MT_Float rX, rY;
                rX = ( hole[nIdxPointHole]->rX_ * MULPRECISION) + DECALPREC;
				rY = ( hole[nIdxPointHole]->rY_ * MULPRECISION) + DECALPREC;
				in.pointlist[ i * 2 + 0 ] = rX;
				in.pointlist[ i * 2 + 1 ] = rY;
				in.pointattributelist[i] = DEFATTRSQUELETTE;
				in.pointmarkerlist[i] = DEFATTRSQUELETTE;
				in.numberofpoints++;
				nIdxPointHole++;
			}
			nBeginPoint += hole.size();

			for ( i = nBeginSegment ; i < nBeginSegment + hole.size() - 1 ; ++i )
			{
				in.segmentlist[i * 2 + 0] = i + 1;
                in.segmentlist[i * 2 + 1] = i + 2;
				in.segmentmarkerlist[i] = DEFATTRSQUELETTE;
				in.numberofsegments++;
			}
			nBeginSegment += hole.size();
	    }
    }
    

    Clean( in.pointmarkerlist );

    in.pointmarkerlist = (int *) malloc( in.numberofpoints * sizeof(int) );
	for ( i = 0 ; i < in.numberofpoints ; ++i )
	{
		in.pointmarkerlist[i] = i;
	}
    
	// Make necessary initializations so that Triangle can return a triangulation in `mid' and a voronoi diagram in `vorout'.

	mid.pointlist = (MT_Float *) NULL;				// Not needed if -N switch used.

	// Not needed if -N switch used or number of point attributes is zero:
	mid.pointattributelist = (MT_Float *) NULL;
	mid.pointmarkerlist = (int *) NULL;			// Not needed if -N or -B switch used.
	mid.trianglelist = (int *) NULL;			// Not needed if -E switch used.
	
	// Not needed if -E switch used or number of triangle attributes is zero:
	mid.triangleattributelist = (MT_Float *) NULL;
	mid.neighborlist = (int *) NULL;			// Needed only if -n switch used.
	
	// Needed only if segments are output (-p or -c) and -P not used:
	mid.segmentlist = (int *) NULL;

	// Needed only if segments are output (-p or -c) and -P and -B not used:
	mid.segmentmarkerlist = (uint*) NULL;
	mid.edgelist = (int *) NULL;				// Needed only if -e switch used.
	mid.edgemarkerlist = (int *) NULL;			// Needed if -e used and -B not used.

	vorout.pointlist = (MT_Float *) NULL;			// Needed only if -v switch used.
	
	// Needed only if -v switch used and number of attributes is not zero:
	vorout.pointattributelist = (MT_Float *) NULL;
	vorout.edgelist = (int *) NULL;				// Needed only if -v switch used.
	vorout.normlist = (MT_Float *) NULL;			// Needed only if -v switch used.

	DoTriangulate(sCommand, in, mid, vorout);		//qa0.01

	MT_Vector2D vPos( 0., 0. );

	// On stock les points
	for ( i = 0 ; i < mid.numberofpoints ; ++i )
	{
		MT_Float rX1 = mid.pointlist[ i * 2 + 0 ];
		MT_Float rY1 = mid.pointlist[ i * 2 + 1 ];
		vPos.rX_ = ( rX1 - DECALPREC ) / MULPRECISION;
		vPos.rY_ = ( rY1 - DECALPREC ) / MULPRECISION;
        vListPoints.push_back( new MT_Vector2D( vPos ) );
	}

	// On stock les triangles
	for ( i = 0 ; i < mid.numberoftriangles ; ++i )
	{
		
        MT_Vector2D& point1 = *vListPoints[ mid.trianglelist[i * 3 + 0] - 1 ];
		MT_Vector2D& point2 = *vListPoints[ mid.trianglelist[i * 3 + 1] - 1 ];
		MT_Vector2D& point3 = *vListPoints[ mid.trianglelist[i * 3 + 2] - 1 ];
		vListTriangles.push_back( new MT_Triangle( point1, point2, point3 ) );
	}
 
	// On stock le squelette
	for ( i = 0 ; i < mid.numberofsegments ; ++i )
	{
		if ( mid.segmentmarkerlist[i] == DEFATTRSQUELETTE  )
		{
			MT_Vector2D& point1 = *vListPoints[ mid.segmentlist[i * 2 + 0] - 1 ];
			MT_Vector2D& point2 = *vListPoints[ mid.segmentlist[i * 2 + 1] - 1 ];
			vListLines.push_back( new MT_Line( point1, point2 ) );
		}
	}
	
	
	// On stock les trous
	for ( j = 1 ; j < rings.size() ; ++j )
	{
		for ( i = 0 ; i < mid.numberofsegments ; ++i )
		{
			if ( mid.segmentmarkerlist[i] == (uint)( DEFATTRHOLE + ( j - 1 ) ) )
			{
				MT_Vector2D& point1 = *vListPoints[ mid.segmentlist[i * 2 + 0] - 1 ];
				MT_Vector2D& point2 = *vListPoints[ mid.segmentlist[i * 2 + 1] - 1 ];
				vListLines.push_back( new MT_Line( point1, point2 ) );
			}
		}
	}

    CleanAll( in, mid, vorout );
}



//-----------------------------------------------------------------------------
// Name: tricall::DoTriangulateCom
// Created: FBD 03-05-06
//-----------------------------------------------------------------------------
void DoTriangulateCom( char* sCommand, const T_PointVector& polygon, T_PointPtrVector& vListPoints, T_TrianglePtrVector& vListTriangles, T_LinePtrVector& vListLines, bool bOutSide )
{
	uint i, j;

	struct triangulateio in, mid, vorout;
	
	memset( (void*)&in, 0, sizeof ( struct triangulateio ) );
	memset( (void*)&mid, 0, sizeof ( struct triangulateio ) );
	memset( (void*)&vorout, 0, sizeof ( struct triangulateio ) );

    uint nNbrHoles = 0;

    if ( !bOutSide )
        nNbrHoles = 0;

    if ( nNbrHoles )
    {
        in.holelist = (MT_Float*) malloc( nNbrHoles * 2 * sizeof(MT_Float));
    }

	int nTotalPoint = polygon.size();
	int nTotalSegment = polygon.size();

	in.numberofpoints = 0;
	in.pointlist = (MT_Float *) malloc( nTotalPoint * 2 * sizeof(MT_Float));
	in.numberofpointattributes = 1;
	in.pointattributelist = (MT_Float *) malloc( nTotalPoint * in.numberofpointattributes * sizeof(MT_Float));
	memset( (void*)in.pointattributelist, 0 , nTotalPoint * in.numberofpointattributes * sizeof(MT_Float) );
	in.pointmarkerlist = (int *) malloc( nTotalPoint * in.numberofpointattributes * sizeof(MT_Float));
	memset( (void*)in.pointmarkerlist, 0 , nTotalPoint * in.numberofpointattributes * sizeof(MT_Float) );
	
	int nBeginPoint = 0;
	int nIdxPoint = 0;
	for ( i = nBeginPoint ; i < nBeginPoint + polygon.size() ; ++i )
	{
        MT_Float rX, rY;
        rX = ( polygon[nIdxPoint].rX_ * MULPRECISION ) + DECALPREC;
		rY = ( polygon[nIdxPoint].rY_ * MULPRECISION ) + DECALPREC;
        in.pointlist[ i * 2 + 0 ] = rX;
		in.pointlist[ i * 2 + 1 ] = rY;
		in.pointattributelist[i] = DEFATTRFREE;
		in.pointmarkerlist[i] = DEFATTRFREE;
		in.numberofpoints++;
		nIdxPoint++;
	}
    nBeginPoint += polygon.size();


    in.numberofsegments = 0;
	int nBeginSegment = 0;
    if ( !bOutSide )
    {
        in.segmentlist = (int *) malloc( nTotalSegment * 2 * sizeof(int));
	    in.segmentmarkerlist = (uint*) malloc( nTotalSegment * sizeof(uint));
	    memset( (void*)in.segmentmarkerlist, 0 , nTotalSegment * sizeof(int) );

	    for ( i = nBeginSegment ; i < nBeginSegment + polygon.size(); ++i )
	    {
		    in.segmentlist[i * 2 + 0] = i + 1;
            if ( i == nBeginSegment + polygon.size() - 1 )
            {
                in.segmentlist[i * 2 + 1] = nBeginSegment + 1;
            }
            else
            {
                in.segmentlist[i * 2 + 1] = i + 2;
            }

		    in.segmentmarkerlist[i] = DEFATTRFREE;
		    in.numberofsegments++;
	    }
	    nBeginSegment += polygon.size();
    }
    else
    {
        in.segmentlist = (int *) malloc( nTotalSegment * 2 * sizeof(int));
	    in.segmentmarkerlist = (uint*) malloc( nTotalSegment * sizeof(uint));
	    memset( (void*)in.segmentmarkerlist, 0 , nTotalSegment * sizeof(int) );

	    for ( i = nBeginSegment ; i < nBeginSegment + polygon.size() - 1; ++i )
	    {
		    in.segmentlist[i * 2 + 0] = i + 1;
            in.segmentlist[i * 2 + 1] = i + 2;
		    in.segmentmarkerlist[i] = DEFATTRFREE;
		    in.numberofsegments++;
	    }
	    nBeginSegment += polygon.size();

    }
	
    if ( !bOutSide )
	    in.numberofholes = 0;
    else
	    in.numberofholes = 0;
      
    Clean( in.pointmarkerlist );

    in.pointmarkerlist = (int *) malloc( in.numberofpoints * sizeof(int) );
	for ( i = 0 ; i < in.numberofpoints ; ++i )
	{
		in.pointmarkerlist[i] = i;
	}
    
	// Make necessary initializations so that Triangle can return a triangulation in `mid' and a voronoi diagram in `vorout'.

	mid.pointlist = (MT_Float *) NULL;				// Not needed if -N switch used.

	// Not needed if -N switch used or number of point attributes is zero:
	mid.pointattributelist = (MT_Float *) NULL;
	mid.pointmarkerlist = (int *) NULL;			// Not needed if -N or -B switch used.
	mid.trianglelist = (int *) NULL;			// Not needed if -E switch used.
	
	// Not needed if -E switch used or number of triangle attributes is zero:
	mid.triangleattributelist = (MT_Float *) NULL;
	mid.neighborlist = (int *) NULL;			// Needed only if -n switch used.
	
	// Needed only if segments are output (-p or -c) and -P not used:
	mid.segmentlist = (int *) NULL;

	// Needed only if segments are output (-p or -c) and -P and -B not used:
	mid.segmentmarkerlist = (uint*) NULL;
	mid.edgelist = (int *) NULL;				// Needed only if -e switch used.
	mid.edgemarkerlist = (int *) NULL;			// Needed if -e used and -B not used.

	vorout.pointlist = (MT_Float *) NULL;			// Needed only if -v switch used.
	
	// Needed only if -v switch used and number of attributes is not zero:
	vorout.pointattributelist = (MT_Float *) NULL;
	vorout.edgelist = (int *) NULL;				// Needed only if -v switch used.
	vorout.normlist = (MT_Float *) NULL;			// Needed only if -v switch used.

	DoTriangulate(sCommand, in, mid, vorout);		//qa0.01

	MT_Vector2D vPos( 0., 0. );

	// On stock les points
	for ( i = 0 ; i < mid.numberofpoints ; ++i )
	{
		MT_Float rX1 = mid.pointlist[ i * 2 + 0 ];
		MT_Float rY1 = mid.pointlist[ i * 2 + 1 ];
		vPos.rX_ = ( rX1 - DECALPREC ) / MULPRECISION;
		vPos.rY_ = ( rY1 - DECALPREC ) / MULPRECISION;
        vListPoints.push_back( new MT_Vector2D( vPos ) );
	}

	// On stock les triangles
	for ( i = 0 ; i < mid.numberoftriangles ; ++i )
	{
		
        MT_Vector2D& point1 = *vListPoints[ mid.trianglelist[i * 3 + 0] - 1 ];
		MT_Vector2D& point2 = *vListPoints[ mid.trianglelist[i * 3 + 1] - 1 ];
		MT_Vector2D& point3 = *vListPoints[ mid.trianglelist[i * 3 + 2] - 1 ];
		vListTriangles.push_back( new MT_Triangle( point1, point2, point3 ) );
	}
 
	// On stock le squelette
	for ( i = 0 ; i < mid.numberofsegments ; ++i )
	{                                       
		if ( mid.segmentmarkerlist[i] == DEFATTRSQUELETTE  )
		{
			MT_Vector2D& point1 = *vListPoints[ mid.segmentlist[i * 2 + 0] - 1 ];
			MT_Vector2D& point2 = *vListPoints[ mid.segmentlist[i * 2 + 1] - 1 ];
			vListLines.push_back( new MT_Line( point1, point2 ) );
		}
	}

    CleanAll( in, mid, vorout );
}

struct sortDist : std::binary_function< MT_Vector3D, MT_Vector3D, int >
{
	int operator() ( const MT_Vector3D& vDist1, const MT_Vector3D& vDist2 ) { return( vDist1.rZ_ < vDist2.rZ_ ); };
};

bool GetHoleCenterPoint( const T_PointPtrVector& hole, MT_Vector2D& vPos )
{
	uint i;
	MT_Float rXMin = +8000000.0;
	MT_Float rXMax = -8000000.0;
	MT_Float rYMin = +8000000.0;
	MT_Float rYMax = -8000000.0;

	for ( i = 0 ; i < hole.size() ; ++i )
	{
		MT_Float rX = (hole[i]->rX_ * MULPRECISION) + DECALPREC;
		MT_Float rY = (hole[i]->rY_ * MULPRECISION) + DECALPREC;
		if ( rX < rXMin )
			rXMin = rX;
		if ( rX > rXMax )
			rXMax = rX;
		if ( rY < rYMin )
			rYMin = rY;
		if ( rY > rYMax )
			rYMax = rY;
	}

	MT_Float rX3 = rXMin - 1.0;
	MT_Float rY3 = (rYMax - rYMin ) / 2.0 + rYMin;
	MT_Float rX4 = rXMax + 1.0;
	MT_Float rY4 = (rYMax - rYMin ) / 2.0 + rYMin;


	std::vector<MT_Vector3D> listIntersect;

	for ( i = 0 ; i < hole.size(); ++i )
	{
        
        MT_Float rX1 = (hole[i+0]->rX_ * MULPRECISION) + DECALPREC;
		MT_Float rY1 = (hole[i+0]->rY_ * MULPRECISION) + DECALPREC;
		MT_Float rX2;
        MT_Float rY2;
        if ( i == hole.size() - 1 )
        {
            rX2 = (hole[0]->rX_ * MULPRECISION) + DECALPREC;;
		    rY2 = (hole[0]->rY_ * MULPRECISION) + DECALPREC;;
        }
        else
        {
            rX2 = (hole[i+1]->rX_ * MULPRECISION) + DECALPREC;;
		    rY2 = (hole[i+1]->rY_ * MULPRECISION) + DECALPREC;;
        }

		MT_Float rA1, rA2, rB1, rB2, rC1, rC2;	// Coefficients of line eqns.
		MT_Float r1, r2, r3, r4;				// 'Sign' values
		MT_Float rDenom, rOffset, rNum;		// Intermediate values

		// Compute rA1, rB1, rC1, where line joining points 1 and 2 is "rA1 rX + rB1 rY + rC1 = 0.f"

		rA1 = rY2 - rY1;
		rB1 = rX1 - rX2;
		rC1 = rX2 * rY1 - rX1 * rY2;

		// Compute r3 and r4.

		r3 = rA1 * rX3 + rB1 * rY3 + rC1;
		r4 = rA1 * rX4 + rB1 * rY4 + rC1;

		/// Check signs of r3 and r4. 
		// If both point 3 and point 4 lie on same side of line 1, the line segments do not intersect

		if ( !MT_IsZero( r3 ) && !MT_IsZero( r4 ) && MT_IsSameSigns( r3, r4 ) )
		{
			continue;
		}
        
		// Compute a2, b2, c2

		rA2 = rY4 - rY3;
		rB2 = rX3 - rX4;
		rC2 = rX4 * rY3 - rX3 * rY4;

		// Compute r1 and r2

		r1 = rA2 * rX1 + rB2 * rY1 + rC2;
		r2 = rA2 * rX2 + rB2 * rY2 + rC2;

		// Check signs of r1 and r2.
		// If both point 1 and point 2 lie on same side of second line segment, the line segments do not intersect

		if ( !MT_IsZero( r1 ) && !MT_IsZero( r2 ) && MT_IsSameSigns( r1, r2 ) )
		{
			continue;
		}
        
		// Line segments intersect: compute intersection point

		rDenom = rA1 * rB2 - rA2 * rB1;
		if ( MT_IsZero( rDenom ) )
		{
			continue; // Colinear
		}
        
		rOffset = rDenom < 0.f ? - rDenom / 2.f : rDenom / 2.f;

		// The rDenom/2 is to get rounding instead of truncating. 
		// It is added or subtracted to the numerator, depending upon the sign of the numerator.

		MT_Vector3D vPosIntersect;

		rNum = rB1 * rC2 - rB2 * rC1;
		vPosIntersect.rX_ = ( rNum < 0.f ? rNum - rOffset : rNum + rOffset ) / rDenom;
		MT_Float rDX = rX3 - vPosIntersect.rX_ ;
		rNum = rA2 * rC1 - rA1 * rC2;
		vPosIntersect.rY_ = ( rNum < 0.f ? rNum - rOffset : rNum + rOffset ) / rDenom;
		MT_Float rDY = rY3 - vPosIntersect.rY_;
		
		vPosIntersect.rZ_ = ( rDX * rDX ) + ( rDY * rDY );
		listIntersect.push_back( vPosIntersect );
	}

	int nCol = listIntersect.size();
	if (  nCol < 2 || nCol & 0x1 )
		return false;
	
	
	std::sort( listIntersect.begin(), listIntersect.end(), sortDist() );


	MT_Float rXCol1 = listIntersect[0].rX_;
	MT_Float rYCol1 = listIntersect[0].rY_;
	MT_Float rXCol2 = listIntersect[1].rX_;
	MT_Float rYCol2 = listIntersect[1].rY_;

	vPos.rX_ = ( ( rXCol2 - rXCol1 ) / 2.0 ) + rXCol1;
	vPos.rY_ = ( ( rYCol2 - rYCol1 ) / 2.0 ) + rYCol1;
	return true;
}


