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

namespace kernel
{
    class PropertiesDictionary;
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
             DisasterAttribute( kernel::PropertiesDictionary& dictionary, const QString& source, const QTime& time );
             DisasterAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary );
    virtual ~DisasterAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary );
    //@}

private:
    //! @name Member data
    //@{
    QString source_;
    QTime time_;
    //@}
};

#endif // DisasterAttribute_h
