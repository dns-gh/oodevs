// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __DisplayExtractor_ABC_h_
#define __DisplayExtractor_ABC_h_

#include "tools/VirtualTemplate.h"
#include "clients_kernel/Tools.h"

namespace kernel
{
    template< typename T >
    struct DisplayExtractor
    {
        virtual QString GetDisplayName( const T& element ) const = 0;

        void GetDisplayNameInternal( const T& element, QString& name ) const
        {
            name = GetDisplayName( element );
        }
    };

// =============================================================================
/** @class  DisplayExtractor_ABC
    @brief  DisplayExtractor_ABC
*/
// Created: AGE 2012-10-17
// =============================================================================
class DisplayExtractor_ABC : public tools::InterfaceContainer< DisplayExtractor_ABC >
{
public:
    //! @name Constructors/Destructor
    //@{
             DisplayExtractor_ABC() { Register( *this ); }
    virtual ~DisplayExtractor_ABC() { Unregister( *this ); }
    //@}

    //! @name Operations
    //@{
    virtual void NotifyLinkClicked( const QString& url ) = 0;
    template< typename T >
    QString GetDisplayName( const T* element );
    template< typename T >
    QString GetDisplayName( const T& element );
    //@}
};

// -----------------------------------------------------------------------------
// Name: DisplayExtractor_ABC::GetDisplayName
// Created: JSR 2012-10-17
// -----------------------------------------------------------------------------
template< typename T >
QString DisplayExtractor_ABC::GetDisplayName( const T& element )
{
    QString name;
    Apply( &DisplayExtractor< T >::GetDisplayNameInternal, element, name );
    return name;
}

// -----------------------------------------------------------------------------
// Name: DisplayExtractor_ABC::GetDisplayName
// Created: JSR 2012-10-18
// -----------------------------------------------------------------------------
template< typename T >
QString DisplayExtractor_ABC::GetDisplayName( const T* element )
{
    if( element )
        return GetDisplayName( *element );
    else
        return tools::translate( "DisplayExtractor_ABC", "Unknown element" );
}

}

#endif // __DisplayExtractor_ABC_h_
