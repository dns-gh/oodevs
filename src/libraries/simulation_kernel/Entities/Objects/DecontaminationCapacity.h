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
//#include "MIL_InteractiveContainer_ABC.h"

class MIL_Agent_ABC;

// =============================================================================
/** @class  DecontaminationCapacity
    @brief  DecontaminationCapacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class DecontaminationCapacity  
    : public ObjectCapacity_ABC
    // , public MIL_InteractiveContainer_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit DecontaminationCapacity( xml::xistream& xis );
             DecontaminationCapacity();
    virtual ~DecontaminationCapacity();
    //@}

    //! @name CheckPoints
    //@{
    template< typename Archive > void serialize( Archive&, const uint );
    virtual void Register( Object& /*object*/ );
    //@}

	//! @name virtual
    //@{
    virtual void Instanciate( Object& object ) const;
	virtual void Update( Object& object, uint time );
	//@}

    //! @name 
    //@{
    void QueueForDecontamination( MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    DecontaminationCapacity( const DecontaminationCapacity& );  //!< Copy constructor
    //@}

    //! @name Helper
    //@{
    bool Decontaminate( Object& object, MIL_Agent_ABC& agent );
    //@}

private:
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

#endif // __Object_DecontaminationCapacity_h_
