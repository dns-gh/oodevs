// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticLevelAttribute_h_
#define __LogisticLevelAttribute_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/SubTypes.h"

namespace kernel
{
    class Controller;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  LogisticLevelAttribute
    @brief  Logistic level attritube
*/
// Created: LGY 2011-07-20
// =============================================================================
class LogisticLevelAttribute: public kernel::Extension_ABC
                            , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticLevelAttribute( kernel::Controller& controller, const kernel::Entity_ABC& entity, bool active, kernel::PropertiesDictionary& dictionary );
             LogisticLevelAttribute( kernel::Controller& controller, const kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dictionary );
             LogisticLevelAttribute( kernel::Controller& controller, xml::xistream& xis, const kernel::Entity_ABC& entity, bool active, kernel::PropertiesDictionary& dictionary );
    virtual ~LogisticLevelAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    const kernel::LogisticLevel& GetLogisticLevel() const;
    const kernel::Entity_ABC& GetEntity() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticLevelAttribute( const LogisticLevelAttribute& );            //!< Copy constructor
    LogisticLevelAttribute& operator=( const LogisticLevelAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary, bool active );
    void SetLogisticLevel( const kernel::EntityLogisticLevel& logisticLevel );
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controller& controller_;
    const kernel::Entity_ABC& entity_;
    kernel::EntityLogisticLevel logisticLevel_;
    //@}
};

#endif // __LogisticLevelAttribute_h_
