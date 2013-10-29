// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __AttritionCapacity_h_
#define __AttritionCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"
#include <boost/serialization/export.hpp>

namespace xml
{
    class xistream;
}

class MIL_Object_ABC;
class PHY_DotationCategory;
class PHY_Protection;
class PHY_AttritionData;
class MIL_PopulationElement_ABC;
class MIL_Agent_ABC;

// =============================================================================
/** @class  AttritionCapacity
    @brief  AttritionCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class AttritionCapacity : public ObjectCapacity_ABC
                        , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AttritionCapacity();
             AttritionCapacity( const AttritionCapacity& from );
    explicit AttritionCapacity( xml::xistream& xis );
    virtual ~AttritionCapacity();
    //@}

    //! @name CheckPoints
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );

    virtual void ProcessAgentMovingInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& agent );
    //@}

    //! @name Accessors
    //@{
    const PHY_AttritionData& GetAttritionData( const PHY_Protection& protection ) const;
    double GetAttritionSurface() const;
    double GetAttritionPH() const;
    //@}

private:
    //! @name types
    //@{
    struct PopulationAttrition
    {
        PopulationAttrition() : surface_( 0. ), ph_( 0 ) {}
        double surface_;
        double ph_;
    };
    //@}

private:
    //! @name
    //@{
    PopulationAttrition population_;
    const PHY_DotationCategory* dotation_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( AttritionCapacity )

#endif
