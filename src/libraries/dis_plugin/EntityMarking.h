// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __EntityMarking_h_
#define __EntityMarking_h_

namespace dis
{

// =============================================================================
/** @class  EntityMarking
    @brief  EntityMarking
*/
// Created: AGE 2008-03-10
// =============================================================================
class EntityMarking
{

public:
    //! @name Constructors/Destructor
    //@{
    explicit EntityMarking( const std::string& name );
            ~EntityMarking();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << characterSet_ << marking_;
    }
    //@}

private:
    //! @name Member data
    //@{
    unsigned char characterSet_;
    unsigned char marking_[11];
    //@}
};

}

#endif // __EntityMarking_h_
