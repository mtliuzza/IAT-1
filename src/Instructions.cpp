#include "Instructions.h"

Instructions::Instructions() {
  this->setupUi( this );
}

void Instructions::setText( QString text ) {
  this->instructions->setText( text );
}

void Instructions::keyPressEvent( QKeyEvent* event ) {
  event->accept();
  emit read();
}

