// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ProtectionCapacity_h_
#define __ProtectionCapacity_h_

#include "ObjectCapacity_ABC.h"
#include "MIL_InteractiveContainer_ABC.h"

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ProtectionCapacity
    @brief  ProtectionCapacity
*/
// Created: JCR 2008-06-02
// =============================================================================
class ProtectionCapacity : public ObjectCapacity_ABC
                         , private MIL_InteractiveContainer_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             ProtectionCapacity();
    explicit ProtectionCapacity( xml::xistream& xis );
    virtual ~ProtectionCapacity();
    //@}

    //! @name Operations
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int );
    virtual void Register( MIL_Object_ABC& object );
    virtual void Instanciate( MIL_Object_ABC& object ) const;
    virtual void ProcessAgentInside( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentEntering( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    virtual void ProcessAgentExiting( MIL_Object_ABC& object, MIL_Agent_ABC& agent );
    //@}

private:
    //! @name Copy
    //@{
    ProtectionCapacity( const ProtectionCapacity& from );
    //@}

private:
    //! @name Member data
    //@{
    std::size_t maxSize_;
    bool bGeniePrepared_;
    std::set< const MIL_Agent_ABC* > agents_;
    //@}
};

BOOST_CLASS_EXPORT_KEY( ProtectionCapacity )

#endif
