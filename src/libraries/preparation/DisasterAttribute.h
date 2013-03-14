// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef DisasterAttribute_h
#define DisasterAttribute_h

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/SubTypes.h"

namespace kernel
{
    class PropertiesDictionary;
    class Entity_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  DisasterAttribute
    @brief  Disaster attribute
*/
// Created: LGY 2012-10-05
// =============================================================================
class DisasterAttribute : public kernel::DisasterAttribute_ABC
                        , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             DisasterAttribute( kernel::PropertiesDictionary& dictionary, const tools::Path& source, const QDateTime& date, const kernel::Entity_ABC& entity );
             DisasterAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~DisasterAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    kernel::DisasterDirectory source_;
    QDateTime date_;
    //@}
};

#endif // DisasterAttribute_h
