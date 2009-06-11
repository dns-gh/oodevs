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

#include "ObjectCapacity_ABC.h"
#include "MIL_DynamicPathDataHandler.h"
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
    template< typename Archive > void serialize( Archive&, const uint );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( Object& object ) const;
    virtual void Register( Object& object );
    virtual void Finalize( Object& object );    
    //@}

    //! @name Accessors
    //@{
    const TER_Localisation& GetLocalisation() const;
    //@}

private:
    //! @name Copy
    //@{    
    AvoidanceCapacity& operator=( const AvoidanceCapacity& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void InitializeArea( Object& object );
    void ResetDynamicData( Object& object );
    //@}

private:
    //! @name Member data
    //@{
    TER_Localisation            avoid_;
    MIL_DynamicPathDataHandler  handler_;
    MT_Float                    distance_;
    //@}
};

#endif // __AvoidanceCapacity_h_
