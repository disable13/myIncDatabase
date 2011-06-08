#include "dfooter.h"
//
#include <QGridLayout>
#include <QProgressBar>
#include "QProgressIndicator.h"
#include <QLabel>
//
DFooter::DFooter(QWidget *parent) :
    QWidget(parent)
{
    max = -1;

    l = new QGridLayout( this );
    setLayout( l );

    lText = new QLabel( this );
    progressBar = new QProgressBar( this );
    progressInicator = new QProgressIndicator(this);

    l->addWidget( lText );
    l->addWidget( progressBar, 0, 1);
    l->addWidget( progressInicator, 0, 2);

    progressBar->setVisible( false );
    progressBar->setMinimum( 0 );
    progressBar->setValue( 0 );
    progressBar->setMaximum( 1 );
    progressInicator->setAnimationDelay( 70 );
}
//
DFooter::~DFooter()
{
    delete progressInicator;
    delete progressBar;
    delete lText;

    delete l;
}
//
void DFooter::setText( QString text )
{
    lText->setText( text );
}
//
void DFooter::progressStart()
{
   //lText->setPixmap( QPixmap() );
    progressInicator->startAnimation();
}
//
void DFooter::progressStart(int mx)
{
    max = mx;
   //lText->setPixmap( QPixmap() );
    progressBar->setMaximum( max );
    progressBar->setVisible( true );
    if (!progressInicator->isAnimated())
        progressInicator->startAnimation();
}
//
void DFooter::progressStart(QString text )
{
   //lText->setPixmap( QPixmap() );
    lText->setText( text );
    progressInicator->startAnimation();
}
//
void DFooter::progressStart(QString text, int mx)
{
    //lText->setPixmap( QPixmap() );
    lText->setText( text );
    max = mx;
    progressBar->setMaximum( max );
    progressBar->setVisible( true );
    if (!progressInicator->isAnimated())
        progressInicator->startAnimation();
}
//
void DFooter::progressChange(int cr)
{
    if (max > cr ) { // then stop
        progressBar->setValue( 0 );
        progressBar->setMaximum( 1 );
        progressBar->setVisible( false );
        progressInicator->stopAnimation();
        return;
    }
    progressBar->setValue( cr );
}
//
void DFooter::progressStop()
{
    progressInicator->stopAnimation();
}
//
void DFooter::progressStop(bool state)
{
    if (state) {
        //lText->setPixmap( QPixmap(":/icon/apply.png") );
        lText->setText( tr("Complete!") );
    } else{
        //lText->setPixmap( QPixmap(":/icon/cancel.png") );
        lText->setText( tr("Uncomplete...") );
    }
    progressInicator->stopAnimation();
}
