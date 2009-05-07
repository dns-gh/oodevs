// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __GaugeNormalizer_h_
#define __GaugeNormalizer_h_

namespace xml
{
    class xistream;
}

namespace indicators
{

// =============================================================================
/** @class  GaugeNormalizer
    @brief  GaugeNormalizer
*/
// Created: SBO 2009-05-06
// =============================================================================
class GaugeNormalizer
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit GaugeNormalizer( xml::xistream& xis );
             GaugeNormalizer( const GaugeNormalizer& );
    virtual ~GaugeNormalizer();
    //@}

    //! @name Operations
    //@{
    double Normalize( double value ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GaugeNormalizer& operator=( const GaugeNormalizer& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void LoadInterval( xml::xistream& xis );
    //@}

    //! @name Types
    //@{
    typedef std::map< std::pair< double, double >, double > T_Intervals;
    //@}

private:
    //! @name Member data
    //@{
    T_Intervals intervals_;
    //@}
};

}

#endif // __GaugeNormalizer_h_
