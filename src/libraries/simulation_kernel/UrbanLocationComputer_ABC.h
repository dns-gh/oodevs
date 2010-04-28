// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef __UrbanLocationComputer_ABC_h_
#define __UrbanLocationComputer_ABC_h_

namespace urban
{
    class TerrainObject_ABC;
}

namespace urbanLocation
{

// =============================================================================
/** @class  UrbanLocationComputer_ABC
    @brief  UrbanLocationComputer_ABC
*/
// Created: SLG 2010-04-12
// =============================================================================
class UrbanLocationComputer_ABC
{
public:

    struct Results
    {
        float urbanDeployment_;
        geometry::Point2f position_;
        Results();
    };


public:

    //! @name Constructors/Destructor
    //@{
             UrbanLocationComputer_ABC(){}
    virtual ~UrbanLocationComputer_ABC(){}
    //@}

    //! @name Operations
    //@{
    virtual void SetUrbanDeployment ( float urbanDeployment ) = 0;
    virtual void SetPosition        ( geometry::Point2f position ) = 0;
    virtual Results& Result() = 0;
    //@}
};

} // namespace urbanLocation

#endif // __UrbanLocationComputer_ABC_h_
