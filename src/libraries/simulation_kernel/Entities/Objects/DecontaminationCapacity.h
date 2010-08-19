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

namespace xml
{
    class xistream;
}

class MIL_Agent_ABC;

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
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DecontaminationCapacity( const DecontaminationCapacity& );  //!< Copy constructor
    //@}

    //! @name Helpers
    //@{
    bool Decontaminate( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    //@}

    //! @name Types
    //@{
    typedef std::deque< MIL_Agent_ABC* > T_AgentQueue;
    //@}

private:
    //! @name Member data
    //@{
    T_AgentQueue decontaminationQueue_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( DecontaminationCapacity )

#endif // __Object_DecontaminationCapacity_h_
