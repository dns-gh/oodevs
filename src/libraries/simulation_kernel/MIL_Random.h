// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __MIL_Random_h_
#define __MIL_Random_h_

#include <vector>

class MT_GaussianRandom;

// =============================================================================
/** @class  MIL_Random
    @brief  MIL_Random
*/
// Created: JSR 2010-07-02
// =============================================================================
class MIL_Random
{
public:
    enum ERandomContexts
    {
        eFire,
        eWounds,
        ePerception,
        eBreakdowns,
        eContextsNbr
    };

public:
    static void Initialize( int nSeed, const std::vector< bool >& bGaussian,
        const std::vector< double >& rDeviation, const std::vector< double >& rMean );
    static void Terminate();

    static long rand32();                            // [ 0  , 0xffffffff ]
    static unsigned long rand32_ii( unsigned long min, unsigned long max, int ctxt = -1 ); // [ min, max ]
    static unsigned long rand32_io( unsigned long min, unsigned long max, int ctxt = -1 ); // [ min, max [
    static unsigned long rand32_oo( unsigned long min, unsigned long max, int ctxt = -1 ); // ] min, max [
    static unsigned long rand32_oi( unsigned long min, unsigned long max, int ctxt = -1 ); // ] min, max ]

    static double rand_ii( int ctxt = -1 );                           // [ 0.  , 1.   ]
    static double rand_io( int ctxt = -1 );                           // [ 0.  , 1.   [
    static double rand_oo( int ctxt = -1 );                           // ] 0.  , 1.   [
    static double rand_oi( int ctxt = -1 );                           // ] 0.  , 1.   ]
    static double rand_ii( double min, double max, int ctxt = -1 );   // [ min., max. ]
    static double rand_io( double min, double max, int ctxt = -1 );   // [ min., max. [
    static double rand_oo( double min, double max, int ctxt = -1 );   // ] min., max. [
    static double rand_oi( double min, double max, int ctxt = -1 );   // ] min., max. ]

    template< typename T >
    static void random_shuffle( std::vector< T >& vector, int ctxt = -1 );

private:
    //! @name Constructors/Destructor
    //@{
             MIL_Random( int nSeed, const std::vector< bool >& bGaussian,
                 const std::vector< double >& rDeviation, const std::vector< double >& rMean );
    virtual ~MIL_Random();
    //@}

private:
    //! @name Copy/Assignment
    //@{
    MIL_Random( const MIL_Random& );            //!< Copy constructor
    MIL_Random& operator=( const MIL_Random& ); //!< Assignment operator
    //@}

private:
    //! @name Helpers
    //@{
    static double gaussian_ii( int ctxt );
    static double gaussian_io( int ctxt );
    static double gaussian_oo( int ctxt );
    static double gaussian_oi( int ctxt );
    //@}

private:
    //! @name Static data
    //@{
    static MIL_Random* pInstance_;
    static std::vector< MT_GaussianRandom* > gaussianRandom_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: MIL_Random::random_shuffle
// Created: JSR 2011-05-05
// -----------------------------------------------------------------------------
template< typename T >
void MIL_Random::random_shuffle( std::vector< T >& vector, int ctxt )
{
    std::function< unsigned long( unsigned long ) > fun =  boost::bind( &MIL_Random::rand32_io, 0, _1, ctxt );
    std::random_shuffle( vector.begin(), vector.end(), fun );
}

#endif // __MIL_Random_h_
