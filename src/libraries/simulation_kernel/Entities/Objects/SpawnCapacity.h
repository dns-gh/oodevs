// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __SpawnCapacity_h_
#define __SpawnCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

class MIL_Agent_ABC;
class TER_Localisation;
// =============================================================================
/** @class  SpawnCapacity
@brief  SpawnCapacity
*/
// Created: MGD 2009-03-05
// =============================================================================
class SpawnCapacity : public ObjectCapacity_ABC
    , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    SpawnCapacity();
    explicit SpawnCapacity( xml::xistream& xis );
    virtual ~SpawnCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );

    void AddCreator( MIL_Object_ABC& object, const MIL_Agent_ABC& agent );
    //@}

    //! @name Helpers
    //@{
    void ReadAcquisitionTime( xml::xistream& xis );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    SpawnCapacity( const SpawnCapacity& );  //!< Copy constructor
    //@}

private:
    //! @name Members
    //@{
    float rActionRange_;
    std::string childType_;
    //@}

};

#endif // __SpawnCapacity_h_
