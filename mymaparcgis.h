#ifndef MYMAPARCGIS_H
#define MYMAPARCGIS_H
namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapGraphicsView;
class Graphic;
class GraphicsOverlay;
class PictureMarkerSymbol;
class RouteTask;
}
}

#include <QMainWindow>
#include <QObject>
#include <QtDebug>
#include "PictureMarkerSymbol.h"

class myMapArcgis : public QMainWindow
{
    Q_OBJECT

public:
    explicit myMapArcgis(QWidget *parent = nullptr);

    void setupMap();

    void createGraphics(Esri::ArcGISRuntime::GraphicsOverlay* overlay);

    void createGraphicsLine(Esri::ArcGISRuntime::GraphicsOverlay* overlayLine);

    void setLonLat(double _lon, double _lat,float _heading);
    void setWidthAndHeight(Esri::ArcGISRuntime::PictureMarkerSymbol* symbol, float size);

    struct GeoLocatedTarget{
        double lat ;
        double lon ;
        double OldLon;
        double OldLat;
        float heading;
        float Oldheading;
        GeoLocatedTarget(){
            lon = 0.0;
            lat = 0.0;
            OldLon=0.0;
            OldLat=0.0;
            heading=0.0;
            Oldheading=0.0;
        }

    } geoLocatedTarget;


    int CountTime=0;
    int CountTimeClear=0;

private:
    Esri::ArcGISRuntime::Map*                   m_map = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*       m_mapView = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*       m_mapViewLine = nullptr;
    Esri::ArcGISRuntime::Graphic* m_startGraphic = nullptr;
     Esri::ArcGISRuntime::GraphicsOverlay* m_graphicsOverlay = nullptr;



};

#endif // MYMAPARCGIS_H
