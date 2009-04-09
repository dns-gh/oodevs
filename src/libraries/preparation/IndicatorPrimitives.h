// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __IndicatorPrimitives_h_
#define __IndicatorPrimitives_h_

#include "clients_kernel/Resolver.h"

namespace xml
{
    class xistream;
}

namespace tools
{
    class GeneralConfig;
}

class IndicatorPrimitive;

// =============================================================================
/** @class  IndicatorPrimitives
    @brief  IndicatorPrimitives
*/
// Created: SBO 2009-04-06
// =============================================================================
class IndicatorPrimitives : public kernel::Resolver< IndicatorPrimitive, QString >
{

public:
    //! @name Constructors/Destructor
    //@{
             IndicatorPrimitives();
    virtual ~IndicatorPrimitives();
    //@}

    //! @name Operations
    //@{
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IndicatorPrimitives( const IndicatorPrimitives& );            //!< Copy constructor
    IndicatorPrimitives& operator=( const IndicatorPrimitives& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadPrimitive( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    //@}
};

#endif // __IndicatorPrimitives_h_
