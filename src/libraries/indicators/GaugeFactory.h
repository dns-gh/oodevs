// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __GaugeFactory_h_
#define __GaugeFactory_h_

namespace xml
{
    class xistream;
}

namespace indicators
{
    class Gauge;
    class GaugeTypes;

// =============================================================================
/** @class  GaugeFactory
    @brief  GaugeFactory
*/
// Created: SBO 2009-05-06
// =============================================================================
class GaugeFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             GaugeFactory();
    virtual ~GaugeFactory();
    //@}

    //! @name Operations
    //@{
    Gauge* Create( xml::xistream& xis ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GaugeFactory( const GaugeFactory& );            //!< Copy constructor
    GaugeFactory& operator=( const GaugeFactory& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< GaugeTypes > types_;
    //@}
};

}

#endif // __GaugeFactory_h_
