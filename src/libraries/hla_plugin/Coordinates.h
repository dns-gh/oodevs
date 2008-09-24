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

namespace plugins
{
namespace hla
{

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
     WorldLocation();
     WorldLocation( const std::string mgrs, float altitude );
     WorldLocation( double latitude, double longitude, float altitude );
    ~WorldLocation();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << x_ << y_ << z_;
    }
    //@}

    //! @name Accessors
    //@{
    double X() const { return x_; };
    double Y() const { return y_; };
    double Z() const { return z_; };
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
     VelocityVector();
     VelocityVector( const WorldLocation& location, float speed, float heading );
    ~VelocityVector();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << xv_ << yv_ << zv_;
    }
    //@}

    //! @name Accessors
    //@{
    double X() const { return xn_; };
    double Y() const { return yn_; };
    double Z() const { return zn_; };

    double VX() const { return xv_; };
    double VY() const { return yv_; };
    double VZ() const { return zv_; };
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
     Orientation();
     Orientation( const WorldLocation& location, const VelocityVector& velocity );
    ~Orientation();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << psi_ << theta_ << phi_;
    }
    //@}

    //! @name Accessors
    //@{
    double Psi() const   { return psi_; };
    double Theta() const { return theta_; };
    double Phi() const   { return phi_; };
    //@}

private:
    //! @name Member data
    //@{
    float psi_, theta_, phi_;
    //@}
};

}
}

#endif // __Hla_Coordinates_h_
