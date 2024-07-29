// CVL includes
#include "imageviewer.h"

// STD includes
#include <cstdint>

// QT includes
#include <QApplication>
#include <QCommandLineParser>

int32_t main( [[maybe_unused]] int32_t argc, [[maybe_unused]] char** argv )
{
    QApplication app( argc, argv );
    QGuiApplication::setApplicationDisplayName(
        ImageViewer::tr( "Image Viewer" ) );
    QCommandLineParser commandLineParser;
    commandLineParser.addHelpOption( );
    commandLineParser.addPositionalArgument(
        ImageViewer::tr( "[file]" ), ImageViewer::tr( "Image file to open." ) );
    commandLineParser.process( QCoreApplication::arguments( ) );
    ImageViewer imageViewer;
    if ( ! commandLineParser.positionalArguments( ).isEmpty( ) &&
         ! imageViewer.loadFile(
             commandLineParser.positionalArguments( ).constFirst( ) ) )
    {
        return -1;
    }
    imageViewer.show( );
    return app.exec( );
}
