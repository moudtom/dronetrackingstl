#include "mymaparcgis.h"
// C++ API headers
#include "Basemap.h"
// Other headers
#include "Map.h"
#include "MapGraphicsView.h"
#include "ArcGISRuntimeEnvironment.h"
#include "GraphicsOverlay.h"
#include "PolylineBuilder.h"
#include "PolygonBuilder.h"
#include "SimpleMarkerSymbol.h"
#include "SimpleFillSymbol.h"
#include "PictureMarkerSymbol.h"
#include "MapQuickView.h"
using namespace Esri::ArcGISRuntime;

myMapArcgis::myMapArcgis(QWidget *parent) : QMainWindow(parent)
{
    // Create a map using the ArcGISNavigation BasemapStyle
    m_map = new Map(BasemapStyle::ArcGISImagery, this);

    // Create the Widget view
    m_mapView = new MapGraphicsView(this);

    // Set map to map view
    m_mapView->setMap(m_map);
    //m_mapViewLine->setMap(m_map);

    setCentralWidget(m_mapView);

    setupMap();

    GraphicsOverlay* overlay = new GraphicsOverlay(this);
    GraphicsOverlay* overlayLine = new GraphicsOverlay(this);
    createGraphicsLine(overlayLine);
    createGraphics(overlay);

    m_mapView->graphicsOverlays()->append(overlay);
    m_mapView->graphicsOverlays()->append(overlayLine);


    geoLocatedTarget.OldLon =100.6307906;
    geoLocatedTarget.OldLat = 13.9186136;
}

void myMapArcgis::setupMap()
{
    const Point center(100.6307906, 13.9186136, SpatialReference::wgs84());
    const Viewpoint viewpoint(center, 10000.0);
    m_mapView->setViewpoint(viewpoint);

}

void myMapArcgis::createGraphics(Esri::ArcGISRuntime::GraphicsOverlay *overlay)
{
    if(geoLocatedTarget.lon != geoLocatedTarget.OldLon||geoLocatedTarget.heading!=geoLocatedTarget.Oldheading)
    {
        if(geoLocatedTarget.lat!=geoLocatedTarget.OldLat||geoLocatedTarget.heading!=geoLocatedTarget.Oldheading){


                m_mapView->graphicsOverlays()->clear();
        }
    }

     // Create a point
     const Point dume_beach(geoLocatedTarget.lon, geoLocatedTarget.lat, SpatialReference::wgs84());
     const Point dume_beachHis(geoLocatedTarget.OldLon, geoLocatedTarget.OldLat, SpatialReference::wgs84());

     // Create symbols for the point
//

     // create a campsite symbol from a URL
     //PictureMarkerSymbol* campSymbol = new PictureMarkerSymbol(QUrl("https://sampleserver6.arcgisonline.com/arcgis/rest/services/Recreation/FeatureServer/0/images/e82f744ebb069bb35b234b3fea46deae"), this);
     //setWidthAndHeight(campSymbol, 38.0f);
     // create an orange symbol from a file path

     PictureMarkerSymbol* droneSymbol = new PictureMarkerSymbol(QUrl("qrc:/drone griffin.png"), this);
     setWidthAndHeight(droneSymbol, 100.0f);
     droneSymbol->setAngle(geoLocatedTarget.heading);
     //setWidthAndHeight(blueSymbol, 64.0f);

     //point_symbol->setOutline(point_outline);

     // Create a graphic to display the point with its symbology
     Graphic* point_graphic = new Graphic(dume_beach, droneSymbol , this);

     // Add point graphic to the graphics overlay
     //overlay->graphics()->append(point_graphic);


     overlay->graphics()->append(point_graphic);


}

void myMapArcgis::createGraphicsLine(Esri::ArcGISRuntime::GraphicsOverlay *overlayLine)
{

    // Create a line
    PolylineBuilder* polyline_builder = new PolylineBuilder(SpatialReference::wgs84(), this);
    polyline_builder->addPoint(geoLocatedTarget.OldLon, geoLocatedTarget.OldLat);
    polyline_builder->addPoint(geoLocatedTarget.lon, geoLocatedTarget.lat);

    // Create a symbol for the line
    SimpleLineSymbol* line_symbol = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor(Qt::red), 3, this);

    // Create a graphic to display the line with its symbology
    Graphic* polyline_graphic = new Graphic(polyline_builder->toGeometry(), line_symbol, this);
    // Add line graphic to the graphics overlay
    overlayLine->graphics()->append(polyline_graphic);

    connect(m_mapView, &MapGraphicsView::mouseClicked, this, [this](QMouseEvent mouse)
      {

        const Point mapPoint = m_mapView->screenToLocation(mouse.x(), mouse.y());

        //const Point dume_beach(mapPoint.x(),mapPoint.y(), SpatialReference::wgs84());
        SimpleLineSymbol* point_outline = new SimpleLineSymbol(SimpleLineSymbolStyle::Solid, QColor(Qt::blue), 3, this);
        SimpleMarkerSymbol* point_symbol = new SimpleMarkerSymbol(SimpleMarkerSymbolStyle::Circle, QColor(Qt::red), 10, this);
        point_symbol->setOutline(point_outline);
        m_graphicsOverlay = new GraphicsOverlay(this);
        m_mapView->graphicsOverlays()->append(m_graphicsOverlay);
        //Create a graphic to display the point with its symbology
        Graphic* point_graphic2 = new Graphic(this);
         point_graphic2->setGeometry(mapPoint);
         point_graphic2->setSymbol( point_symbol);
         m_graphicsOverlay->graphics()->append( point_graphic2);
        // Add point graphic to the graphics overlay
        //overlayLine->graphics()->append(point_graphic2);


        //qDebug()<<<<","<<mapPoint.y();
      });


}

void myMapArcgis::setLonLat(double _lon, double _lat,float _heading){
    geoLocatedTarget.lon = _lon;
    geoLocatedTarget.lat = _lat;
    geoLocatedTarget.heading=_heading;

    GraphicsOverlay* overlayLine = new GraphicsOverlay(this);
    createGraphicsLine(overlayLine);
     m_mapView->graphicsOverlays()->append(overlayLine);

    GraphicsOverlay* overlay = new GraphicsOverlay(this);
    createGraphics(overlay);
    m_mapView->graphicsOverlays()->append(overlay);

    CountTime++;
    if(CountTime > 80){
        Point ptEsriHeadquarters(geoLocatedTarget.lon,geoLocatedTarget.lat , SpatialReference(4326));
        m_mapView->setViewpointCenter(ptEsriHeadquarters);
       // m_mapView->setViewpointScale(1000.0);
        CountTime = 0;
    }
    geoLocatedTarget.OldLon = geoLocatedTarget.lon;
    geoLocatedTarget.OldLat = geoLocatedTarget.lat;
    geoLocatedTarget.Oldheading = geoLocatedTarget.heading;
}

void myMapArcgis::setWidthAndHeight(Esri::ArcGISRuntime::PictureMarkerSymbol *symbol, float size)
{
    symbol->setWidth(size);
    symbol->setHeight(size);

}


