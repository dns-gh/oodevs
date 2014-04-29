// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_ClassListenerComposite_h
#define plugins_hla_ClassListenerComposite_h

#include "ClassListener_ABC.h"
#include <set>

namespace plugins
{
namespace hla
{
class ClassListenerComposite : public ClassListener_ABC
{
public:
    ClassListenerComposite();
    virtual ~ClassListenerComposite();

    //! @name Operations
    //@{
    virtual void RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void RemoteDestroyed( const std::string& identifier );
    virtual void LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object );
    virtual void LocalDestroyed( const std::string& identifier );
    virtual void Divested( const std::string& identifier, const T_AttributeIdentifiers& attributes );
    virtual void Acquired( const std::string& identifier, const T_AttributeIdentifiers& attributes );

    virtual void Register( ClassListener_ABC& listener );
    virtual void Unregister( ClassListener_ABC& listener );
    //@}

private:
    typedef std::set< ClassListener_ABC* > T_Listeners;

private:
    T_Listeners listeners_;
};

}
}
#endif // plugins_hla_ClassListenerComposite_h
