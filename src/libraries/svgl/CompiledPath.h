// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __CompiledPath_h_
#define __CompiledPath_h_

#include "Node.h"
#include "TesselatorBase.h"

namespace svg
{
    class PathCommand_ABC;

// =============================================================================
/** @class  CompiledPath
    @brief  CompiledPath
    // $$$$ AGE 2006-08-12: Ne pas hériter de TesselatorBase
*/
// Created: AGE 2006-08-12
// =============================================================================
class CompiledPath : public Node, public TesselatorBase
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit CompiledPath( const Node& rhs, unsigned int fillRule );
    virtual ~CompiledPath();
    //@}

    //! @name Operations
    //@{
    virtual Node& CompileInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    virtual void DrawInternal( RenderingContext_ABC& context, References_ABC& references ) const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    CompiledPath( const CompiledPath& );            //!< Copy constructor
    CompiledPath& operator=( const CompiledPath& ); //!< Assignement operator
    //@}

private:
    //! @name Types
    //@{
    typedef std::vector< geometry::Point >            T_Points;
    struct T_Border {
        T_Border( unsigned short first ) : first_( first ) {};
        unsigned int type_; unsigned short first_, last_;
    };
    typedef std::vector< T_Border >                     T_Borders;
    typedef std::vector< unsigned short >               T_Indices;
    struct T_Part { 
        T_Part( unsigned int type ) : type_( type ) {};
        unsigned int type_; T_Indices indices_;
    };
    typedef std::vector< T_Part >                       T_Parts;
    //@}

    //! @name Helpers
    //@{    
    virtual void StartPath();
    virtual void EndPath( bool close = true );

    virtual void Begin( unsigned int type );
    virtual void Vertex( const geometry::Point& point, unsigned int index );
    virtual void End();

    void LockArray() const;
    //@}

private:
    //! @name Member data
    //@{
    unsigned int fillRule_;
    T_Borders borders_;
    T_Parts   parts_;
    //@}
};

}

#endif // __CompiledPath_h_
