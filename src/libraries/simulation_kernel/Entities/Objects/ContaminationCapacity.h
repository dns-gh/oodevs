// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __ContaminationCapacity_h_
#define __ContaminationCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"
#include <boost/ptr_container/ptr_vector.hpp>

namespace xml
{
    class xistream;
}

class MIL_NbcAgentType;
class MIL_Agent_ABC;
class TER_Localisation;
class MT_Vector2D;

// =============================================================================
/** @class  ContaminationCapacity
    @brief  ContaminationCapacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class ContaminationCapacity : public ObjectCapacity_ABC
                            , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ContaminationCapacity();
    explicit ContaminationCapacity( xml::xistream& xis );
    virtual ~ContaminationCapacity();
    //@}

     //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population );
    void DecontaminateZone( MIL_Object_ABC& object, const TER_Localisation& zone );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ContaminationCapacity( const ContaminationCapacity& );  //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    bool IsInsideDecontaminatedZone( const MT_Vector2D& position ) const;
    //@}

    //! @name Types
    //@{
    typedef boost::ptr_vector< TER_Localisation >  T_LocalisationVector;
    typedef T_LocalisationVector::const_iterator CIT_LocalisationVector;
    //@}

private:
    //! @name
    //@{
    int maxToxic_;
    T_LocalisationVector decontaminatedZones_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ContaminationCapacity )

#endif // __ContaminationCapacity_h_
