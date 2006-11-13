// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __Melody_h_
#define __Melody_h_

class XPSLights;

// =============================================================================
/** @class  Melody
    @brief  Melody
*/
// Created: AGE 2006-11-13
// =============================================================================
class Melody
{

public:
    //! @name Static
    //@{
    static Melody Explosion();
    static Melody TickStart();
    //@}

public:
    //! @name Constructors/Destructor
    //@{
             Melody();
    virtual ~Melody();
    //@}

    //! @name Modifiers
    //@{
    void StartSequence();
    void SetLights( char side, char front, char top, char brightness );
    void SetSideColor( char value ); 
    void SetFrontColor( char value );
    void SetTopColor( char value );  
    void SetBrightness( char value );
    void Pause( unsigned msec );
    void Repeat( short times );
    //@}

    //! @name Operations
    //@{
    void Play( XPSLights& lights ) const;
    //@}

private:
    //! @name Types
    //@{
    struct Operation
    {
        char type;
        unsigned value;
    };
    typedef std::vector< Operation >       T_Operations;
    typedef T_Operations::const_iterator CIT_Operations;
    //@}

    //! @name Helpers
    //@{
    void AddOperation( char type, unsigned parameter );
    void PlayOperation( const Operation& op, XPSLights& lights ) const;
    void Repeat( short index, short times, XPSLights& lights  ) const;
    //@}

private:
    //! @name Member data
    //@{
    T_Operations operations_;
    //@}
};

#endif // __Melody_h_
