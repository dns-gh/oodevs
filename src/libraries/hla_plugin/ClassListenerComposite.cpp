// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "hla_plugin_pch.h"
#include "ClassListenerComposite.h"
#include <algorithm>

using namespace plugins::hla;

// -----------------------------------------------------------------------------
// Name: ClassListenerComposite constructor
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
ClassListenerComposite::ClassListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassListenerComposite constructor
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
ClassListenerComposite::~ClassListenerComposite()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ClassListenerComposite::RemoteCreated
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ClassListenerComposite::RemoteCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object)
{
    std::for_each(listeners_.begin(), listeners_.end(), [&](ClassListener_ABC* listener)
        {
            listener->RemoteCreated( identifier, hlaClass, object );
        });
}

// -----------------------------------------------------------------------------
// Name: ClassListenerComposite::RemoteDestroyed
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ClassListenerComposite::RemoteDestroyed( const std::string& identifier)
{
    std::for_each(listeners_.begin(), listeners_.end(), [&](ClassListener_ABC* listener)
        {
            listener->RemoteDestroyed( identifier );
        });
}

// -----------------------------------------------------------------------------
// Name: ClassListenerComposite::Register
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ClassListenerComposite::Register( ClassListener_ABC& listener )
{
    listeners_.insert( &listener);
}

// -----------------------------------------------------------------------------
// Name: ClassListenerComposite::Unregister
// Created: AHC 2012-02-27
// -----------------------------------------------------------------------------
void ClassListenerComposite::Unregister( ClassListener_ABC& listener )
{
    listeners_.erase( &listener );
}

// -----------------------------------------------------------------------------
// Name: ClassListenerComposite::LocalCreated
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void ClassListenerComposite::LocalCreated( const std::string& identifier, HlaClass_ABC& hlaClass, HlaObject_ABC& object )
{
    std::for_each(listeners_.begin(), listeners_.end(), [&](ClassListener_ABC* listener)
        {
            listener->LocalCreated( identifier, hlaClass, object );
        });
}

// -----------------------------------------------------------------------------
// Name: ClassListenerComposite::LocalDestroyed
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void ClassListenerComposite::LocalDestroyed( const std::string& identifier )
{
    std::for_each(listeners_.begin(), listeners_.end(), [&](ClassListener_ABC* listener)
        {
            listener->LocalDestroyed( identifier );
        });
}

// -----------------------------------------------------------------------------
// Name: ClassListenerComposite::Divested
// Created: AHC 2010-03-02
// -----------------------------------------------------------------------------
void ClassListenerComposite::Divested( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes )
{
    std::for_each(listeners_.begin(), listeners_.end(), [&](ClassListener_ABC* listener)
        {
            listener->Divested( identifier, attributes );
        });
}

// -----------------------------------------------------------------------------
// Name: ClassListenerComposite::Acquired
// Created: AHC 2010-02-27
// -----------------------------------------------------------------------------
void ClassListenerComposite::Acquired( const std::string& identifier, const ::hla::T_AttributeIdentifiers& attributes )
{
    std::for_each(listeners_.begin(), listeners_.end(), [&](ClassListener_ABC* listener)
        {
            listener->Acquired( identifier, attributes );
        });
}
