// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __PopulationAttribute_h_
#define __PopulationAttribute_h_

#include "ObjectAttribute_ABC.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

class MIL_AutomateLOG;

// =============================================================================
/** @class  PopulationAttribute
    @brief  PopulationAttribute
*/
// Created: JCR 2008-05-30
// =============================================================================
class PopulationAttribute : public ObjectAttribute_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             PopulationAttribute();
             PopulationAttribute( double density );
    explicit PopulationAttribute( xml::xistream& xis );
    virtual ~PopulationAttribute();
    //@}

    //! @name Operations
    //@{
    double GetDensity() const;
    void  Reset();
    void  SetDensity( double rDensity );
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name From ObjectAttribute_ABC
    //@{
    virtual void Register( MIL_Object_ABC& object ) const;
    //@}

    //! @name Copy
    //@{
    PopulationAttribute& operator=( const PopulationAttribute& ); //!< Assignment operator
    //@}

private:
    //! @name Copy
    //@{
    PopulationAttribute( const PopulationAttribute& from );
    //@}

private:
    //! @name
    //@{
    double density_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( PopulationAttribute )

#endif //__PopulationAttribute_h_