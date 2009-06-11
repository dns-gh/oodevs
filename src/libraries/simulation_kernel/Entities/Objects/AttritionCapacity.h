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

class Object;
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
    void load( MIL_CheckPointInArchive&, const uint );
    void save( MIL_CheckPointOutArchive&, const uint ) const;
    //@}

	//! @name Operations
    //@{
    virtual void Instanciate( Object& object ) const;	
    virtual void Register( Object& object );

    virtual void ProcessAgentMovingInside( Object& object, MIL_Agent_ABC& agent );
    virtual void ProcessPopulationMovingInside( Object& object, MIL_PopulationElement_ABC& agent );
    //@}

    //! @name Accessors
    //@{
    const PHY_AttritionData& GetAttritionData( const PHY_Protection& protection ) const;
    MT_Float GetAttritionSurface() const;
    MT_Float GetAttritionPH() const;
    //@}

private:
    //! @name Helpers
    //@{
    bool HasInteractionCapabilities( Object& object ) const;
    //@}

    //! @name types
    //@{
    struct PopulationAttrition 
    {
        PopulationAttrition() : surface_( 0. ), ph_( 0 ) {}
        MT_Float surface_;
        MT_Float ph_;
    };
    //@}

private:
    //! @name 
    //@{
    std::string category_;
    PopulationAttrition population_;
    const PHY_DotationCategory* dotation_;
    //@}
};

#endif