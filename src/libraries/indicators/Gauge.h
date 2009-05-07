// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __Gauge_h_
#define __Gauge_h_

namespace kernel
{
    class Displayer_ABC;
}

namespace xml
{
    class xistream;
}

namespace indicators
{
    class GaugeNormalizer;
    class GaugeType;
    class GaugeTypes;

// =============================================================================
/** @class  Gauge
    @brief  Gauge
*/
// Created: SBO 2009-05-05
// =============================================================================
class Gauge
{

public:
    //! @name Constructors/Destructor
    //@{
             Gauge( xml::xistream& xis, const GaugeTypes& types );
             Gauge( const Gauge& );
    virtual ~Gauge();
    //@}

    //! @name Operations
    //@{
    void Display( kernel::Displayer_ABC& displayer, double value ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Gauge& operator=( const Gauge& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    const GaugeType& type_;
    std::auto_ptr< GaugeNormalizer > normalizer_;
    //@}
};

}

#endif // __Gauge_h_
