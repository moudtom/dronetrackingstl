#include "mainwindow.h"

#include <QApplication>
#include <QMessageBox>
#include "ArcGISRuntimeEnvironment.h"
#include "mymaparcgis.h"
using namespace Esri::ArcGISRuntime;
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QGuiApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
    // Use of Esri location services, including basemaps and geocoding, requires
    // either an ArcGIS identity or an API key. For more information see
    // https://links.esri.com/arcgis-runtime-security-auth.

    // 1. ArcGIS identity: An ArcGIS named user account that is a member of an
    // organization in ArcGIS Online or ArcGIS Enterprise.

    // 2. API key: A permanent key that gives your application access to Esri
    // location services. Create a new API key or access existing API keys from
    // your ArcGIS for Developers dashboard (https://links.esri.com/arcgis-api-keys).

    //const QString apiKey = QStringLiteral("C:/Users/moudp/Documents/Basemap/basemap3.tfw");
    const QString apiKey = QStringLiteral("AAPK7ee37e644bc747f18c8a3adcf5cd7fbbYqcZ5qo1j61p7Zd7ROs8jV-7YmjCLP9qieEfFFJjoNgNwKDSVjvK0dDgRJIQwi9V");
    if (apiKey.isEmpty())
    {
        qWarning() << "Use of Esri location services, including basemaps, requires "
                  "you to authenticate with an ArcGIS identity or set the API Key property.";
    }
    else
    {
        ArcGISRuntimeEnvironment::setApiKey(apiKey);
        //ArcGISRuntimeEnvironment::("C:/Users/moudp/Documents/Basemap/basemap3.tfw");
    }

    // Production deployment of applications built with ArcGIS Runtime requires you to
    // license ArcGIS Runtime functionality. For more information see
    // https://links.esri.com/arcgis-runtime-license-and-deploy.

    // ArcGISRuntimeEnvironment::setLicense("Place license string in here");

    //  use this code to check for initialization errors
    //  QObject::connect(ArcGISRuntimeEnvironment::instance(), &ArcGISRuntimeEnvironment::errorOccurred, [](const Error& error){
    //    QMessageBox msgBox;
    //    msgBox.setText(error.message);
    //    msgBox.exec();
    //  });

    //  if (ArcGISRuntimeEnvironment::initialize() == false)
    //  {
    //    application.quit();
    //    return 1;
    //  }

//    myMapArcgis applicationWindow;
//    applicationWindow.setMinimumWidth(800);
//    applicationWindow.setMinimumHeight(600);
//    applicationWindow.show();

    MainWindow w;
    w.show();
    return a.exec();
}
