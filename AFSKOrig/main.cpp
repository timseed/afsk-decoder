#include "AfskDecoder.h"
#include "data_types.h"
#include <QCommandLineParser>
#include <QCoreApplication>
#include <QFile>    // Check if it exists
#include <iostream> // For cout

int main(int argc, char *argv[]) {
  QCoreApplication app(argc, argv);
  QCoreApplication::setApplicationName("Afsk Decoder");
  QCoreApplication::setApplicationVersion("0.0-a");

  QCommandLineParser parser;
  parser.setApplicationDescription("afsk decoder");
  parser.addHelpOption();
  parser.addVersionOption();

  QCommandLineOption fileOption(
      QStringList() << "f"
                    << "file",
      QCoreApplication::translate("main", "input wav file to decode."),
      "filename");
  parser.addOption(fileOption);

  // A boolean option with a single name (-p)
  QCommandLineOption showRaw("raw", "show raw data.");
  parser.addOption(showRaw);

  QCommandLineOption showInfo("info", "show info data.");
  parser.addOption(showInfo);

  QCommandLineOption showError("error", "show error data.");
  parser.addOption(showError);

  // Options with default Values
  QCommandLineOption minZero(
      "min-zero", "min width of zero rectangle in microseconds.", "min-zero");
  minZero.setDefaultValue("600.0"); // double
  parser.addOption(minZero);

  QCommandLineOption maxZero(
      "max-zero", "maz width of zero rectangle in microseconds.", "max-zero");
  maxZero.setDefaultValue("680.0"); // double
  parser.addOption(maxZero);

  QCommandLineOption minOne(
      "min-one", "min width of one rectangle in microseconds", "min-one");
  minOne.setDefaultValue("220.0"); // double
  parser.addOption(minOne);

  QCommandLineOption maxOne(
      "max-one", "max width of one rectangle in microseconds", "max-one");
  maxOne.setDefaultValue("360.0"); // double
  parser.addOption(maxOne);

  QCommandLineOption zcResolution(
      "zc-resolution",
      "resolution of zero-crossing detection in number of samples.",
      "zc-resolution");
  zcResolution.setDefaultValue("3"); // Integer
  parser.addOption(zcResolution);

  // Process the actual command line arguments given by the user
  parser.process(app);

  double zeroMinWidth = parser.value("min-zero").toDouble();
  double zeroMaxWidth = parser.value("max-zero").toDouble();

  double oneMinWidth = parser.value("min-one").toDouble();
  double oneMaxWidth = parser.value("max-one").toDouble();
  Samples::size_type zcDetectResolution = parser.value("zc-resolution").toInt();

  std::cout << "MinZero Set as " << zeroMinWidth << std::endl;
  std::cout << "MaxZero Set as " << zeroMaxWidth << std::endl;
  std::cout << "MinOne  Set as " << oneMinWidth << std::endl;
  std::cout << "MaxOne  Set as " << oneMaxWidth << std::endl;
  std::cout << "zr-Res  Set as " << zcDetectResolution << std::endl;

  const QStringList args = parser.positionalArguments();

  bool printRaw = parser.isSet(showRaw);
  bool printInfo = parser.isSet(showInfo);
  bool printErrors = parser.isSet(showError);

  std::cout << "showRaw  Set as " << printRaw << std::endl;
  std::cout << "showInfo Set as " << printInfo << std::endl;
  std::cout << "showErr  Set as " << printErrors << std::endl;

  QString filename = parser.value("file");

  if (filename.length()) {
    std::cout << "File  Set as " << filename.toStdString() << std::endl;
    if (!QFile::exists(filename))
    {
        std::cout << "Filedoes not exist";
        exit(0);
    }

  } else {
    parser.helpText();
    std::cout << "Filename Missing";
    exit(0);
  }

  AfskDecoder decoder;

  auto normalWriter = [](std::string msg) { std::cout << msg; };
  auto redWriter = [](std::string msg) {
    std::cout << "\x1B[31m" << msg << "\033[0m";
  };
  auto magentaWriter = [](std::string msg) { std::cout << msg; };

   decoder.decode(
               std::move(filename.toStdString()),
            {
                    .zeroBitRangeInMicroseconds = { zeroMinWidth, zeroMaxWidth
    }, .oneBitRangeInMicroseconds = { oneMinWidth, oneMaxWidth },
            },
            zcDetectResolution,
            printInfo ? normalWriter : nullptr,
            printErrors ? redWriter : nullptr,
            normalWriter,
            printRaw ? magentaWriter : nullptr
    );


  return app.exec();
}
