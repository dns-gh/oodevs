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

#include <vector>

inline double Interpolate( const double& x1, const double& y1, const double& x2, const double& y2, const double& x )
{
    return ( y2 + ( y1 - y2 ) * ( x2 - x ) / ( x2 - x1 ) );
}

inline double InverseInterpolate( const double& x1, const double& y1, const double& x2, const double& y2, const double& y )
{
    return y1 == y2 ? y == y1 ? x2 : 0.0 : ( x2 + ( x1 - x2 ) * ( y2 - y ) / ( y2 - y1 ) );
}

class MT_InterpolatedFunction
{
public:
    typedef std::pair< double, double >       T_Point;
    typedef std::vector< T_Point >            T_Points;
    typedef T_Points::iterator                IT_Points;
    typedef T_Points::const_iterator          CIT_Points;
    typedef T_Points::const_reverse_iterator  CRIT_Points;

public:
    MT_InterpolatedFunction()
        : knownData_(), useBeforeDefault_( false ), beforeDefaultValue_( double() ), useAfterDefault_( false ), afterDefaultValue_( double() ) {}
    MT_InterpolatedFunction( const double& beforeVal, const double& afterVal )
        : knownData_(), useBeforeDefault_( true ), beforeDefaultValue_( beforeVal ), useAfterDefault_( true ), afterDefaultValue_( afterVal ) {}
    virtual ~MT_InterpolatedFunction() {}

    void SetBeforeValue( const double& val )
    {
        useBeforeDefault_   = true;
        beforeDefaultValue_ = val;
    }

    void SetAfterValue( const double& val )
    {
        useAfterDefault_   = true;
        afterDefaultValue_ = val;
    }

    void Reset()
    {
        knownData_.clear();
    }

    void AddNewPoint( const double& x, const double& y )
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

    double operator () ( const double& x ) const
    {
        if( knownData_.empty() )
            return (double)-1;

        // $$$$ AGE 2005-04-12: Use a dichotomy
        CIT_Points it = knownData_.begin();
        while( it != knownData_.end() && it->first < x )
            ++it;

        if( it == knownData_.end() )
            return useAfterDefault_ ? afterDefaultValue_ : knownData_.back().second;

        if( it->first == x )
            return it->second;

        if( it == knownData_.begin() )
            return useBeforeDefault_ ? beforeDefaultValue_ : it->second;

        CIT_Points it0 = it;
        --it0;
        return Interpolate( it0->first, it0->second, it->first, it->second, x );
    }

    // $$$$ JVT : Fonction un peu pourrie....
    // $$$$ AGE 2005-04-12: Et c'est plutot GetMaxXForY ...
    double GetMaxYForX ( const double& y ) const
    {
        if( knownData_.empty() )
            return (double)0;

        CRIT_Points it1 = knownData_.rbegin();

        if( ( useAfterDefault_ && afterDefaultValue_ >= y ) || ( !useAfterDefault_ && it1->second >= y ) )
            return it1->first;

        for ( CRIT_Points it2 = it1++; it1 != knownData_.rend(); ++it1, ++it2 )
        {
            if( y <= it2->second )
                return it2->first;

            double rRes = InverseInterpolate( it1->first, it1->second, it2->first, it2->second, y );
            if( rRes >= it1->first && rRes <= it2->first && rRes >= (double)0 )
                return rRes;
        }

        return double();
    }

    // $$$$ JVT : Fonction un peu pourrie....
    double GetMinYForX ( const double& y ) const
    {
        if( knownData_.empty() )
            return (double)0;

        CIT_Points it1 = knownData_.begin();

        if( ( useBeforeDefault_ && beforeDefaultValue_ >= y ) || ( !useBeforeDefault_ && it1->second >= y ) )
            return std::min( (double)0, it1->first );

        for ( CIT_Points it2 = it1++; it1 != knownData_.end(); ++it1, ++it2 )
        {
            if( y <= it2->second )
                return it2->first;

            double rRes = InverseInterpolate( it2->first, it2->second, it1->first, it1->second, y );
            if( rRes <= it1->first && rRes >= it2->first && rRes >= (double)0 )
                return rRes;
        }
        return double();
    }

private:
    T_Points      knownData_;

    double        beforeDefaultValue_;
    double        afterDefaultValue_;
    bool          useBeforeDefault_;
    bool          useAfterDefault_;
};

#endif // __MT_InterpolatedFunction_h_
