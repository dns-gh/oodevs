// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef ADN_WeaponFilter_h
#define ADN_WeaponFilter_h

#include "ADN_Types.h"
#include "ADN_Connector_ABC.h"
#include "ADN_Weapons_Data.h"
#include <boost/noncopyable.hpp>
#include <boost/function.hpp>

// =============================================================================
// @class  ADN_WeaponFilter
// @brief  ADN_WeaponFilter
//
// Created: MCO 2011-07-05
// =============================================================================
class ADN_WeaponFilter : public ADN_Connector_ABC, boost::noncopyable
{
    Q_OBJECT

private:
    //! @name Types
    //@{
    typedef boost::function< bool( const ADN_Weapons_Data::WeaponInfos& infos ) > T_Validate;
    //@}

public:
    //! @name Constructors/Destructor
    //@{
    ADN_WeaponFilter( T_Validate validate );
    //@}

    //! @name Modifiers
    //@{
    void SetData( ADN_Type_Bool* pData );
    //@}

protected:
    //! @name Operations
    //@{
    virtual void Initialize( ADN_Connector_ABC& connector ) const;
    virtual void SetDataPrivate( void* pData );
    //@}

    //! @name Helpers
    //@{
    bool Check() const;
    //@}

private:
    //! @name Member data
    //@{
    T_Validate validate_;
    ADN_Type_Bool* pData_;
    mutable ADN_Connector_ABC* pConnector_;
    //@}
};

#endif // ADN_WeaponFilter_h
