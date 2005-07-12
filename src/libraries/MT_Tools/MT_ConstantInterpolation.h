//*****************************************************************************
//
// $Created: JVT 03-03-19 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_ConstantInterpolation.h $
// $Author: Jvt $
// $Modtime: 19/03/03 10:59 $
// $Revision: 1 $
// $Workfile: MT_ConstantInterpolation.h $
//
//*****************************************************************************

#ifndef __MT_ConstantInterpolation_h_
#define __MT_ConstantInterpolation_h_


//*****************************************************************************
// Created: JVT 03-03-19
//*****************************************************************************
template < class TypeIn, class TypeOut >
struct MT_ConstantInterpolation
{
    TypeOut operator () ( const TypeIn& x1, const TypeOut& y1, const TypeIn& x2, const TypeOut& y2, const TypeIn& x ) const
    {
        assert( x1 <= x );
        assert( x <= x2 );
        assert( x1 != x2 );

        return x == x2 ? y2 : y1;
    }
};

#endif // __MT_ConstantInterpolation_h_