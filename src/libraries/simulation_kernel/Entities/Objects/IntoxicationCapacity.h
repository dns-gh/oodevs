// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __IntoxicationCapacity_h_
#define __IntoxicationCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;
class TER_Localisation;
class MT_Vector2D;

// =============================================================================
/** @class  IntoxicationCapacity
    @brief  IntoxicationCapacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class IntoxicationCapacity : public ObjectCapacity_ABC
                           , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             IntoxicationCapacity();
    explicit IntoxicationCapacity( xml::xistream& xis );
    virtual ~IntoxicationCapacity();
    //@}

     //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@{

     //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population );
    void DesintoxicateZone( MIL_Object_ABC& object, const TER_Localisation& zone );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    IntoxicationCapacity( const IntoxicationCapacity& );  //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    bool IsInsideDesintoxicatedZone( const MT_Vector2D& position ) const;
    //@}

    //! @name Types
    //@{
    typedef boost::ptr_vector< TER_Localisation >  T_LocalisationVector;
    typedef T_LocalisationVector::const_iterator CIT_LocalisationVector;
    //@}

private:
    //! @name Member data
    //@{
    int maxToxic_;
    T_LocalisationVector desintoxicatedZones_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( IntoxicationCapacity )

#endif // __IntoxicationCapacity_h_
