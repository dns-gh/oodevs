// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationFilterCapacity_h_
#define __PopulationFilterCapacity_h_

#include "ObjectCapacity_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  PopulationFilterCapacity
    @brief  PopulationFilterCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class PopulationFilterCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationFilterCapacity();
    explicit PopulationFilterCapacity( xml::xistream& xis );
    virtual ~PopulationFilterCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    //@}

private:
    //! @name Member data
    //@{
    double density_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PopulationFilterCapacity )

#endif // __PopulationFilterCapacity_h_
