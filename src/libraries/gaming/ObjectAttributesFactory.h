// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectAttributesFactory_h_
#define __ObjectAttributesFactory_h_

namespace Common
{
    class ObjectAttributes;
}

namespace kernel
{
    class Controllers;
    class Entity_ABC;
}

class Model;
class StaticModel;

// =============================================================================
/** @class  ObjecAttributesFactory
    @brief  ObjecAttributesFactory
    // $$$$ AGE 2006-08-02: abc
*/
// Created: AGE 2006-02-14
// =============================================================================
class ObjectAttributesFactory
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectAttributesFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~ObjectAttributesFactory();
    //@}

    //! @name Operations
    //@{
    void Register( kernel::Entity_ABC& entity, const Common::ObjectAttributes& attributes ) const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ObjectAttributesFactory( const ObjectAttributesFactory& );            //!< Copy constructor
    ObjectAttributesFactory& operator=( const ObjectAttributesFactory& ); //!< Assignment operator
    //@}

private:
    //! @name Member data
    //@{
    kernel::Controllers& controllers_;
    Model& model_;
    const StaticModel& static_;
    //@}
};

#endif // __ObjectAttributesFactory_h_
