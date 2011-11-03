// *****************************************************************************
//
// This file is part of a MASA library or program.
// Refer to the included end-user license agreement for restrictions.
//
// Copyright (c) 2010 MASA Group
//
// *****************************************************************************

#include "frontend_pch.h"
#include "RandomPluginConfigPanel.h"
#include "moc_RandomPluginConfigPanel.cpp"
#include "CreateSession.h"
#include "clients_gui/Tools.h"
#pragma warning( push, 0 )
#include "QtCore/qcoreapplication.h"
#include <QtGui/qcheckbox.h>
#include <QtGui/qcombobox.h>
#include <Qt3Support/q3groupbox.h>
#include <QtGui/qlabel.h>
#include <QtGui/qlineedit.h>
#include <QtGui/qpushbutton.h>
#include <QtCore/qsettings.h>
#include <QtGui/qspinbox.h>
#include <QtGui/qvalidator.h>
#pragma warning( pop )

using namespace frontend;

namespace
{
    void ReadGaussian( bool* distributions )
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        for( int i = 0; i < RandomPluginConfigPanel::eContextsNbr; ++i )
            distributions[ i ] = settings.readBoolEntry( QString( "/sword/RandomDistribution" ) + QString::number( i ), false );
    }

    void ReadDeviation( double* deviations )
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        for( int i = 0; i < RandomPluginConfigPanel::eContextsNbr; ++i )
            deviations[ i ] = settings.readDoubleEntry( QString( "/sword/RandomDeviation" ) + QString::number( i ), 0.5 );
    }

    void ReadMean( double* means )
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        for( int i = 0; i < RandomPluginConfigPanel::eContextsNbr; ++i )
            means[ i ] = settings.readDoubleEntry( QString( "/sword/RandomMean" ) + QString::number( i ), 0.5 );
    }

    bool ReadHasSeed()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readBoolEntry( "/sword/RandomHasSeed", false );
    }

    int ReadSeed()
    {
        QSettings settings;
        settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
        return settings.readNumEntry( "/sword/RandomSeed", 1 );
    }

    class Validator : public QDoubleValidator
    {
    public:
        Validator( QObject* pParent )
            : QDoubleValidator( 0., 1., 10, pParent ) {}

        QValidator::State validate( QString& input, int& /*nPos*/ ) const
        {
            double b = bottom();
            double t = top();
            int d = decimals();

            QRegExp empty( QString::fromLatin1(" *-?\\.? *") );
            if( b >= 0 &&
                input.stripWhiteSpace().startsWith(QString::fromLatin1("-")) )
                return Invalid;
            if( empty.exactMatch(input) )
                return Intermediate;
            bool ok = TRUE;
            double entered = input.toDouble( &ok );
            if( !ok )
                return Invalid;

            int i = input.find( '.' );
            if( i >= 0 )
                if( d==0 )
                    return Invalid;
                else
                {
                    // has decimal point (but no E), now count digits after that
                    i++;
                    int j = i;
                    while( input[j].isDigit() )
                        j++;
                    if( j - i > d )
                        return Invalid;
                }

            if( entered > t )
                return Invalid;
            else if( entered < b )
                return Intermediate;
            else
               return Acceptable;
        }
    };
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel constructor
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
RandomPluginConfigPanel::RandomPluginConfigPanel( QWidget* parent, const tools::GeneralConfig& config )
    : PluginConfig_ABC( parent )
    , config_( config )
{
    setMargin( 5 );

    Q3GroupBox* hbox = new Q3GroupBox( 2, Qt::Horizontal, tools::translate( "RandomPluginConfigPanel", "Random Generator" ), this );
    hasSeed_ = new QCheckBox( tools::translate( "RandomPluginConfigPanel", "Seed:" ), hbox );
    connect( hasSeed_, SIGNAL( toggled( bool ) ), SLOT( OnSeedToggled() ) );
    seed_ = new QSpinBox( 1, std::numeric_limits< int >::max(), 1, hbox );
    bool bHasSeed = ReadHasSeed();
    seed_->setValue( ReadSeed() );
    seed_->setEnabled( bHasSeed );
    hasSeed_->setChecked( bHasSeed );

    new QLabel( tools::translate( "RandomPluginConfigPanel", "Context:" ), hbox );
    contextList_ = new QComboBox( hbox );
    contextList_->insertItem( tools::translate( "RandomPluginConfigPanel", "Fire" ), eFire );
    contextList_->insertItem( tools::translate( "RandomPluginConfigPanel", "Wounds" ), eWounds );
    contextList_->insertItem( tools::translate( "RandomPluginConfigPanel", "Perception" ), ePerception );
    contextList_->insertItem( tools::translate( "RandomPluginConfigPanel", "Breakdowns" ), eBreakdowns );
    contextList_->setCurrentItem( 0 );
    connect( contextList_, SIGNAL( activated( int ) ), SLOT( OnContextChanged( int ) ) );

    new QLabel( tools::translate( "RandomPluginConfigPanel", "Distribution:" ), hbox );
    distributionList_ = new QComboBox( hbox );
    distributionList_->insertItem( tools::translate( "RandomPluginConfigPanel", "Linear" ) );
    distributionList_->insertItem( tools::translate( "RandomPluginConfigPanel", "Gaussian" ) );
    connect( distributionList_, SIGNAL( activated( int ) ), SLOT( OnDistributionChanged( int ) ) );

    new QLabel( tools::translate( "RandomPluginConfigPanel", "Standard deviation:" ), hbox );
    deviation_ = new QLineEdit( hbox );
    deviation_->setValidator( new Validator( deviation_ ) );
    connect( deviation_, SIGNAL( textChanged( const QString& ) ), SLOT( OnDeviationChanged( const QString& ) ) );

    new QLabel( tools::translate( "RandomPluginConfigPanel", "Mean:" ), hbox );
    mean_ = new QLineEdit( hbox );
    mean_->setValidator( new Validator( mean_ ) );
    connect( mean_, SIGNAL( textChanged( const QString& ) ), SLOT( OnMeanChanged( const QString& ) ) );

    QPushButton* defaultBtn = new QPushButton( tools::translate( "RandomPluginConfigPanel", "Set as default" ), hbox );
    connect( defaultBtn, SIGNAL( clicked() ), this, SLOT( OnDefaultClicked() ) );

    ReadRandomValues();
    OnContextChanged( 0 );
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel destructor
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
RandomPluginConfigPanel::~RandomPluginConfigPanel()
{
    // NOTHING
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::GetName
// Created: SBO 2011-05-09
// -----------------------------------------------------------------------------
QString RandomPluginConfigPanel::GetName() const
{
    return tools::translate( "RandomPluginConfigPanel", "Random" );
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::Commit
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::Commit( const std::string& exercise, const std::string& session )
{
    frontend::CreateSession action( config_, exercise, session );
    action.SetOption( "session/config/simulation/random/@seed", hasSeed_->isChecked() ? seed_->value() : 0 );
    for( int i = 0; i < eContextsNbr; ++i ) // Fire, Wounds, Perception, Breakdowns
    {
        std::stringstream stream;
        stream << "session/config/simulation/random";
        stream << i;
        action.SetOption( stream.str() + "/@distribution", bDistributions_[ i ] );
        action.SetOption( stream.str() + "/@deviation"   , rDeviations_[ i ] );
        action.SetOption( stream.str() + "/@mean"        , rMeans_[ i ] );
    }
    action.Commit();
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::ReadRandomValues
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::ReadRandomValues()
{
    ReadGaussian( bDistributions_ );
    ReadDeviation( rDeviations_ );
    ReadMean( rMeans_ );
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnContextChanged
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnContextChanged( int index )
{
    ERandomContexts context = ( ERandomContexts ) index;
    if( context >= eFire && context < eContextsNbr )
    {
        distributionList_->setCurrentItem( bDistributions_[ context ] ? 1 : 0 );
        deviation_->setText( QString::number( rDeviations_[ context ] ) );
        mean_->setText( QString::number( rMeans_[ context ] ) );
        deviation_->setEnabled( bDistributions_[ context ] );
        mean_->setEnabled( bDistributions_[ context ] );
    }
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnDistributionChanged
// Created: JSR 2010-07-02
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnDistributionChanged( int index )
{
    deviation_->setEnabled( index != 0 );
    mean_->setEnabled( index != 0 );
    ERandomContexts context = ( ERandomContexts ) contextList_->currentItem();
    if( context >= eFire && context < eContextsNbr )
    {
        bDistributions_[ context ] = ( index != 0 );
    }
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnDeviationChanged
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnDeviationChanged( const QString& )
{
    if( deviation_->hasAcceptableInput() )
    {
        ERandomContexts context = ( ERandomContexts ) contextList_->currentItem();
        if( context >= eFire && context < eContextsNbr )
            rDeviations_[ context ] = deviation_->text().toDouble();
    }
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnMeanChanged
// Created: JSR 2010-07-05
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnMeanChanged( const QString& )
{
    if( mean_->hasAcceptableInput() )
    {
        ERandomContexts context = ( ERandomContexts ) contextList_->currentItem();
        if( context >= eFire && context < eContextsNbr )
            rMeans_[ context ] = mean_->text().toDouble();
    }
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnSeedToggled
// Created: JSR 2010-07-01
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnSeedToggled()
{
    seed_->setEnabled( hasSeed_->isChecked() );
}

// -----------------------------------------------------------------------------
// Name: RandomPluginConfigPanel::OnDefaultClicked
// Created: JSR 2010-07-12
// -----------------------------------------------------------------------------
void RandomPluginConfigPanel::OnDefaultClicked()
{
    QSettings settings;
    settings.setPath( "MASA Group", qApp->translate( "Application", "SWORD" ) );
    for( int i = 0; i < eContextsNbr; ++i )
    {
        settings.writeEntry( QString( "/sword/RandomDistribution" ) + QString::number( i ), bDistributions_[ i ] );
        settings.writeEntry( QString( "/sword/RandomDeviation" ) + QString::number( i ), rDeviations_[ i ] );
        settings.writeEntry( QString( "/sword/RandomMean" ) + QString::number( i ), rMeans_[ i ] );
    }
    settings.writeEntry( "/sword/RandomHasSeed", hasSeed_->isChecked() );
    settings.writeEntry( "/sword/RandomSeed", seed_->value() );
}
