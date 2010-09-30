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
    explicit PopulationAttribute( xml::xistream& xis );
    virtual ~PopulationAttribute();
    //@}

    //! @name Operations
    //@{
    MT_Float GetDensity() const;
    void  Reset();
    void  SetDensity( MT_Float rDensity );
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
    MT_Float density_;
    //@}
};

#endif //__PopulationAttribute_h_