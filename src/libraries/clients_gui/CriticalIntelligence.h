// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef gui_CriticalIntelligence_h
#define gui_CriticalIntelligence_h

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Updatable_ABC.h"
#include "clients_kernel/SubTypes.h"
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
    class Controller;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace gui
{
// =============================================================================
/** @class  CriticalIntelligence
    @brief  Critical intelligence
*/
// Created: LGY 2012-08-28
// =============================================================================
class CriticalIntelligence : public kernel::Extension_ABC
                           , public kernel::Serializable_ABC
                           , public kernel::Updatable_ABC< sword::CrowdUpdate >
                           , public kernel::Updatable_ABC< sword::UnitAttributes >
                           , private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             CriticalIntelligence( kernel::Entity_ABC& entity, kernel::Controller& controller,
                                   PropertiesDictionary& dictionary );
             CriticalIntelligence( xml::xistream& xis, kernel::Controller& controller, kernel::Entity_ABC& entity,
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
    kernel::CriticalIntelligenceType& GetType();
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
    kernel::Controller& controller_;
    kernel::Entity_ABC& entity_;
    const QString property_;
    kernel::CriticalIntelligenceType value_;
    //@}
};

}

#endif // gui_CriticalIntelligence_h
