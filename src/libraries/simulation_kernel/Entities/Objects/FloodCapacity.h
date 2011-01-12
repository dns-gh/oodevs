// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#ifndef __FloodCapacity_h_
#define __FloodCapacity_h_

#include "MIL.h"
#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

class PHY_HumanWound;

// =============================================================================
/** @class  FloodCapacity
    @brief  FloodCapacity
*/
// Created: JSR 2010-12-15
// =============================================================================
class FloodCapacity : public ObjectCapacity_ABC
                    , public MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             FloodCapacity();
    explicit FloodCapacity( xml::xistream& xis );
    virtual ~FloodCapacity();
    //@}

    //! @name Operations
    //@{
    BOOST_SERIALIZATION_SPLIT_MEMBER()
    void load( MIL_CheckPointInArchive&, const unsigned int );
    void save( MIL_CheckPointOutArchive&, const unsigned int ) const;
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void Finalize( MIL_Object_ABC& /*object*/ );
    virtual void Register( MIL_Object_ABC& object );
    virtual void ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Types
    //@{
    typedef std::map< const PHY_HumanWound*, double > T_InjuryMap;
    typedef T_InjuryMap::const_iterator             CIT_InjuryMap;
    //@}

private:
    //! @name Helpers
    //@{
    void ReadInjury( xml::xistream& xis );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    FloodCapacity( const FloodCapacity& );            //!< Copy constructor
    FloodCapacity& operator=( const FloodCapacity& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    T_InjuryMap injuries_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( FloodCapacity )

#endif // __FloodCapacity_h_
