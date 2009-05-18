#include "Item.h"

Item::Item() {
  setupUi( this );
}

void Item::init(
  QString leftCategory1,
  QString leftCategory2,
  QString rightCategory1,
  QString rightCategory2,
  QString word
) {
  this->acceptAnswer = false;
  this->leftCategory1->setText( leftCategory1 );
  this->leftCategory2->setText( leftCategory2 );
  this->rightCategory1->setText( rightCategory1 );
  this->rightCategory2->setText( rightCategory2 );
  this->word->hide();
  this->word->setText( word );
  QTimer::singleShot( MSECS_BEFORE_DISPLAYING_WORD, this, SLOT( showWord() ) );
}

int Item::getTime() {
  int time = this->timer.elapsed();

  if ( time > 3000 ) {
    return 3000;
  }

  if ( time < 300 ) {
    return 300;
  }

  return time;
}

void Item::keyPressEvent( QKeyEvent* event ) {
  if ( ! this->acceptAnswer ) {
    event->ignore();
    return;
  }

  if ( event->key() == Qt::Key_F ) {
    event->accept();
    this->word->setText( "f" );
    emit answered( true, this->getTime() );
    return;
  }
  if ( event->key() == Qt::Key_J ) {
    event->accept();
    this->word->setText( "j" );
    emit answered( false, this->getTime() );
    return;
  }

  event->ignore();
}

void Item::showWord() {
  this->word->show();
  this->timer.start();
  this->acceptAnswer = true;
}

