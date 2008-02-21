// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Hla_Coordinates_h_
#define __Hla_Coordinates_h_

// =============================================================================
/** @class  WorldLocation
    @brief  WorldLocation
*/
// Created: AGE 2008-02-21
// =============================================================================
class WorldLocation
{

public:
    //! @name Constructors/Destructor
    //@{
             WorldLocation( const std::string mgrs, float altitude );
    virtual ~WorldLocation();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive )
    {
        archive << x_ << y_ << z_;
    }
    //@}

    //! @name Accessors
    //@{
    double GetX() const { return x_; };
    double GetY() const { return y_; };
    double GetZ() const { return z_; };
    //@}

private:
    //! @name Member data
    //@{
    double x_, y_, z_;
    //@}
};

// =============================================================================
/** @class  VelocityVector
    @brief  VelocityVector
*/
// Created: AGE 2008-02-21
// =============================================================================
class VelocityVector
{

public:
    //! @name Constructors/Destructor
    //@{
             VelocityVector( const WorldLocation& location, float speed, unsigned short heading );
    virtual ~VelocityVector();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive )
    {
        archive << xv_ << yv_ << zv_;
    }
    //@}

    //! @name Accessors
    //@{
    double GetX() const { return xn_; };
    double GetY() const { return yn_; };
    double GetZ() const { return zn_; };
    //@}

private:
    //! @name Member data
    //@{
    float xn_, yn_, zn_; // normalized
    float xv_, yv_, zv_;
    //@}
};

// =============================================================================
/** @class  Orientation
    @brief  Orientation
*/
// Created: AGE 2008-02-21
// =============================================================================
class Orientation
{

public:
    //! @name Constructors/Destructor
    //@{
             Orientation( const WorldLocation& location, const VelocityVector& velocity );
    virtual ~Orientation();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive )
    {
        archive << psi_ << theta_ << phi;
    }
    //@}

private:
    //! @name Member data
    //@{
    float psi_, theta_, phi_;
    //@}
};


#endif // __Hla_Coordinates_h_
