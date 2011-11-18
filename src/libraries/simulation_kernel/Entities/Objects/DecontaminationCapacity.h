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
#include "MIL_InteractiveContainer_ABC.h"
#include <deque>
#include <vector>

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;
class MIL_PopulationElement_ABC;
class MIL_Population;

// =============================================================================
/** @class  DecontaminationCapacity
    @brief  DecontaminationCapacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class DecontaminationCapacity : public ObjectCapacity_ABC
                              , public MIL_InteractiveContainer_ABC
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

    virtual void ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessPopulationInside( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population );

    void QueueForDecontamination( MIL_Agent_ABC& agent );
    void QueueForDecontamination( MIL_Population& population );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DecontaminationCapacity( const DecontaminationCapacity& );  //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    bool Decontaminate( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    void Decontaminate( MIL_Object_ABC& object, MIL_PopulationElement_ABC& population );
    MIL_PopulationElement_ABC* GetElement( MIL_Population* population );
    //@}

    //! @name Types
    //@{
    typedef std::deque< MIL_Agent_ABC* > T_AgentQueue;
    typedef std::deque< MIL_Population* > T_PopulationQueue;

    typedef std::vector< MIL_Agent_ABC* > T_Agents;
    typedef std::vector< MIL_PopulationElement_ABC* > T_Populations;
    typedef T_Populations::iterator                  IT_Populations;
    //@}

private:
    //! @name Member data
    //@{
    T_AgentQueue agents_;
    T_Agents agentsInside_;
    T_PopulationQueue populations_;
    T_Populations populationsInside_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DecontaminationCapacity )

#endif // __Object_DecontaminationCapacity_h_
