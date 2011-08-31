// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActivityTimeAttribute_h_
#define __ActivityTimeAttribute_h_

#include "clients_kernel/ObjectExtensions.h"
#include "clients_kernel/Serializable_ABC.h"
#include "clients_kernel/Units.h"

namespace kernel
{
    class Displayer_ABC;
    class PropertiesDictionary;
}

namespace xml
{
    class xistream;
}

// =============================================================================
/** @class  ActivityTimeAttribute
    @brief  ActivityTimeAttribute
*/
// Created: SBO 2007-02-08
// =============================================================================
class ActivityTimeAttribute : public kernel::TimeLimitedAttribute_ABC
                            , public kernel::Serializable_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit ActivityTimeAttribute( kernel::PropertiesDictionary& dico );
             ActivityTimeAttribute( xml::xistream& xis, kernel::PropertiesDictionary& dico );
    virtual ~ActivityTimeAttribute();
    //@}

    //! @name Operations
    //@{
    virtual void Display( kernel::Displayer_ABC& displayer ) const;
    virtual void SerializeAttributes( xml::xostream& xos ) const;
    //@}

    //! @name Modifiers
    //@{
    void SetActivityTime( unsigned int time );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActivityTimeAttribute( const ActivityTimeAttribute& );            //!< Copy constructor
    ActivityTimeAttribute& operator=( const ActivityTimeAttribute& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void CreateDictionary( kernel::PropertiesDictionary& dico );
    //@}

private:
    //! @name Member data
    //@{
    kernel::UnitedValue< unsigned int > activityTime_;
    //@}
};

#endif // __ActivityTimeAttribute_h_
