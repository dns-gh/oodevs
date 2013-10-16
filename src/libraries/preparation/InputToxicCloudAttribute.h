// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __InputToxicCloudAttribute_h_
#define __InputToxicCloudAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include <tools/Resolver_ABC.h>

namespace kernel
{
    class Entity_ABC;
}

namespace gui
{
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  InputToxicCloudAttribute
    @brief  InputToxicCloudAttribute
*/
// Created: AGE 2006-02-14
// =============================================================================
class InputToxicCloudAttribute : public kernel::ToxicCloudAttribute_ABC
                               , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             InputToxicCloudAttribute( gui::PropertiesDictionary& dictionary, const QString& source,
                                       const QString& dataField, bool bExport, const kernel::Entity_ABC& entity );
             InputToxicCloudAttribute( xml::xistream& xis, gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    virtual ~InputToxicCloudAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    InputToxicCloudAttribute( const InputToxicCloudAttribute& );            //!< Copy constructor
    InputToxicCloudAttribute& operator=( const InputToxicCloudAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( gui::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    QString source_;
    QString dataField_;
    bool bExport_;
    //@}
};

#endif // __InputToxicCloudAttribute_h_
