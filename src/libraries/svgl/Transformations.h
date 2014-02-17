// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Transformations_h_
#define __Transformations_h_

#include "Matrix.h"

namespace svg
{
    class Transformation_ABC;

// =============================================================================
/** @class  Transformations
    @brief  Transformations
*/
// Created: AGE 2006-08-16
// =============================================================================
class Transformations
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Transformations( xml::xistream& input );
    virtual ~Transformations();
    //@}

    //! @name Constructions
    //@{
    static Transformations* Translation( float x, float y );
    //@}

    //! @name Operations
    //@{
    void Apply() const;
    void Undo() const;
    //@}

private:
    //! @name Copy/Assignement
    //@{
    Transformations();
    Transformations& operator=( const Transformations& ); //!< Assignement operator
    //@}

    //! @name Helpers
    //@{
    bool Transform( std::string& parameters );
    bool Multiply( const std::string& parameters );
    bool Translate( const std::string& parameters );
    bool Scale( const std::string& parameters );
    bool Rotate( const std::string& parameters );
    bool SkewOnX( const std::string& parameters );    
    bool SkewOnY( const std::string& parameters );    
    //@}

private:
    //! @name Member data
    //@{
    geometry::Matrix matrix_;
    bool isIdentity_;
    //@}
};

}

#endif // __Transformations_h_
