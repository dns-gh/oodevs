// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Gradient_h_
#define __Gradient_h_

namespace xml 
{
    class xistream;
}

namespace gui
{

// =============================================================================
/** @class  Gradient
    @brief  Gradient
*/
// Created: AGE 2007-07-02
// =============================================================================
class Gradient
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit Gradient( xml::xistream& xis );
    virtual ~Gradient();
    //@}

    //! @name Operations
    //@{
    void MakeGlTexture( float alpha );
    unsigned Length() const;
    float UsedRatio() const;
    //@}

private:
    //! @name Copy/Assignment
    //@{
    Gradient( const Gradient& );            //!< Copy constructor
    Gradient& operator=( const Gradient& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    void LoadValue( xml::xistream& xis );
    unsigned FindBaseDistance() const;
    //@}

    //! @name Types
    //@{
    typedef std::pair< float, QColor > T_Color;
    typedef std::vector< T_Color >     T_Colors;
    typedef T_Colors::const_iterator CIT_Colors;
    //@}

private:
    //! @name Member data
    //@{
    T_Colors colors_;
    unsigned textureSize_;
    float usedRatio_;
    //@}
};

}

#endif // __Gradient_h_
