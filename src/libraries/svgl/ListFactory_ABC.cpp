// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "ListFactory_ABC.h"
#include "RenderingContext_ABC.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: ListFactory_ABC constructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
ListFactory_ABC::ListFactory_ABC()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: ListFactory_ABC destructor
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
ListFactory_ABC::~ListFactory_ABC()
{
    for( CIT_Lists it = lists_.begin(); it != lists_.end(); ++it )
        glDeleteLists( it->second, 1 );
}

namespace 
{
    class ViewportContext : public RenderingContext_ABC
    {
    public:
        ViewportContext( const geometry::BoundingBox& viewport, unsigned w, unsigned h )
        {
            pixels_ =   std::sqrt( ( viewport.Width() * viewport.Width() + viewport.Height() * viewport.Height() ) )
                      / std::sqrt( ( float( w ) * w + float( h ) * h ) );
            expectedPrecision_ = 10 * pixels_;
        }
        virtual float ExpectedPrecision() const { return expectedPrecision_; }
        virtual float Pixels() const            { return pixels_; }
        virtual float LineWidthFactor() const { return 1.f;}

        virtual bool SetupFill( References_ABC& ) const { return false; };
        virtual bool SetupStroke( References_ABC& ) const { return false; };
        virtual void PushProperty( E_Properties , Property_ABC& ) {};
        virtual void PopProperty ( E_Properties ) {};
        virtual const Property_ABC* FindProperty( E_Properties ) const { return 0; }
        virtual bool IsPickingMode() const { return false; };
        virtual void EnablePickingMode( float /*lineWithFactor*/ ) {};
        virtual void DisablePickingMode() {};

    private:
        float pixels_, expectedPrecision_;
    };
}

// -----------------------------------------------------------------------------
// Name: ListFactory_ABC::SetViewport
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
void ListFactory_ABC::SetViewport( const geometry::BoundingBox& viewport, unsigned w, unsigned h )
{
    for( CIT_Lists it = lists_.begin(); it != lists_.end(); ++it )
    {
        glNewList( it->second, GL_COMPILE );
        ViewportContext context( viewport, w, h );
        CompileList( it->first, context );
        glEndList();
    }
}

// -----------------------------------------------------------------------------
// Name: ListFactory_ABC::CreateProperty
// Created: AGE 2007-10-30
// -----------------------------------------------------------------------------
Property_ABC* ListFactory_ABC::CreateProperty( const std::string& content ) const
{
    if( content.empty() || content == "inherit" )
        return 0;
    if( IsFixed( content ) )
        return CreateBaseProperty( content );
    unsigned& list = lists_[ content ];
    if( ! list )
        list = glGenLists( 1 );
    return CreateListProperty( list );
}
