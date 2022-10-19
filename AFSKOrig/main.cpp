#include <QCoreApplication>
#include <QCommandLineParser>
#include "data_types.h"
#include "AfskDecoder.h"
#include <iostream>     // For cout

int main(int argc, char *argv[])
{
    QCoreApplication app(argc, argv);
     QCoreApplication::setApplicationName("Afsk Decoder");
     QCoreApplication::setApplicationVersion("0.0-a");

//    QCommandLineParser parser;
//        parser.setApplicationDescription("Test helper");
//        parser.addHelpOption();
//        parser.addVersionOption();
//        parser.addPositionalArgument("source", QCoreApplication::translate("main", "Source file to copy."));
//        parser.addPositionalArgument("destination", QCoreApplication::translate("main", "Destination directory."));

//        // A boolean option with a single name (-p)
//        QCommandLineOption showProgressOption("p", QCoreApplication::translate("main", "Show progress during copy"));
//        parser.addOption(showProgressOption);
    // A boolean option with multiple names (-f, --force)
//        QCommandLineOption forceOption(QStringList() << "f" << "force",
//                QCoreApplication::translate("main", "Overwrite existing files."));
//        parser.addOption(forceOption);



    QCommandLineParser  parser;
            parser.setApplicationDescription("afsk decoder");
            parser.addHelpOption();
            parser.addVersionOption();

//           program.add_option("-f", "--file")
//                   .help("input wav file to decode")
//                   .required();

           QCommandLineOption fileOption(QStringList() << "f" << "file",
                      QCoreApplication::translate("main", "input wav file to decode."));
           parser.addOption(fileOption);


           // A boolean option with a single name (-p)
           QCommandLineOption minZero("min-zero",  "min width of zero rectangle in microseconds.","min-zero");
           minZero.setDefaultValue("600.0");    // double
           parser.addOption(minZero);

           QCommandLineOption maxZero("max-zero",  "maz width of zero rectangle in microseconds.","max-zero");
           maxZero.setDefaultValue("680.0");    // double
           parser.addOption(maxZero);

           QCommandLineOption minOne("min-one",  "min width of one rectangle in microseconds","min-one");
           minOne.setDefaultValue("220.0");     // double
           parser.addOption(minOne);

           QCommandLineOption maxOne("max-one",  "max width of one rectangle in microseconds","max-one");
           maxOne.setDefaultValue("360.0");     // double
           parser.addOption(maxOne);

           QCommandLineOption zcResolution("zc-resolution",  "resolution of zero-crossing detection in number of samples.","zc-resolution");
           zcResolution.setDefaultValue("3");   // Integer
           parser.addOption(zcResolution);


           /*

           program.add_argument("--min-zero")
                   .help("min width of zero rectangle in microseconds")
                   .action([](const std::string& value) { return std::stod(value); })
                   .default_value(600.0);

           program.add_argument("--max-zero")
                   .help("max width of zero rectangle in microseconds")
                   .action([](const std::string& value) { return std::stod(value); })
                   .default_value(680.0);

           program.add_argument("--min-one")
                   .help("min width of one rectangle in microseconds")
                   .action([](const std::string& value) { return std::stod(value); })
                   .default_value(220.0);

           program.add_argument("--max-one")
                   .help("max width of one rectangle in microseconds")
                   .action([](const std::string& value) { return std::stod(value); })
                   .default_value(360.0);

           program.add_argument("--zc-resolution")
                   .help("resolution of zero-crossing detection in number of samples")
                   .action([](const std::string& value) { return std::stoi(value); })
                   .default_value(3);

           program.add_argument("--raw")
                   .help("print raw bits and bytes")
                   .default_value(false)
                   .implicit_value(true);

           program.add_argument("--info")
                   .help("print info messages about decoding")
                   .default_value(false)
                   .implicit_value(true);

           program.add_argument("--errors")
                   .help("print decoding errors")
                   .default_value(false)
                   .implicit_value(true);

           try {
               program.parse_args(argc, argv);
            }
            catch (const std::runtime_error& err) {
                   cout << "bad argument" << endl;
                   cout << program.help().str() << endl;
                   return 1;
            }
            catch (const std::invalid_argument& err) {
                   cout << "bad argument" << endl;
                   cout << program.help().str() << endl;
                   return 1;
            }
*/

           // Process the actual command line arguments given by the user
               parser.process(app);

               double zeroMinWidth =  parser.value("min-zero").toDouble();
               double zeroMaxWidth =  parser.value("max-zero").toDouble();

               double oneMinWidth  = parser.value("min-one").toDouble();
               double oneMaxWidth  = parser.value("max-one").toDouble();
               Samples::size_type zcDetectResolution = parser.value("zc-resolution").toInt();

               std::cout << "MinZero Set as " << zeroMinWidth << std::endl;
               std::cout << "MaxZero Set as " << zeroMaxWidth << std::endl;
               std::cout << "MinOne  Set as " << oneMinWidth << std::endl;
               std::cout << "MaxOne  Set as " << oneMaxWidth << std::endl;
               std::cout << "zr-Res  Set as " << zcDetectResolution << std::endl;

               const QStringList args = parser.positionalArguments();
               // source is args.at(0), destination is args.at(1)

           std::string filename;


           bool printRaw;
           bool printErrors;
           bool printInfo;
/*
           try {
                   filename = program.get<string>("--file");
                   zeroMinWidth = program.get<double>("--min-zero");
                   zeroMaxWidth = program.get<double>("--max-zero");
                   oneMinWidth = program.get<double>("--min-one");
                   oneMaxWidth = program.get<double>("--max-one");
                   zcDetectResolution = program.get<int>("--zc-resolution");
                   printRaw = program.get<bool>("--raw");
                   printInfo = program.get<bool>("--info");
                   printErrors = program.get<bool>("--errors");
           } catch(const std::runtime_error) {
                   cout << "bad argument" << endl;
                   cout << program.help().str() << endl;
                   return 1;
           }
*/
           AfskDecoder decoder;

           auto normalWriter = [](std::string msg) { std::cout << msg; };
           auto redWriter = [](std::string msg) { std::cout << "\x1B[31m" << msg << "\033[0m"; };
           auto magentaWriter = [](std::string msg) { std::cout << msg; };

         /*  decoder.decode(
                   move(filename),
                   {
                           .zeroBitRangeInMicroseconds = { zeroMinWidth, zeroMaxWidth },
                           .oneBitRangeInMicroseconds = { oneMinWidth, oneMaxWidth },
                   },
                   zcDetectResolution,
                   printInfo ? normalWriter : nullptr,
                   printErrors ? redWriter : nullptr,
                   normalWriter,
                   printRaw ? magentaWriter : nullptr
           );
           */
           return 0;
    return app.exec();
}
