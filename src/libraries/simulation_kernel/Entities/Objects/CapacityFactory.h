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

namespace legacy
{
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

    //! @name Operations
    //@{
    virtual void Create( ObjectPrototype& object, const std::string& capacity, xml::xistream& xis ) const;
    virtual void Update( MIL_Object_ABC& object, const std::string& capacity, xml::xistream& xis ) const;
    virtual void FinalizeCreate( ObjectPrototype& object ) const;
    //@}

private:
    //! @name Types
    //@{
    typedef ObjectComponentRegistry_ABC< ObjectPrototype >::T_CallBack   Prototype_CallBack;
    typedef std::function< void( ObjectPrototype& ) >                  FinalizePrototype_CallBack;
    typedef ObjectComponentRegistry_ABC< MIL_Object_ABC >::T_CallBack    Object_CallBack;
    typedef std::map< std::string, Prototype_CallBack >                  Prototype_CallBacks;
    typedef Prototype_CallBacks::const_iterator                        CIPrototype_Callbacks;
    typedef std::vector< FinalizePrototype_CallBack >                    FinalizePrototype_CallBacks;
    typedef std::map< std::string, Object_CallBack >                     Object_CallBacks;
    typedef Object_CallBacks::const_iterator                           CIObject_Callbacks;
    //@}

protected:
    //! @name Helpers
    //@{
    virtual void DoRegister( const std::string& capacity, const Prototype_CallBack& prototypeCallback );
    //@}

private:
    //! @name Registration
    //@{
    virtual void Register( const std::string& capacity, const Prototype_CallBack& callback );
    virtual void Register( const std::string& capacity, const Object_CallBack& callback );
    //@}

    //! @name Helpers
    //@{
    void RegisterFinalizeCreate( FinalizePrototype_CallBack finalizePrototypeCallback );
    void DoRegister( const std::string& capacity, const Prototype_CallBack& prototypeCallback, const Object_CallBack& objectCallback );
    //@}

private:
    //! @name Members
    //@{
    Prototype_CallBacks prototypeCallbacks_;
    FinalizePrototype_CallBacks finalizePrototypeCallbacks_;
    Object_CallBacks objectCallbacks_;
    //@}
};

}
#endif // __Object_CapacityFactory_h_
