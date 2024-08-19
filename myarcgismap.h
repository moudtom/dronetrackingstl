#ifndef MYARCGISMAP_H
#define MYARCGISMAP_H
namespace Esri
{
namespace ArcGISRuntime
{
class Map;
class MapGraphicsView;
class GraphicsOverlay;
}
}
#include <QObject>

class myArcgisMap : public QObject
{
    Q_OBJECT
public:
    explicit myArcgisMap(QObject *parent = nullptr);

signals:

public slots:



private:
    Esri::ArcGISRuntime::Map*                   m_map = nullptr;
    Esri::ArcGISRuntime::MapGraphicsView*       m_mapView = nullptr;
    void setupMap();
    void createGraphics(Esri::ArcGISRuntime::GraphicsOverlay* overlay);

};

#endif // MYARCGISMAP_H
