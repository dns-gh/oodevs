// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Object_CapacityFactory_h_
#define __Object_CapacityFactory_h_

#include "ObjectComponentRegistry_ABC.h"
#include "CapacityFactory_ABC.h"

class ObjectPrototype;
class MIL_Object_ABC;
class MIL_PropagationManager;

// =============================================================================
/** @class  CapacityFactory
    @brief  CapacityFactory
*/
// Created: JCR 2008-05-30
// =============================================================================
class CapacityFactory : public ObjectComponentRegistry_ABC< ObjectPrototype >
                      , public ObjectComponentRegistry_ABC< MIL_Object_ABC >
                      , public CapacityFactory_ABC
{
public:
    //! @name Constructors/Destructor
    //@{
             CapacityFactory();
    virtual ~CapacityFactory();
    //@}

    //! @name Creation/Update
    //@{
    void Create( ObjectPrototype& object, const std::string& capacity, xml::xistream& xis );
    void Update( MIL_Object_ABC& object, const std::string& capacity, xml::xistream& xis ) const;
    //@}

    //! @name Accessors
    //@{
    MIL_PropagationManager* GetPropagationManager() const;
    //@}

private:
    //! @name Types
    //@{
    typedef ObjectComponentRegistry_ABC< ObjectPrototype >::T_CallBack   Prototype_CallBack;
    typedef ObjectComponentRegistry_ABC< MIL_Object_ABC >::T_CallBack    Object_CallBack;
    typedef std::map< std::string, Prototype_CallBack >                  Prototype_CallBacks;
    typedef Prototype_CallBacks::const_iterator                        CIPrototype_Callbacks;
    typedef std::map< std::string, Object_CallBack >                     Object_CallBacks;
    typedef Object_CallBacks::const_iterator                           CIObject_Callbacks;
    //@}
private:
    //! @name Registration
    //@{
    virtual void Register( const std::string& capacity, const Prototype_CallBack& callback );
    virtual void Register( const std::string& capacity, const Object_CallBack& callback );
    //@}

    //! @name Helpers
    //@{
    void DoRegister( const std::string& capacity, const Prototype_CallBack& prototypeCallback );
    void DoRegister( const std::string& capacity, const Prototype_CallBack& prototypeCallback, const Object_CallBack& objectCallback );
    //@}

private:
    //! @name Members
    //@{
    Prototype_CallBacks prototypeCallbacks_;
    Object_CallBacks objectCallbacks_;
    std::auto_ptr< MIL_PropagationManager > propagation_;
    //@}
};

#endif // __Object_CapacityFactory_h_