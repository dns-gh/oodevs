// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2012 MASA Group
//
// *****************************************************************************

#ifndef __ModesObserver_ABC_h_
#define __ModesObserver_ABC_h_

namespace kernel
{

// =============================================================================
/** @class  ModesObserver_ABC
    @brief  ModesObserver_ABC
*/
// Created: ABR 2012-05-09
// =============================================================================
class ModesObserver_ABC
{

public:
    //! @name Constructors/Destructor
    //@{
             ModesObserver_ABC()
                 : defaultModes_( 0 ), visibleModes_( 0 ), hiddenModes_( 0 ) {}
             ModesObserver_ABC( int defaultModes, int visibleModes, int hiddenModes )
                 : defaultModes_( defaultModes ), visibleModes_( visibleModes ), hiddenModes_( hiddenModes ) {}
    virtual ~ModesObserver_ABC() {}
    //@}

    //! @name Abstract operations
    //@{
    virtual void SetVisible( bool visible ) = 0;
    virtual void ForceEnabled( bool enabled ) = 0;
    virtual void EnsureIsEnabled() = 0;
    //@}

    //! @name Accessors
    //@{
    void SetModes( int defaultModes, int hiddenModes = 0, int visibleModes = 0 )
    {
        defaultModes_ = defaultModes;
        hiddenModes_ = hiddenModes;
        visibleModes_ = visibleModes;
    }
    void SetDefaultModes( int modes ) { defaultModes_ = modes; }
    void SetHiddenModes( int modes ) { hiddenModes_ = modes; }
    void SetVisibleModes( int modes ) { visibleModes_ = modes; }
    int GetDefaultModes() const { return defaultModes_; }
    int GetHiddenModes() const { return hiddenModes_; }
    int GetVisibleModes() const { return visibleModes_; }
    //@}

    //! @name Observer operation
    //@{
    void NotifyModeChange( int newMode, bool useDefault )
    {
#ifdef DEBUG
        if( newMode & hiddenModes_ && newMode & defaultModes_ )
            throw std::exception( __FUNCTION__ " This component tries to always be hidden but it's used by default." );
        if( newMode & visibleModes_ && !( newMode & defaultModes_ ) )
            throw std::exception( __FUNCTION__ " This component tries to always be visible but it's not used by default." );
        if( newMode & visibleModes_ && newMode & hiddenModes_ )
            throw std::exception( __FUNCTION__ " This component tries to always be visible and hidden in the same time." );
#endif
        if( newMode & visibleModes_ )
        {
            SetVisible( true );
            ForceEnabled( true );
        }
        else if( newMode & hiddenModes_ )
        {
            SetVisible( false );
            ForceEnabled( false );
        }
        else if( useDefault )
        {
            SetVisible( ( newMode & defaultModes_ ) != 0 );
            EnsureIsEnabled();
        }
        else
            EnsureIsEnabled();
    }
    //@}

private:
    //! @name Member data
    //@{
    int defaultModes_;
    int hiddenModes_;
    int visibleModes_;
    //@}
};

} //! namespace kernel

#endif // __ModesObserver_ABC_h_
