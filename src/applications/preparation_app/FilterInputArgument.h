// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2011 MASA Group
//
// *****************************************************************************

#ifndef __FilterInputArgument_h_
#define __FilterInputArgument_h_

#include <boost/noncopyable.hpp>

namespace tools
{
    class ExerciseConfig;
}

namespace kernel
{
    class XmlDescription;
}

class FilterPartiesListView;

// =============================================================================
/** @class  FilterInputArgument
    @brief  FilterInputArgument
*/
// Created: ABR 2011-09-28
// =============================================================================
class FilterInputArgument : public QObject
                          , private boost::noncopyable
{
    Q_OBJECT

public:
    //! @name Constructors/Destructor
    //@{
    explicit FilterInputArgument( const tools::ExerciseConfig& config, const std::string& argumentValue, const kernel::XmlDescription& description, const std::string& exerciseDir = "" );
    virtual ~FilterInputArgument();
    //@}

    static bool IsInputArgument( const std::string& argument );

    //! @name Operations
    //@{
    QWidget* CreateWidget( QWidget* parent );
    QString GetText() const;
    void Update();
    bool IsValid();
    //@}

private:
    //! @name Types
    //@{
    enum E_ArgumentType { eInput, eOpen, eSave, eDirectory, eTeamList };
    //@}

signals:
    //! @name Signals
    //@{
    void ValueChanged( const QString& text );
    //@}

private slots:
    //! @name Slots
    //@{
    void OnBrowse();
    void OnTextChanged( const QString& text = "" );
    //@}

private:
    //! @name Member data
    //@{
    const tools::ExerciseConfig& config_;
    E_ArgumentType               type_;
    QLineEdit*                   line_;
    FilterPartiesListView*       listView_;
    QLabel*                      errorLabel_;
    const std::string            exerciseDir_;
    const std::string            description_;
    //@}
};

#endif // __FilterInputArgument_h_
