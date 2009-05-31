#include "Controller.h"
#include "Item.h"
#include "Instructions.h"

#include <QtCore>
#include <stdlib.h>

Controller::Controller() {
  this->correctOverall = 0;
  this->wrongOverall = 0;
  this->round = 0;
  this->currentItem = new Item();
  this->currentInstructions = new Instructions();

  connect(
    this->currentItem, SIGNAL( answered( bool, int ) ),
    this, SLOT( getAnswer( bool, int ) )
  );
  connect(
    this->currentInstructions, SIGNAL( read() ),
    this, SLOT( instructionsRead() )
  );

  QTime midnight( 0, 0 );
  srand( midnight.msecsTo( QTime::currentTime() ) );

  this->versionA = rand() % 2;
}

Controller::~Controller() {
  delete this->logStream;
  this->currentItem->deleteLater();
  this->currentInstructions->deleteLater();
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
    QMessageBox::warning(
      NULL,
      "IAT",
      trUtf8( "Mam już wyniki takiej osoby." )
    );
    return false;
  }

  this->logFile.setFileName( fileName );

  if ( ! this->logFile.open( QIODevice::Text | QIODevice::WriteOnly ) ) {
    return false;
  }
  this->logStream = new QTextStream( &this->logFile );
  this->logStream->setCodec( QTextCodec::codecForName( "UTF-8" ) );

  if ( this->versionA ) {
    *this->logStream << trUtf8( "Wersja A.\n\n" );
  }
  else {
    *this->logStream << trUtf8( "Wersja B.\n\n" );
  }

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
  stream.setCodec( QTextCodec::codecForName( "UTF-8" ) );

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
  if ( --this->wordsLeftToSkip < 0 ) {
    results.push_back( msecs );
    overallResults.push_back( msecs );

    if ( this->showingLeftWord && left ) {
      ++this->correctAnswers;
      ++this->correctOverall;
      *this->logStream << trUtf8( "Poprawna po " ) << msecs << "\n";
    }
    else if ( ! this->showingLeftWord && ! left ) {
      ++this->correctAnswers;
      ++this->correctOverall;
      *this->logStream << trUtf8( "Poprawna po " ) << msecs << "\n";
    }
    else {
      ++this->wrongAnswers;
      ++this->wrongOverall;
      *this->logStream << trUtf8( "Błędna po " ) << msecs << "\n";
    }
  }
  else {
    if ( this->showingLeftWord && left ) {
      *this->logStream << trUtf8( "Pominięta poprawna po " ) << msecs << "\n";
    }
    else if ( ! this->showingLeftWord && ! left ) {
      *this->logStream << trUtf8( "Pominięta poprawna po " ) << msecs << "\n";
    }
    else {
      *this->logStream << trUtf8( "Pominięta błędna po " ) << msecs << "\n";
    }
  }

  if ( --this->wordsLeft > 0 ) {
    this->prepareNewWord();
  }
  else {
    this->prepareNewRound();
  }
}

void Controller::instructionsRead() {
  this->prepareNewWord();
  this->currentItem->showFullScreen();
  this->currentInstructions->hide();
}

void Controller::prepareNewWord() {
  this->currentItem->init(
    this->l1n, this->l2n, this->r1n, this->r2n, this->getRandomWord()
  );
}

