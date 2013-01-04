// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __AvoidanceCapacity_h_
#define __AvoidanceCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "simulation_terrain/TER_DynamicPathDataHandler.h"
#include "simulation_terrain/TER_Localisation.h"

class TER_Localisation;
class TER_DynamicData;

// =============================================================================
/** @class  AvoidanceCapacity
    @brief  AvoidanceCapacity
*/
// Created: JCR 2008-05-30
// =============================================================================
class AvoidanceCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             AvoidanceCapacity();
             AvoidanceCapacity( const AvoidanceCapacity& from );
    explicit AvoidanceCapacity( xml::xistream& xis );
    virtual ~AvoidanceCapacity();
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
    virtual void Finalize( MIL_Object_ABC& object );
    //@}

    //! @name Accessors
    //@{
    const TER_Localisation& GetLocalisation() const;
    const double GetDistance() const;
    //@}

private:
    //! @name Copy
    //@{
    AvoidanceCapacity& operator=( const AvoidanceCapacity& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeArea( MIL_Object_ABC& object );
    void ResetDynamicData( MIL_Object_ABC& object );
    //@}

private:
    //! @name Member data
    //@{
    TER_Localisation           avoid_;
    TER_DynamicPathDataHandler handler_;
    double                     distance_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( AvoidanceCapacity )

#endif // __AvoidanceCapacity_h_
