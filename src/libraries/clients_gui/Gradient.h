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
    class xostream;
}

namespace gui
{

class GradientVisitor_ABC
{
public:
             GradientVisitor_ABC() {};
    virtual ~GradientVisitor_ABC() {}

    virtual void Visit( float position, const QColor& color ) = 0;
};


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
             Gradient();
    explicit Gradient( xml::xistream& xis );
    virtual ~Gradient();
    //@}

    //! @name Construction
    //@{
    void AddColor( float position, const QColor& color );
    //@}

    //! @name Operations
    //@{
    void SetName( const QString& name );
    QString GetName() const;
    void MakeGlTexture( float alpha );
    unsigned Length() const;
    float UsedRatio() const;
    void Save( xml::xostream& xos ) const;
    void Accept( GradientVisitor_ABC& visitor ) const;
    Gradient& operator=( const Gradient& rhs );
    //@}

private:
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
    QString name_;
    T_Colors colors_;
    unsigned textureSize_;
    float usedRatio_;
    //@}
};

}

#endif // __Gradient_h_
