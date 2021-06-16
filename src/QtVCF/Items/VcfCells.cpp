#include <qtvcf.h>

N::VcfCells:: VcfCells  ( VcfHeadParaments  )
            : VcfNode   ( parent , item , p )
            , Dimension ( QSize(1,1)        )
{
  Configure ( ) ;
}

N::VcfCells::~VcfCells(void)
{
}

void N::VcfCells::Configure(void)
{
  setFlag(ItemIsMovable           ,true )               ;
  setFlag(ItemIsSelectable        ,true )               ;
  setFlag(ItemIsFocusable         ,true )               ;
  setFlag(ItemClipsToShape        ,false)               ;
  setFlag(ItemClipsChildrenToShape,false)               ;
  ///////////////////////////////////////////////////////
  Painter . addMap   ( "Default" , 0                  ) ;
  Painter . addPen   ( 0 , QColor ( 192 , 192 , 192 ) ) ;
  Painter . addPen   ( 1 , QColor (   0 ,   0 ,   0 ) ) ;
  Painter . addBrush ( 0 , QColor ( 240 , 240 , 240 ) ) ;
}

QString N::VcfCells::Value(int row,int column)
{
  return "" ;
}

QRectF N::VcfCells::Cell(int row,int column)
{
  return QRectF(0,0,0,0) ;
}

void N::VcfCells::setColumnCount(int column)
{
  Dimension . setWidth  ( column ) ;
}

void N::VcfCells::setRowCount(int row)
{
  Dimension . setHeight ( row    ) ;
}

int N::VcfCells::columnCount(void)
{
  return Dimension . width  ( ) ;
}

int N::VcfCells::rowCount(void)
{
  return Dimension . height ( ) ;
}

void N::VcfCells::RenderAppearance(QPainter * p,QRectF Region,bool clip,bool color)
{
  PaintPathes ( p ) ;
}

void N::VcfCells::RenderObjects(QPainter * p,QRectF Region,bool clip,bool color)
{
  for (int row=0;row<rowCount();row++)               {
    for (int column=0;column<columnCount();column++) {
      Paint ( p , row , column )                     ;
    }                                                ;
  }                                                  ;
}

void N::VcfCells::Paint(QPainter * painter,int row,int column)
{
}
