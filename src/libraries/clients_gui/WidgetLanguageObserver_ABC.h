// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __gui_WidgetLanguageObserver_ABC_h_
#define __gui_WidgetLanguageObserver_ABC_h_

namespace gui
{

// =============================================================================
/** @class  WidgetLanguageObserver_ABC
    @brief  WidgetLanguageObserver_ABC
*/
// Created: ABR 2011-11-09
// =============================================================================
template< typename ParentType >
class WidgetLanguageObserver_ABC : public ParentType
{
public:
    //! @name Constructors/Destructor
    //@{
    WidgetLanguageObserver_ABC() : ParentType() {}
    template< typename T >
    WidgetLanguageObserver_ABC( T param ) : ParentType( param ) {}
    template< typename T1, typename T2 >
    WidgetLanguageObserver_ABC( T1 param1, T2 param2 ) : ParentType( param1, param2 ) {}
    template< typename T1, typename T2, typename T3 >
    WidgetLanguageObserver_ABC( T1 param1, T2 param2, T3 param3 ) : ParentType( param1, param2, param3 ) {}

    virtual ~WidgetLanguageObserver_ABC() {}
    //@}

public:
    //! @name Operations
    //@{
    virtual void OnLanguageChanged() = 0;
    //@}

private:
    //! @name Member data
    //@{
    virtual void changeEvent( QEvent * event );
    //@}
};

template< typename ParentType >
void WidgetLanguageObserver_ABC< ParentType >::changeEvent( QEvent * event )
{
    if( event->type() == QEvent::LanguageChange )
        OnLanguageChanged();
    ParentType::changeEvent( event );
}

}

#endif // __gui_WidgetLanguageObserver_ABC_h_
