//*****************************************************************************
//
// $Created: JVT 02-12-17 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_InterpolatedFunction.h $
// $Author: Nld $
// $Modtime: 15/04/05 10:22 $
// $Revision: 16 $
// $Workfile: MT_InterpolatedFunction.h $
//
//*****************************************************************************

#ifndef __MT_InterpolatedFunction_h_
#define __MT_InterpolatedFunction_h_

//*****************************************************************************
// Created: JVT 02-12-17
// Last modified: JVT 03-03-11
//*****************************************************************************

#include "MT_LinearInterpolation.h"

template < typename TypeIn, typename TypeOut = TypeIn, class Interpolation = MT_LinearInterpolation< TypeIn, TypeOut > >
class MT_InterpolatedFunction
{
public:
    typedef std::pair< TypeIn, TypeOut >                 T_Point;
    typedef std::vector< T_Point >                       T_Points;
    typedef typename T_Points::iterator                 IT_Points;
    typedef typename T_Points::const_iterator          CIT_Points;
    typedef typename T_Points::const_reverse_iterator CRIT_Points;

public:
    MT_InterpolatedFunction()
        : interpolation_(), knownData_(), useBeforeDefault_( false ), beforeDefaultValue_( TypeOut() ), useAfterDefault_( false ), afterDefaultValue_( TypeOut() ) {}
    MT_InterpolatedFunction( const TypeOut& beforeVal, const TypeOut& afterVal )
        : interpolation_(), knownData_(), useBeforeDefault_( true ), beforeDefaultValue_( beforeVal ), useAfterDefault_( true ), afterDefaultValue_( afterVal ) {}

    void SetBeforeValue( const TypeOut& val )
    {
        useBeforeDefault_   = true;
        beforeDefaultValue_ = val;
    }

    void SetAfterValue( const TypeOut& val )
    {
        useAfterDefault_   = true;
        afterDefaultValue_ = val;
    }

    void Reset()
    {
        knownData_.clear();
    }

    void AddNewPoint( const TypeIn& x, const TypeOut& y )
    {
        IT_Points it = knownData_.begin();
        while( it != knownData_.end() && it->first < x )
            ++it;
        if( it != knownData_.end() && it->first == x )
            it->second = y;
        else
            knownData_.insert( it, std::make_pair( x, y ) );
    }

    bool empty() const
    {
        return knownData_.empty();
    }
    
    TypeOut operator () ( const TypeIn& x ) const
    {
        if ( knownData_.empty() )
            return (TypeOut)-1;

        // $$$$ AGE 2005-04-12: Use a dichotomy
        CIT_Points it = knownData_.begin();
        while( it != knownData_.end() && it->first < x )
            ++it;

        if ( it == knownData_.end() )
            return useAfterDefault_ ? afterDefaultValue_ : knownData_.back().second;

        if ( it->first == x )
            return it->second;
            
        if( it == knownData_.begin() )
            return useBeforeDefault_ ? beforeDefaultValue_ : it->second;

        CIT_Points it0 = it;
        --it0;
        return interpolation_( it0->first, it0->second, it->first, it->second, x );
    }

    // $$$$ JVT : Fonction un peu pourrie....
    // $$$$ AGE 2005-04-12: Et c'est plutot GetMaxXForY ...
    TypeIn GetMaxYForX ( const TypeOut& y ) const
    {
        if ( knownData_.empty() )
            return (TypeIn)0;

        CRIT_Points it1 = knownData_.rbegin();

        if ( ( useAfterDefault_ && afterDefaultValue_ >= y ) || ( !useAfterDefault_ && it1->second >= y ) )
            return it1->first;

        for ( CRIT_Points it2 = it1++; it1 != knownData_.rend(); ++it1, ++it2 )
        {
            if ( y <= it2->second )
                return it2->first;

            TypeIn rRes = interpolation_.InverseInterpolation( it1->first, it1->second, it2->first, it2->second, y );
            if ( rRes >= it1->first && rRes <= it2->first && rRes >= (TypeIn)0 )
                return rRes;
        }
        
        return TypeIn();
    }

    // $$$$ JVT : Fonction un peu pourrie....
    TypeIn GetMinYForX ( const TypeOut& y ) const
    {
        if ( knownData_.empty() )
            return (TypeIn)0;
            
        CIT_Points it1 = knownData_.begin();
        
        if ( ( useBeforeDefault_ && beforeDefaultValue_ >= y ) || ( !useBeforeDefault_ && it1->second >= y ) )
            return std::min( (TypeIn)0, it1->first );
        
        for ( CIT_Points it2 = it1++; it1 != knownData_.end(); ++it1, ++it2 )
        {
            if ( y <= it2->second )
                return it2->first;
                
            TypeIn rRes = interpolation_.InverseInterpolation( it2->first, it2->second, it1->first, it1->second, y );
            if ( rRes <= it1->first && rRes >= it2->first && rRes >= (TypeIn)0 )
                return rRes;
        }
        return TypeIn();
    }

private:
    Interpolation interpolation_;
    T_Points      knownData_;

    TypeOut       beforeDefaultValue_;
    TypeOut       afterDefaultValue_;
    bool          useBeforeDefault_;
    bool          useAfterDefault_;
};

#endif // __MT_InterpolatedFunction_h_