// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2009 MASA Group
//
// *****************************************************************************

#ifndef SWORD_PERCEPTION_PERCEPTION_LOCALISATION_H
#define SWORD_PERCEPTION_PERCEPTION_LOCALISATION_H

namespace sword
{
namespace perception
{
// =============================================================================
/** @class  PerceptionLocalisation
    @brief  Perception localisation
*/
// Created: LDC 2009-07-29
// =============================================================================
class PerceptionLocalisation
{
public:
    //! @name Constructors/Destructor
    //@{
             PerceptionLocalisation();
    virtual ~PerceptionLocalisation();
    //@}

    //! @name Operations
    //@{
    int Id() const;
    //@}

private:
    //! @name Member data
    //@{
    int id_;
    //@}
};

}
}

#endif // SWORD_PERCEPTION_PERCEPTION_LOCALISATION_H
