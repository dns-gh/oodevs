// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __Object_DecontaminationCapacity_h_
#define __Object_DecontaminationCapacity_h_

#include "ObjectCapacity_ABC.h"
#include <deque>

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;
class MIL_PopulationElement_ABC;

// =============================================================================
/** @class  DecontaminationCapacity
    @brief  DecontaminationCapacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class DecontaminationCapacity : public ObjectCapacity_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DecontaminationCapacity();
    explicit DecontaminationCapacity( xml::xistream& xis );
    virtual ~DecontaminationCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    //@}

    //! @name Operations
    //@{
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Register( MIL_Object_ABC& object );
    virtual void Update( MIL_Object_ABC& object, unsigned int time );
    void QueueForDecontamination( MIL_Agent_ABC& agent );
    void QueueForDecontamination( MIL_PopulationElement_ABC& population );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DecontaminationCapacity( const DecontaminationCapacity& );  //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    bool Decontaminate( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    bool Decontaminate( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population );
    //@}

    //! @name Types
    //@{
    typedef std::deque< MIL_Agent_ABC* > T_AgentQueue;
    typedef std::deque< MIL_PopulationElement_ABC* > T_PopulationQueue;
    //@}

private:
    //! @name Member data
    //@{
    T_AgentQueue agents_;
    T_PopulationQueue populations_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DecontaminationCapacity )

#endif // __Object_DecontaminationCapacity_h_
