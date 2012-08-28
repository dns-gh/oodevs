// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef kernel_CriticalIntelligence_h
#define kernel_CriticalIntelligence_h

#include "Extension_ABC.h"
#include "Serializable_ABC.h"
#include "Updatable_ABC.h"
#include "SubTypes.h"
#include <boost/noncopyable.hpp>

namespace xml
{
    class xistream;
}

namespace sword
{
    class CrowdUpdate;
    class UnitAttributes;
}

namespace kernel
{
    class Entity_ABC;
    class PropertiesDictionary;
    class Controller;
// =============================================================================
/** @class  CriticalIntelligence
    @brief  Critical intelligence
*/
// Created: LGY 2012-08-28
// =============================================================================
class CriticalIntelligence : public Extension_ABC
                           , public Serializable_ABC
                           , public Updatable_ABC< sword::CrowdUpdate >
                           , public Updatable_ABC< sword::UnitAttributes >
                           , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             CriticalIntelligence( Entity_ABC& entity, Controller& controller,
                                   PropertiesDictionary& dictionary );
             CriticalIntelligence( xml::xistream& xis, Controller& controller, Entity_ABC& entity,
                                   PropertiesDictionary& dictionary );
    virtual ~CriticalIntelligence();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    virtual void DoUpdate( const sword::CrowdUpdate& message );
    virtual void DoUpdate( const sword::UnitAttributes& message );
    //@}

    //! @name Accessors
    //@{
    CriticalIntelligenceType& GetType();
    //@}

private:
    //! @name Helpers
    //@{
    template< typename T >
    void UpdateData( const T& message );
    //@}

private:
    //! @name Member data
    //@{
    Controller& controller_;
    Entity_ABC& entity_;
    const QString property_;
    CriticalIntelligenceType value_;
    //@}
};

}

#endif // kernel_CriticalIntelligence_h
