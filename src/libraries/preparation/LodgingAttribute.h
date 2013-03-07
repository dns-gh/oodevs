// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __LodgingAttribute_h_
#define __LodgingAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"

namespace kernel
{
    class Displayer_ABC;
    class PropertiesDictionary;
    class Entity_ABC;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  LodgingAttribute
    @brief  LodgingAttribute
*/
// Created: MARTIN 2011-05-02
// =============================================================================
class LodgingAttribute : public kernel::ObjectExtension
                       , public kernel::Serializable_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             LodgingAttribute( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
             LodgingAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );

    virtual ~LodgingAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void DisplayInTooltip( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeObjectAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Operations
    //@{
    void SetLodgingCapacity( unsigned int );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    LodgingAttribute( const LodgingAttribute& );            //!< Copy constructor
    LodgingAttribute& operator=( const LodgingAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dictionary, const kernel::Entity_ABC& entity );
    //@}

private:
    //! @name Member data
    //@{
    unsigned int lodgingCapacity_;
    //@}
};

#endif // __LodgingAttribute_h_
