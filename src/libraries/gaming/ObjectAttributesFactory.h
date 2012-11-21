// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Math�matiques Appliqu�es SA (MASA)
//
// *****************************************************************************

#ifndef __ObjectAttributesFactory_h_
#define __ObjectAttributesFactory_h_

namespace sword
{
    class ObjectAttributes;
}

namespace kernel
{
    class Controllers;
    class Object_ABC;
    class ObjectKnowledge_ABC;
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
class ObjectAttributesFactory : private boost::noncopyable
{
public:
    //! @name Constructors/Destructor
    //@{
             ObjectAttributesFactory( kernel::Controllers& controllers, Model& model, const StaticModel& staticModel );
    virtual ~ObjectAttributesFactory();
    //@}

    //! @name Operations
    //@{
    void Register( kernel::Object_ABC& entity, const sword::ObjectAttributes& attributes ) const;
    void Register( kernel::ObjectKnowledge_ABC& entity, const sword::ObjectAttributes& attributes ) const;
    //@}

private:
    //! @name Helpers
    //@{
    void Register( kernel::Entity_ABC& entity, const sword::ObjectAttributes& attributes ) const;
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
