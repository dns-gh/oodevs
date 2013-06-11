// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __DisplayableModesObserver_ABC_h_
#define __DisplayableModesObserver_ABC_h_

#include "ReadOnlyModable.h"

namespace kernel
{

// =============================================================================
/** @class  DisplayableModesObserver_ABC
    @brief  DisplayableModesObserver_ABC
*/
// Created: ABR 2012-05-09
// =============================================================================
class DisplayableModesObserver_ABC : public ReadOnlyModable
{

public:
    //! @name Constructors/Destructor
    //@{
             DisplayableModesObserver_ABC()
                 : hiddenModes_( 0 ), visibleModes_( 0 ), currentMode_( eModes_None ), visible_( false ), visibleByDefault_( false ) {}
             DisplayableModesObserver_ABC( int hiddenModes, int visibleModes, bool visibleByDefault )
                 : hiddenModes_( hiddenModes ), visibleModes_( visibleModes ), currentMode_( eModes_None ), visible_( false ), visibleByDefault_( visibleByDefault ) {}
    virtual ~DisplayableModesObserver_ABC() {}
    //@}

    //! @name Abstract operations
    //@{
    virtual void SetVisible( bool visible ) = 0;
    virtual void ForceEnabled( bool enabled ) = 0;
    virtual void EnsureIsEnabled() = 0;
    virtual bool IsVisible() const = 0;
    //@}

    //! @name Accessors
    //@{
    virtual E_Modes GetCurrentMode() const { return currentMode_; }
    void SetModes( int hiddenModes, int visibleModes = 0, bool visibleByDefault = false )
    {
        hiddenModes_ = hiddenModes;
        visibleModes_ = visibleModes;
        visibleByDefault_ = visibleByDefault;
    }
    void SetHiddenModes( int modes ) { hiddenModes_ = modes; }
    void SetVisibleModes( int modes ) { visibleModes_ = modes; }
    void SetVisibleByDefault( bool visibleByDefault ) { visibleByDefault_ = visibleByDefault; }
    virtual void SetLastVisible() { SetVisible( visible_ ); }
    int GetHiddenModes() const { return hiddenModes_; }
    int GetVisibleModes() const { return visibleModes_; }
    bool GetVisibleByDefault() const { return visibleByDefault_; }
    //@}

    //! @name Observer operation
    //@{
    virtual void NotifyModeChanged( E_Modes newMode, bool useDefault, bool firstChangeToSavedMode )
    {
        if( currentMode_ != -1 && IsOptional( currentMode_ ) )  // was optional
            visible_ = IsVisible();

        if( newMode & hiddenModes_ )                            // becoming forbidden
        {
            SetVisible( false );
            ForceEnabled( false );
        }
        else if( newMode & visibleModes_ )                      // becoming mandatory
        {
            SetVisible( true );
            ForceEnabled( true );
        }
        else if( useDefault )                                   // becoming optional by default, let default choose if visible or not (only for savedMode without register)
        {
            EnsureIsEnabled();
            SetVisible( visibleByDefault_ );
        }
        else                                                    // becoming optional
        {
            EnsureIsEnabled();
            if( !firstChangeToSavedMode )                       // first change to saved mode, let register choose if visible or not
                SetLastVisible();
        }
        currentMode_ = newMode;
        SetReadOnly( IsReadOnly() );
    }
    //@}

private:
    bool IsOptional( int mode ) const
    {
        return !( mode & visibleModes_ || mode & hiddenModes_ );
    }

private:
    //! @name Member data
    //@{
    int hiddenModes_;
    int visibleModes_;
    E_Modes currentMode_;
    bool visible_;
    bool visibleByDefault_;
    //@}
};

} //! namespace kernel

#endif // __DisplayableModesObserver_ABC_h_
