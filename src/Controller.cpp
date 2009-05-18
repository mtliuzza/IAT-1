#include "Controller.h"
#include "Item.h"

#include <QtCore>
#include <stdlib.h>

Controller::Controller() {
  this->correctAnswers = 0;
  this->wrongAnswers = 0;
  this->round = 0;
  sranddev();
}

Controller::~Controller() {
  delete this->logStream;
}

bool Controller::init() {
  if ( ! this->readFile(
    "left1.txt", this->leftCategory1Name, this->leftCategory1Words
  ) ) {
    return false;
  }
  if ( ! this->readFile(
    "left2.txt", this->leftCategory2Name, this->leftCategory2Words
  ) ) {
    return false;
  }
  if ( ! this->readFile(
    "right1.txt", this->rightCategory1Name, this->rightCategory1Words
  ) ) {
    return false;
  }
  if ( ! this->readFile(
    "right2.txt", this->rightCategory2Name, this->rightCategory2Words
  ) ) {
    return false;
  }

  bool userPressedOk;
  QString fileName = QInputDialog::getText(
    NULL,
    "IAT",
    trUtf8( "Twoje imię:" ),
    QLineEdit::Normal,
    "",
    &userPressedOk
  );

  if ( ! userPressedOk ) {
    return false;
  }

  fileName.prepend( "outputs/" );
  fileName.append( ".txt" );

  if ( QFile::exists( fileName ) ) {
    QMessageBox::warning( NULL, "IAT", "Mam już wyniki takiej osoby." );
    return false;
  }

  this->logFile.setFileName( fileName );

  if ( ! this->logFile.open( QIODevice::Text | QIODevice::WriteOnly ) ) {
    return false;
  }
  this->logStream = new QTextStream( &this->logFile );

  return true;
}

void Controller::start() {
  this->prepareNewRound();
}

bool Controller::readFile(
  QString fileName, QString& categoryName, QStringList& categoryItems
) {
  fileName.prepend( "inputs/" );
  QFile file( fileName );
  file.open( QIODevice::ReadOnly | QIODevice::Text );
  QTextStream stream( &file );

  QString contents = stream.readAll();
  QStringList lines = contents.split( "\n", QString::SkipEmptyParts );

  if ( lines.size() < 2 ) {
    return false;
  }

  categoryName = lines.takeFirst();
  categoryItems = lines;

  return true;
}

QString Controller::getRandomWord() {
  int list = rand() % 4;
  QStringList* words;
  switch ( list ) {
    case 0: words = this->l1w; this->showingLeftWord = true; break;
    case 1: words = this->l2w; this->showingLeftWord = true; break;
    case 2: words = this->r1w; this->showingLeftWord = false; break;
    case 3: words = this->r2w; this->showingLeftWord = false; break;
    default: return QString();
  }

  int element = rand() % words->size();
  return words->at( element );
}

void Controller::getAnswer( bool left, int msecs ) {
  results.push_back( msecs );

  if ( this->showingLeftWord && left ) {
    ++this->correctAnswers;
    *this->logStream << "Correct in " << msecs << "\n";
  }
  else if ( ! this->showingLeftWord && ! left ) {
    ++this->correctAnswers;
    *this->logStream << "Correct in " << msecs << "\n";
  }
  else {
    ++this->wrongAnswers;
    *this->logStream << "Wrong in " << msecs << "\n";
  }

  this->currentItem->hide();
  this->currentItem->deleteLater();

  if ( --this->wordsLeft > 0 ) {
    this->prepareNewWord();
  }
  else {
    this->prepareNewRound();
  }
}

void Controller::prepareNewWord() {
  this->currentItem = new Item();
  this->currentItem->init(
    this->l1n, this->l2n, this->r1n, this->r2n, this->getRandomWord()
  );

  connect(
    this->currentItem, SIGNAL( answered( bool, int ) ),
    this, SLOT( getAnswer( bool, int ) )
  );

  this->currentItem->show();
}

void Controller::prepareNewRound() {
  switch ( this->round ) {
    case 0:
      this->l1w = &this->leftCategory1Words;
      this->l2w = &this->leftCategory1Words;
      this->r1w = &this->rightCategory1Words;
      this->r2w = &this->rightCategory1Words;
      this->l1n = this->leftCategory1Name;
      this->l2n = QString();
      this->r1n = this->rightCategory1Name;
      this->r2n = QString();
      break;
    case 1:
      this->l1w = &this->leftCategory1Words;
      this->l2w = &this->leftCategory2Words;
      this->r1w = &this->rightCategory1Words;
      this->r2w = &this->rightCategory2Words;
      this->l1n = this->leftCategory1Name;
      this->l2n = this->leftCategory2Name;
      this->r1n = this->rightCategory1Name;
      this->r2n = this->rightCategory2Name;
      break;
    default:
      this->writeRoundResults();
      qApp->quit();
      return;
  }
  this->writeRoundResults();

  ++this->round;
  this->wordsLeft = WORDS_PER_ROUND;
  this->prepareNewWord();
}

void Controller::writeRoundResults() {
  
}

