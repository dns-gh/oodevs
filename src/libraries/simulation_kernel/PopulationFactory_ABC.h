// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __PopulationFactory_ABC_h_
#define __PopulationFactory_ABC_h_

#include "tools/Resolver.h"
#include "Entities/Populations/MIL_Population.h"

namespace xml
{
    class xistream;
}

class MIL_Army_ABC;
class UrbanObjectWrapper;

// =============================================================================
/** @class  PopulationFactory_ABC
    @brief  PopulationFactory_ABC
*/
// Created: MGD 2009-10-24
// =============================================================================
class PopulationFactory_ABC : public tools::Resolver< MIL_Population >
                            , private boost::noncopyable
{
public:
    //! @name Destructor
    //@{
    virtual ~PopulationFactory_ABC()
    {
        tools::Resolver< MIL_Population >::DeleteAll();
    }
    //@}

    //! @name Operations
    //@{
    virtual MIL_Population& Create( xml::xistream& xis, MIL_Army_ABC& army ) = 0;
    virtual MIL_Population& Create( const std::string& type, const MT_Vector2D& point, int number, const std::string& name, MIL_Army_ABC& formation, UrbanObjectWrapper* pUrbanObject, unsigned int context ) = 0;
    MIL_Population* FindByUrbanObject( UrbanObjectWrapper* urbanObject ) const;
    //@}

    //! @name CheckPoint
    //@{
    template< typename Archive > void serialize( Archive& file, const unsigned int );
    //@}

protected:
    //! @name Constructor
    //@{
    PopulationFactory_ABC()
    {
        // NOTHING
    }

    // $$$$ _RC_ JSR 2011-03-23: A nettoyer, trouver un meilleur moyen pour faire ça
    tools::Resolver< MIL_Population, UrbanObjectWrapper* > populationFromUrbanObjectResolver_;
    //@}
};

// -----------------------------------------------------------------------------
// Name: template< typename Archive > void PopulationFactory_ABC::serialize
// Created: MGD 2009-10-24
// -----------------------------------------------------------------------------
template< typename Archive >
void PopulationFactory_ABC::serialize( Archive& file, const unsigned int )
{
    file & elements_;
}

// -----------------------------------------------------------------------------
// Name: PopulationFactory_ABC::Find
// Created: BCI 2011-03-17
// -----------------------------------------------------------------------------
inline
MIL_Population* PopulationFactory_ABC::FindByUrbanObject( UrbanObjectWrapper* urbanObject ) const
{
    return populationFromUrbanObjectResolver_.Find( urbanObject );
}

#endif // __PopulationFactory_ABC_h_
