#include "src/Item.h"

#include <QApplication>


int main( int argc, char ** argv ) {
  QApplication app( argc, argv );
  Item item;
  item.show();
  return app.exec();
}

