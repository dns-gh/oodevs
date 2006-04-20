// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2006 Mathématiques Appliquées SA (MASA)
//
// *****************************************************************************

#ifndef __RichLabel_h_
#define __RichLabel_h_

// =============================================================================
/** @class  RichLabel
    @brief  Rich label
*/
// Created: SBO 2006-04-18
// =============================================================================
class RichLabel : public QLabel
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
             RichLabel( const QString& text, QWidget* parent = 0, const char* name = 0 );
             RichLabel( const QString& text, bool required, QWidget* parent = 0, const char* name = 0 );
    virtual ~RichLabel();
    //@}

    //! @name Operations
    //@{
    void Warn( int msec );
    //@}

private slots:
    //! @name slots
    //@{
    void OnWarnStop();
    //@}

private:
    //! @name Copy/Assignement
    //@{
    RichLabel( const RichLabel& );            //!< Copy constructor
    RichLabel& operator=( const RichLabel& ); //!< Assignement operator
    //@}
};

#endif // __RichLabel_h_
