// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LogisticLevelAttritube_h_
#define __LogisticLevelAttritube_h_

#include "clients_kernel/Extension_ABC.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/SubTypes.h"

namespace kernel
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  LogisticLevelAttritube
    @brief  Logistic level attritube
*/
// Created: LGY 2011-07-20
// =============================================================================
class LogisticLevelAttritube: public kernel::Extension_ABC
                            , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             LogisticLevelAttritube( const kernel::Entity_ABC& entity, bool active, kernel::PropertiesDictionary& dictionary );
             LogisticLevelAttritube( const kernel::Entity_ABC& entity, kernel::PropertiesDictionary& dictionary );
             LogisticLevelAttritube( xml::xistream& xis, const kernel::Entity_ABC& entity, bool active, kernel::PropertiesDictionary& dictionary );
    virtual ~LogisticLevelAttritube();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    const kernel::LogisticLevel& GetLogisticLevel() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LogisticLevelAttritube( const LogisticLevelAttritube& );            //!< Copy constructor
    LogisticLevelAttritube& operator=( const LogisticLevelAttritube& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary, bool active );
    void SetLogisticLevel( const kernel::EntityLogisticLevel& logisticLevel );
    //@}

private:
    //! @name Member data
    //@{
    const kernel::Entity_ABC& entity_;
    kernel::EntityLogisticLevel logisticLevel_;
    //@}
};

#endif // __LogisticLevelAttritube_h_
