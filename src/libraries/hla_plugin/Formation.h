// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef plugins_hla_Formation_h
#define plugins_hla_Formation_h

namespace hla
{
    class UpdateFunctor_ABC;
}

namespace plugins
{
namespace hla
{
// =============================================================================
/** @class  Formation
    @brief  Formation
*/
// Created: AGE 2008-02-25
// =============================================================================
class Formation
{
public:
    //! @name Constructors/Destructor
    //@{
    explicit Formation( bool isOnRoad );
    virtual ~Formation();
    //@}

    //! @name Operations
    //@{
    template< typename Archive >
    void Serialize( Archive& archive ) const
    {
        archive << formation_;
    }
    //@}

private:
    //! @name Member data
    //@{
    long formation_;
    //@}
};

}
}

#endif // plugins_hla_Formation_h
