//*****************************************************************************
//
// $Created: JVT 03-03-05 $
// $Archive: /MVW_v10/Build/SDK/MT_Tools/Src/MT_Polynome.h $
// $Author: Nld $
// $Modtime: 16/07/04 15:38 $
// $Revision: 5 $
// $Workfile: MT_Polynome.h $
//
//*****************************************************************************

#ifndef __MT_Polynome_h_
#define __MT_Polynome_h_

//*****************************************************************************
// Created: JVT 03-03-05
// Last modified: JVT 03-03-06
//
// Polynôme simple ayant la plupart de ses coefficients non nul
// $$$$ Si besoin : prévoir une classe gérant mieux ceux qui ont beaucoup de coeff à 0
//
// contraintes :
// _ valuesType doit supporter l'operateur + ( valuesType )
// _ valuesType doit supporter l'opérateur * ( valuesType )
// _ valuesType doit supporter l'opérateur * ( coeffType )
// _ si DISPLAYPOLY est défini, il faut éventuellement définir l'opérateur << d'une ostream avec valuesType
//*****************************************************************************

template < class coeffType, class valuesType >
class MT_Polynome
{
public:
    typedef             std::vector< coeffType >        T_CoeffVector;
    typedef typename T_CoeffVector::const_iterator   CIT_CoeffVector;

public:
    MT_Polynome( uint nOrder = 0 )
        : vCoeff_( nOrder, coeffType() )
    {
    }

    MT_Polynome( const MT_Polynome< coeffType, valuesType >& rhs )
        : vCoeff_( rhs.vCoeff_ )
    {
    }

    virtual ~MT_Polynome()
    {
        vCoeff_.clear();
    }

    MT_Polynome< coeffType, valuesType >& operator = ( const MT_Polynome< coeffType, valuesType >& rhs )
    {
        vCoeff_ = rhs.vCoeff_;
    }

    uint        GetOrder () const
    {
        return vCoeff_.size() ? vCoeff_.size() - 1 : 0;
    }

    coeffType   GetCoeff ( uint nOrder ) const
    {
        return nOrder > vCoeff_.size() ? coeffType() : vCoeff_[ nOrder ];
    }

    void        SetCoeff ( uint nOrder, const coeffType& value )
    {
        if ( nOrder >= vCoeff_.size() )
            vCoeff_.resize( nOrder + 1 );
        vCoeff_[ nOrder ] = value;
    }

    void        SetCoeffs( const T_CoeffVector& coefs )
    {
        vCoeff_ = coefs;
    }

    valuesType operator () ( valuesType value )
    {
        if ( !vCoeff_.size() )
            return valuesType();

        CIT_CoeffVector it = vCoeff_.begin();
        valuesType res = (*it);

        for ( ++it; it != vCoeff_.end(); ++it, value *= value )
            res += value * *it;

        return res;
    }

protected:
    T_CoeffVector   vCoeff_;
};

#ifdef DISPLAYPOLY
    template < class coeffType, class valuesType >
    std::ostream& operator << ( std::ostream& out, const MT_Polynome< coeffType, valuesType >& poly );
#endif


#ifdef USE_INLINE
#    include "MT_Polynome.inl"
#endif


#endif // __MT_Polynome_h_