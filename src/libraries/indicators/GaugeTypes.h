// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __GaugeTypes_h_
#define __GaugeTypes_h_

#include "GaugeFactory_ABC.h"
#include "clients_kernel/Resolver.h"

namespace xml
{
    class xistream;
}

namespace indicators
{
    class GaugeType;
    class GaugeTypeFactory;

// =============================================================================
/** @class  GaugeTypes
    @brief  GaugeTypes
*/
// Created: SBO 2009-05-05
// =============================================================================
class GaugeTypes : public kernel::Resolver< GaugeType, QString >
                 , public GaugeFactory_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             GaugeTypes();
    virtual ~GaugeTypes();
    //@}

    //! @name Operations
    //@{
    void Load( const std::string& filename );
    void Purge();
    virtual Gauge* Create() const;
    virtual Gauge* Create( xml::xistream& xis ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    GaugeTypes( const GaugeTypes& );            //!< Copy constructor
    GaugeTypes& operator=( const GaugeTypes& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void ReadTemplate( xml::xistream& xis );
    //@}

private:
    //! @name Member data
    //@{
    std::auto_ptr< GaugeTypeFactory > factory_;
    //@}
};

}

#endif // __GaugeTypes_h_
