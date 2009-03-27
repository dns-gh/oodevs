// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2008 MASA Group
//
// *****************************************************************************

#ifndef __DEC_Knowledge_ObjectAttributeUpdatable_ABC_h_
#define __DEC_Knowledge_ObjectAttributeUpdatable_ABC_h_

// =============================================================================
/** @class  DEC_Knowledge_ObjectAttributeUpdatable_ABC
    @brief  DEC_Knowledge_ObjectAttributeUpdatable_ABC
*/
// Created: JCR 2008-05-30
// =============================================================================
class DEC_Knowledge_ObjectAttributeUpdatable_ABC
{ 
public:    
    //! @name Type
    //@{
    enum E_UpdateType {           
        eOnCreation  = 0x01,
        eOnUpdate    = 0x02
    };
    //@}

public:
    //! @name Constructors/Destructor
    //@{
            DEC_Knowledge_ObjectAttributeUpdatable_ABC() : xToUpdate_ ( 0 ) {}
    virtual ~DEC_Knowledge_ObjectAttributeUpdatable_ABC() {}
    //@}

    //! @name BOOST::serialization
    //@{
//    template< typename Archive > void serialize( Archive&, const uint ) {}
    //@}

protected:
    //! @name 
    //@{
    virtual void Reset() const
    {
        xToUpdate_ = 0;
    }

    virtual void NotifyAttributeUpdated( E_UpdateType update ) const
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

#endif // __DEC_Knowledge_ObjectAttributeUpdatable_ABC_h_