void Controller::prepareNewRound() {
  this->writeRoundResults();
  this->results.clear();
  this->correctAnswers = 0;
  this->wrongAnswers = 0;

  QString instructions;

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
      instructions = trUtf8(
        "Naciśnij f, jeżeli wyraz wyświetlony na środku ekranu należy do "
        "kategorii "
      );
      instructions.append( this->l1n ).append( ".\n\n" );
      instructions.append( trUtf8(
        "Naciśnij j, jeżeli należy do kategorii "
      ) );
      instructions.append( this->r1n ).append( "." );
      break;
    case 1:
      this->l1w = &this->leftCategory2Words;
      this->l2w = &this->leftCategory2Words;
      this->r1w = &this->rightCategory2Words;
      this->r2w = &this->rightCategory2Words;
      this->l1n = this->leftCategory2Name;
      this->l2n = QString();
      this->r1n = this->rightCategory2Name;
      this->r2n = QString();
      instructions = trUtf8(
        "Naciśnij f, jeżeli wyraz wyświetlony na środku ekranu należy do "
        "kategorii "
      );
      instructions.append( this->l1n ).append( ".\n\n" );
      instructions.append( trUtf8(
        "Naciśnij j, jeżeli należy do kategorii "
      ) );
      instructions.append( this->r1n ).append( "." );
      break;
    case 2:
      if ( this->versionA ) {
        this->l1w = &this->leftCategory1Words;
        this->l2w = &this->leftCategory2Words;
        this->r1w = &this->rightCategory1Words;
        this->r2w = &this->rightCategory2Words;
        this->l1n = this->leftCategory1Name;
        this->l2n = this->leftCategory2Name;
        this->r1n = this->rightCategory1Name;
        this->r2n = this->rightCategory2Name;
      }
      else {
        this->l1w = &this->rightCategory1Words;
        this->l2w = &this->leftCategory2Words;
        this->r1w = &this->leftCategory1Words;
        this->r2w = &this->rightCategory2Words;
        this->l1n = this->rightCategory1Name;
        this->l2n = this->leftCategory2Name;
        this->r1n = this->leftCategory1Name;
        this->r2n = this->rightCategory2Name;
      }
      instructions = trUtf8(
        "Naciśnij f, jeżeli wyraz wyświetlony na środku ekranu należy do "
        "kategorii "
      );
      instructions.append( this->l1n ).append( " lub " ).append( this->l2n );
      instructions.append( trUtf8(
        ".\n\nNaciśnij j, jeżeli należy do kategorii "
      ) );
      instructions.append( this->r1n ).append( " lub " ).append( this->r2n );
      instructions.append( "." );
      break;
    case 3:
      this->l1w = &this->rightCategory1Words;
      this->l2w = &this->rightCategory1Words;
      this->r1w = &this->leftCategory1Words;
      this->r2w = &this->leftCategory1Words;
      this->l1n = this->rightCategory1Name;
      this->l2n = QString();
      this->r1n = this->leftCategory1Name;
      this->r2n = QString();
      instructions = trUtf8(
        "Naciśnij f, jeżeli wyraz wyświetlony na środku ekranu należy do "
        "kategorii "
      );
      instructions.append( this->l1n ).append( ".\n\n" );
      instructions.append( trUtf8(
        "Naciśnij j, jeżeli należy do kategorii "
      ) );
      instructions.append( this->r1n ).append( "." );
      break;
    case 4:
      if ( this->versionA ) {
        this->l1w = &this->rightCategory1Words;
        this->l2w = &this->leftCategory2Words;
        this->r1w = &this->leftCategory1Words;
        this->r2w = &this->rightCategory2Words;
        this->l1n = this->rightCategory1Name;
        this->l2n = this->leftCategory2Name;
        this->r1n = this->leftCategory1Name;
        this->r2n = this->rightCategory2Name;
      }
      else {
        this->l1w = &this->leftCategory1Words;
        this->l2w = &this->leftCategory2Words;
        this->r1w = &this->rightCategory1Words;
        this->r2w = &this->rightCategory2Words;
        this->l1n = this->leftCategory1Name;
        this->l2n = this->leftCategory2Name;
        this->r1n = this->rightCategory1Name;
        this->r2n = this->rightCategory2Name;
      }
      instructions = trUtf8(
        "Naciśnij f, jeżeli wyraz wyświetlony na środku ekranu należy do "
        "kategorii "
      );
      instructions.append( this->l1n ).append( " lub " ).append( this->l2n );
      instructions.append( trUtf8(
        ".\n\nNaciśnij j, jeżeli należy do kategorii "
      ) );
      instructions.append( this->r1n ).append( " lub " ).append( this->r2n );
      instructions.append( "." );
      break;
    default:
      this->writeOverallResults();
      qApp->quit();
      return;
  }

  this->currentInstructions->setText( instructions );
  this->currentInstructions->showFullScreen();
  this->currentItem->hide();

  ++this->round;
  this->wordsLeft = WORDS_PER_ROUND;
  this->wordsLeftToSkip = WORDS_TO_SKIP;
}

void Controller::writeRoundResults() {
  if ( this->results.size() < 1 ) {
    return;
  }

  int sum = 0;
  for ( int i = 0; i < this->results.size(); ++i ) {
    sum += this->results.at( i );
  }
  int avg = sum / this->results.size();
  int prcWrong =
    100 * this->wrongAnswers /
    ( this->wrongAnswers + this->correctAnswers );

  *this->logStream << trUtf8( "Średnia rundy to: " ) << avg << "\n";
  *this->logStream << trUtf8( "Błędnych odpowiedzi: " ) << prcWrong << "%\n\n";
}

void Controller::writeOverallResults() {
  if ( this->overallResults.size() < 1 ) {
    return;
  }

  int sum = 0;
  for ( int i = 0; i < this->overallResults.size(); ++i ) {
    sum += this->overallResults.at( i );
  }
  int avg = sum / this->overallResults.size();
  int prcWrong =
    100 * this->wrongOverall /
    ( this->wrongOverall + this->correctOverall );

  *this->logStream << trUtf8( "Średnia ogólna to: " ) << avg << "\n";
  *this->logStream
    << trUtf8( "Błędnych odpowiedzi w sumie: " )
    << prcWrong << "%\n\n";
}

