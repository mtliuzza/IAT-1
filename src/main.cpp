#include "Controller.h"

#include <QtCore>
#include <QtGui>

int main( int argc, char ** argv ) {
  QApplication app( argc, argv );
  QDir::setCurrent( QApplication::applicationDirPath() );

  Controller controller;

  if ( ! controller.init() ) {
    qDebug() << "Couldn't init test.";
    return 0;
  }

  controller.start();

  return app.exec();
}

