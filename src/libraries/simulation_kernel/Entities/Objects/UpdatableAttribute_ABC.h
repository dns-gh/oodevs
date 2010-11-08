// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __UpdatableAttribute_ABC_h_
#define __UpdatableAttribute_ABC_h_

//$$$ NLD - Comment faire simple quand on peut faire compliquer => Virer les flags

// =============================================================================
/** @class  UpdatableAttribute_ABC
    @brief  UpdatableAttribute_ABC
*/
// Created: JCR 2008-05-30
// =============================================================================
class UpdatableAttribute_ABC
{
public:
    //! @name Type
    //@{
    enum E_UpdateType {
        eOnUpdate    = 0x02,
        eOnHLAUpdate = 0x04,
        eUpdateAll   = eOnHLAUpdate | eOnUpdate
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             UpdatableAttribute_ABC() : xToUpdate_ ( eUpdateAll ) {}
    virtual ~UpdatableAttribute_ABC() {}
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const unsigned int ) {}
    //@}

protected:
    //! @name
    //@{
    virtual void Reset( int update ) const
    {
        xToUpdate_ &= ( xToUpdate_ ^ update );
    }

    /*virtual void Reset() const
    {
        xToUpdate_ = 0;
    }*/

    virtual void NotifyAttributeUpdated( int update ) const
    {
        xToUpdate_ |= update;
    }

    /*virtual bool NeedUpdate() const
    {
        return xToUpdate_ != 0;
    }*/

    virtual bool NeedUpdate( E_UpdateType type ) const
    {
        return ( xToUpdate_ & type ) != 0;
    }
    //@}

private:
    //! @name
    //@{
    mutable unsigned char xToUpdate_;
    //@}
};

#endif // __UpdatableAttribute_ABC_h_
