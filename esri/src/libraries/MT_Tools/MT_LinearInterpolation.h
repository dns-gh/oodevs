//*****************************************************************************
//
// $Created: JVT 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_LinearInterpolation.h $
// $Author: Jvt $
// $Modtime: 19/09/03 17:05 $
// $Revision: 7 $
// $Workfile: MT_LinearInterpolation.h $
//
//*****************************************************************************

#ifndef __MT_LinearInterpolation_h_
#define __MT_LinearInterpolation_h_

//*****************************************************************************
// Created: JVT 02-12-17
// Last modified: JVT 03-02-24
//*****************************************************************************
template < class TypeIn, class TypeOut = TypeIn >
struct MT_LinearInterpolation
{
    TypeOut operator () ( const TypeIn& x1, const TypeOut& y1, const TypeIn& x2, const TypeOut& y2, const TypeIn& x ) const
    {
        return (TypeOut)( y2 + ( y1 - y2 ) * ( x2 - x ) / ( x2 - x1 ) );
    }
    
    TypeIn InverseInterpolation ( const TypeIn& x1, const TypeOut& y1, const TypeIn& x2, const TypeOut& y2, const TypeOut& y ) const
    {
        return y1 == y2 ? y == y1 ? x2 : 0  : (TypeIn)( x2 + ( x1 - x2 ) * ( y2 - y ) / ( y2 - y1 ) );
    }
};

#endif // __MT_LinearInterpolation_h_