//*****************************************************************************
//
// $Created: JVT 03-03-05 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Interpolation.cpp $
// $Author: Jvt $
// $Modtime: 19/03/03 10:59 $
// $Revision: 3 $
// $Workfile: MT_Interpolation.cpp $
//
//*****************************************************************************

#include "MT_Tools_pch.h"

#include "MT_InterpolatedFunction.h"

#include "MT_LinearInterpolation.h"
#include "MT_ConstantInterpolation.h"

#include "MT_Polynome.h"

#ifdef DISPLAYPOLY
    template < class coeffType, class valuesType >
    std::ostream& operator << ( std::ostream& out, const MT_Polynome< coeffType, valuesType >& poly )
    {
        for ( uint i = poly.GetOrder(); i; --i )
        {
            coeffType coef = poly.GetCoeff( i );

            if ( coef != coeffType() )
                out << coef << ".x^" << i << " + " ;
        }
        return out << poly.GetCoeff( 0 );
    }
#endif
