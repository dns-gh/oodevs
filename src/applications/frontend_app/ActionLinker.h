// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2007 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __ActionLinker_h_
#define __ActionLinker_h_

class Panel_ABC;

// =============================================================================
/** @class  ActionLinker
    @brief  ActionLinker
*/
// Created: AGE 2007-10-10
// =============================================================================
class ActionLinker
{
    struct SubLinker;
public:
    //! @name Constructors/Destructor
    //@{
             ActionLinker();
    virtual ~ActionLinker();
    //@}

    //! @name Operations
    //@{
    void Register( const QString& action, Panel_ABC& panel );

    SubLinker& operator,( const QString& action );
    //@}

private:
    //! @name Copy/Assignment
    //@{
    ActionLinker( const ActionLinker& );            //!< Copy constructor
    ActionLinker& operator=( const ActionLinker& ); //!< Assignment operator
    //@}

    //! @name Helpers
    //@{
    struct SubLinker
    {
        SubLinker( ActionLinker* that ) : that_( that ) {}
        SubLinker& operator,( const QString& action )
        {
            that_->Sublink( action );
            return *this;
        }
        ActionLinker* that_;
    };
    void Sublink( const QString& action );
    //@}

private:
    //! @name Member data
    //@{
    SubLinker sub_;
    std::map< QString, Panel_ABC* > panels_;
    QString current_;
    //@}
};

#endif // __ActionLinker_h_
