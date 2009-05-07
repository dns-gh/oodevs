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
    class xostream;
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
    //! @name Types
    //@{
    typedef std::map< std::pair< double, double >, double > T_Intervals;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             GaugeNormalizer();
    explicit GaugeNormalizer( xml::xistream& xis );
             GaugeNormalizer( const GaugeNormalizer& );
    virtual ~GaugeNormalizer();
    //@}

    //! @name Accessors
    //@{
    const T_Intervals& Intervals() const;
    //@}

    //! @name Operations
    //@{
    void AddInterval( double min, double max, double key );
    double Normalize( double value ) const;
    void Serialize( xml::xostream& xos ) const;
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

private:
    //! @name Member data
    //@{
    T_Intervals intervals_;
    //@}
};

}

#endif // __GaugeNormalizer_h_
