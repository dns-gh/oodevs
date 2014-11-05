// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2013 MASA Group
//
// *****************************************************************************

#ifndef __EventTaskViewState_h_
#define __EventTaskViewState_h_

namespace gui
{

// =============================================================================
/** @class  EventOrderViewState
    @brief  EventOrderViewState
*/
// Created: ABR 2013-11-18
// =============================================================================
struct EventTaskViewState
{
    //! @name Constructors
    //@{
    EventTaskViewState()
        : target_( 0 )
        , bytes_( 0 )
        , isUrlValid_( false )
        , isPayloadVisible_( false )
    {
        // NOTHING
    }
    EventTaskViewState( unsigned long target,
                        const QString& label,
                        const QString& description,
                        const QString& url,
                        const std::string& payload,
                        bool isUrlValid,
                        bool isPayloadVisible,
                        int bytes )
        : target_( target )
        , label_( label )
        , description_( description )
        , url_( url )
        , payload_( payload )
        , isUrlValid_( isUrlValid )
        , isPayloadVisible_( isPayloadVisible )
        , bytes_( bytes )
    {
        // NOTHING
    }
    //@}

    //! @name Operations
    //@{
    void Purge()
    {
        *this = EventTaskViewState();
    }
    //@}

    //! @name Member data
    //@{
    unsigned long target_;
    QString label_;
    QString description_;
    QString url_;
    std::string payload_;
    bool isUrlValid_;
    bool isPayloadVisible_;
    int bytes_;
    //@}
};

} //! namespace gui

#endif // __EventTaskViewState_h_
