// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#include "svgl_pch.h"
#include "CompiledPath.h"
#include "RenderingContext_ABC.h"
#include "Paint_ABC.h"
#include "RenderingContext_ABC.h"
#include "Length.h"
#include "DashArray.h"
#include "gl_extensions.h"

using namespace svg;

// -----------------------------------------------------------------------------
// Name: CompiledPath constructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
CompiledPath::CompiledPath( const Node& rhs, unsigned int fillRule )
    :  Node( rhs )
    , TesselatorBase( fillRule )
    , fillRule_( fillRule )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompiledPath constructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
CompiledPath::CompiledPath( const CompiledPath& rhs )
    : Node( rhs )
    , TesselatorBase( rhs.fillRule_ )
    , borders_( rhs.borders_ )
    , parts_  ( rhs.parts_ )
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompiledPath destructor
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
CompiledPath::~CompiledPath()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: CompiledPath::CompileInternal
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
Node& CompiledPath::CompileInternal( RenderingContext_ABC& , References_ABC& ) const
{
    return *new CompiledPath( *this );
}

// -----------------------------------------------------------------------------
// Name: CompiledPath::LockArray
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void CompiledPath::LockArray() const
{
    glVertexPointer( 2, GL_FLOAT, 0, &GetPoints().front() );
    gl::lockArrays( 0, static_cast< GLsizei >( GetPoints().size() ));
}

// -----------------------------------------------------------------------------
// Name: CompiledPath::DrawInternal
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void CompiledPath::DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const
{
    bool arraySet = false;
    if( !GetPoints().empty() && context.SetupFill( references ) )
    {
        arraySet = true;
        LockArray();
        for( T_Parts::const_iterator it = parts_.begin(); it != parts_.end(); ++it )
            glDrawElements( it->type_, static_cast< GLsizei >( it->indices_.size() ),
                    GL_UNSIGNED_SHORT, &it->indices_.front() );
    }

    if( !GetPoints().empty() && context.SetupStroke( references ) )
    {
        if( ! arraySet )
            LockArray();
        arraySet = true;
        for( T_Borders::const_iterator it = borders_.begin(); it != borders_.end(); ++it )
            glDrawArrays( it->type_, it->first_, it->last_ - it->first_ );
    }

    if( arraySet )
        gl::unlockArrays();
}

// -----------------------------------------------------------------------------
// Name: CompiledPath::StartPath
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void CompiledPath::StartPath()
{
    TesselatorBase::StartPath();
    borders_.push_back( T_Border( unsigned short( GetPoints().size() ) ) );
}

// -----------------------------------------------------------------------------
// Name: CompiledPath::EndPath
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void CompiledPath::EndPath( bool close /*= true*/ )
{
    T_Border& border = borders_.back();
    border.type_ = close ? GL_LINE_LOOP : GL_LINE_STRIP;
    border.last_ = unsigned short( GetPoints().size() );
    TesselatorBase::EndPath( close );
}

// -----------------------------------------------------------------------------
// Name: CompiledPath::Begin
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void CompiledPath::Begin( unsigned int type )
{
    parts_.push_back( T_Part( type ) );
}

// -----------------------------------------------------------------------------
// Name: CompiledPath::Vertex
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void CompiledPath::Vertex( const geometry::Point& , unsigned int index )
{
    parts_.back().indices_.push_back( unsigned short( index ) );
}

// -----------------------------------------------------------------------------
// Name: CompiledPath::End
// Created: AGE 2006-08-12
// -----------------------------------------------------------------------------
void CompiledPath::End()
{
    // NOTHING
}
