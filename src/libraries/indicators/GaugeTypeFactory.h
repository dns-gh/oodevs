// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __GaugeTypeFactory_h_
#define __GaugeTypeFactory_h_

namespace xml
{
    class xistream;
}

namespace indicators
{
    class GaugeType;

// =============================================================================
/** @class  GaugeTypeFactory
    @brief  GaugeTypeFactory
*/
// Created: SBO 2009-05-05
// =============================================================================
class GaugeTypeFactory
{

public:
    //! @name Constructors/Destructor
    //@{
             GaugeTypeFactory();
    virtual ~GaugeTypeFactory();
    //@}

    //! @name Operations
    //@{
    GaugeType* Create( xml::xistream& xis );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GaugeTypeFactory( const GaugeTypeFactory& );            //!< Copy constructor
    GaugeTypeFactory& operator=( const GaugeTypeFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

}

#endif // __GaugeTypeFactory_h_
