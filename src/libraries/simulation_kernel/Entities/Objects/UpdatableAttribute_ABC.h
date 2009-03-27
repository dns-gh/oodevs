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
        eOnCreation  = 0x01,
        eOnUpdate    = 0x02,
        eOnHLAUpdate = 0x04,        
        eUpdateAll   = 0xFF
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
            UpdatableAttribute_ABC() : xToUpdate_ ( 0 ) {}
    virtual ~UpdatableAttribute_ABC() {}
    //@}

    //! @name Serialization
    //@{
    template< typename Archive > void serialize( Archive&, const uint ) {}
    //@}

protected:
    //! @name 
    //@{
    virtual void Reset( int update ) const
    {
        xToUpdate_ &= ( xToUpdate_ ^ update );
    }
    
    virtual void Reset() const
    {
        xToUpdate_ = 0;
    }

    virtual void NotifyAttributeUpdated( int update ) const
    {
        xToUpdate_ |= update;
    }

    virtual bool NeedUpdate() const
    {
        return xToUpdate_ != 0;
    }

    virtual bool NeedUpdate( E_UpdateType type ) const
    {
        return ( xToUpdate_ & type ) != 0;
    }
    //@}

private:
    //! @name 
    //@{    
    mutable uint8 xToUpdate_;
    //@}
};

#endif // __UpdatableAttribute_ABC_h_
